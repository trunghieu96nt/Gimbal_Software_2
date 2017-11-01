#include "imageprocessor.h"
#define DEFAULT_FRAMERATE 29

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
    ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace cv;

const string ImageProcessor::TRACKER_TYPES[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
const int ImageProcessor::FRAME_WIDTH = 640;
const int ImageProcessor::FRAME_HEIGHT = 480;
const int ImageProcessor::INVALID_COORDINATE = 100000;
const float ImageProcessor::MAX_VEL_CMD_DEG_S = 45;

ImageProcessor::ImageProcessor(cv::Mat *sharedCVImage, QMutex *mutex, QObject *parent) :
    QObject(parent)
{
    this->sharedCVImage = sharedCVImage;
    this->mutex = mutex;
    /*timer governs capture interval*/
    this->timer = new QTimer(Q_NULLPTR);
    this->timer->moveToThread(this->thread());
    /*defaults*/
    this->framerate = DEFAULT_FRAMERATE;
    this->bbox = Rect2d(0,0,0,0);

    /*capture from first detected device*/
    //TODO: check if device is connected
    //this->capture_device = cvCaptureFromCAM( CV_CAP_ANY );
    this->captureDevice.open( 0 );
    //this->capture_device.open( "D:\\ObjectTrackingTest.mp4" );
    if (cv::ocl::haveOpenCL())
        cv::ocl::setUseOpenCL(true);

    /*default to NULL for instance's IplImage and QImage pointers*/
    //this->cur_image = NULL;

    /*set the capture interval*/
    this->timer->setInterval((1000/this->framerate));

    /*bind QTimer::timeout() for timer to our frame-grabbing function*/
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(captureFrame()));

    createTracker();

    initPIDTracking();
}

ImageProcessor::~ImageProcessor()
{
    /*delete heap-allocated objects*/
    delete this->timer;
    //cvReleaseMemStorage(&this->storage);
}

void ImageProcessor::createTracker(void)
{
    // Create a tracker
    string trackerType = TRACKER_TYPES[2];
#if (CV_MINOR_VERSION < 3)
    {
        this->tracker = Tracker::create(trackerType);
    }
#else
    {
        if (trackerType == "BOOSTING")//slow
            this->tracker = TrackerBoosting::create();
        if (trackerType == "MIL")//slow, cannot detect when object is rotated
            this->tracker = TrackerMIL::create();
        if (trackerType == "KCF")//fast, best performance, but still fails when object is obscured
            this->tracker = TrackerKCF::create();
        if (trackerType == "TLD")//fail
            this->tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")//fast, fails when object is obscured
            this->tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            this->tracker = TrackerGOTURN::create();
    }
#endif
}

void ImageProcessor::startCapture()
{
    this->isInitiated = false;
    this->timer->start();
}

void ImageProcessor::stopCapture()
{
    this->timer->stop();
}

void ImageProcessor::startCapture(bool start)
{
    if (start)
        startCapture();
    else
        stopCapture();
}

void ImageProcessor::captureFrame()
{
    bool imgProcResult;
    this->captureDevice >> this->currentImage;

    if (!isInitiated)
    {
        //bbox.x = 300;
        //bbox.y = 200;
        //bbox.width = 100;
        //bbox.height = 100;
        //bbox = selectROI(cur_image, false);
        if (bbox.area() != 0)
        {
            isInitiated = true;
            tracker->clear();
            createTracker();
            tracker->init(currentImage, bbox);
        }
    }
    else
    {
        float fps;
        double timer = (double)getTickCount();
        imgProcResult = this->tracker->update(currentImage, bbox);
        fps = getTickFrequency() / ((double)getTickCount() - timer);
        if (imgProcResult)
        {
            // Tracking success : Draw the tracked object
            rectangle(currentImage, bbox, Scalar( 255, 0, 0 ), 2, 1 );
            objectLocation = (bbox.br() + bbox.tl())*0.5;
            /* Offset coordinates -> Calculate PID */
            objectLocation.x = objectLocation.x - FRAME_WIDTH/2;
            objectLocation.y = objectLocation.y - FRAME_HEIGHT/2;
            /*write coords to terminal*/
            qDebug()<<"object coord: x="<<objectLocation.x<<"   y="<<this->objectLocation.y;
            /* TODO: calculate dt */
            omegaZCmd = XTrackingPID.PIDCalc(objectLocation.x,MAX_VEL_CMD_DEG_S);
            omegaYCmd = YTrackingPID.PIDCalc(objectLocation.y,MAX_VEL_CMD_DEG_S);
            emit velCmdUpdated(omegaZCmd,omegaYCmd);
        }
        else
        {
            objectLocation = Point2d(INVALID_COORDINATE,INVALID_COORDINATE);
            emit velCmdUpdated(0,0);
            qDebug()<<"Tracking failure detected";
            putText(currentImage, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        // Display FPS on frame
        putText(currentImage, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
    }
    // display result
    /*write to the shared cv image object (lock before writing)...*/
    mutex->lock();
    *(this->sharedCVImage) = this->currentImage;
    //*(this->shared_cv_image) = imread("D:\\hoc hanh\\image\\circuit.png", CV_LOAD_IMAGE_COLOR);

    mutex->unlock();
    emit currentFrameUpdated();
}

void ImageProcessor::initPIDTracking()
{
    XTrackingPID.PIDEnable();
    YTrackingPID.PIDEnable();
    XTrackingPID.PIDSet(0,false);
    YTrackingPID.PIDSet(0,false);
}

void ImageProcessor::setFramerate(double framerate)
{
    mutex->lock();
    this->framerate = framerate;
    this->timer->setInterval(1000/this->framerate);
    mutex->unlock();
}

Point2d ImageProcessor::getObjectLocation()
{
    return this->objectLocation;
}

void ImageProcessor::setBoundingBox(Rect2d bbox)
{
    mutex->lock();
    this->bbox = bbox;
    isInitiated = false;
    mutex->unlock();
}
