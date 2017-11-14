#include "imageprocessor.h"
#define DEFAULT_FRAMERATE 30

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
    ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace cv;

const string ImageProcessor::TRACKER_TYPES[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
const int ImageProcessor::FRAME_WIDTH = 800;//640;
const int ImageProcessor::FRAME_HEIGHT = 600;//480;
const int ImageProcessor::INVALID_COORDINATE = 10000;
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
    //this->capture_device.open();
    this->captureDevice.open( 1 );
    this->captureDevice.set(CV_CAP_PROP_FRAME_WIDTH,ImageProcessor::FRAME_WIDTH);
    this->captureDevice.set(CV_CAP_PROP_FRAME_HEIGHT,ImageProcessor::FRAME_HEIGHT);
    //this->capture_device.open( "D:\\ObjectTrackingTest.mp4" );
    //if (cv::ocl::haveOpenCL())
    //    cv::ocl::setUseOpenCL(true);

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
    mutex->lock();
    this->gotFirstOKFrame = false;
    this->isInitiated = false;
    this->cntLossFrame = 0;
    this->timer->start();
    mutex->unlock();
}

void ImageProcessor::stopCapture()
{
    mutex->lock();
    this->timer->stop();
    mutex->unlock();
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
            cntLossFrame = 0;
            // Tracking success : Draw the tracked object
            rectangle(currentImage, bbox, Scalar( 255, 0, 0 ), 2, 1 );
            objectLocation = (bbox.br() + bbox.tl())*0.5;
            /* Offset coordinates -> Calculate PID */
            objectLocation.x = -(objectLocation.x - FRAME_WIDTH/2);
            objectLocation.y = (objectLocation.y - FRAME_HEIGHT/2);
            /*write coords to terminal*/
            qDebug()<<"object coord: x="<<objectLocation.x<<"   y="<<this->objectLocation.y;
            /* TODO: calculate dt */
            if (!gotFirstOKFrame)
            {
                gotFirstOKFrame = true;
                lastCaptureTime = getTickCount();
            }
            else
            {
                double dT = getTickCount() - lastCaptureTime;
                omegaZCmd = xTrackingPID.PIDCalc(objectLocation.x,MAX_VEL_CMD_DEG_S,dT);
                omegaYCmd = yTrackingPID.PIDCalc(objectLocation.y,MAX_VEL_CMD_DEG_S,dT);
                //emit velCmdUpdated(5,10);
                emit velCmdUpdated(omegaZCmd,omegaYCmd,objectLocation.x,objectLocation.y);
            }
        }
        else
        {            
            objectLocation = Point2d(INVALID_COORDINATE,INVALID_COORDINATE);
            if (cntLossFrame <= 3)
                cntLossFrame++;
            else
            {
                cntLossFrame = 0;// send zero velocity continuously when object detection is failed.
                gotFirstOKFrame = false;
                emit velCmdUpdated(0,0,objectLocation.x,objectLocation.y);
            }
            //qDebug()<<"Tracking failure detected";
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
    xTrackingPID.PIDSet(0,false);
    yTrackingPID.PIDSet(0,false);
    //xTrackingPID.PIDSetParams(0.1,0,0,0,0);
    //yTrackingPID.PIDSetParams(0.1,0,0,0,0);
    xTrackingPID.PIDEnable();
    yTrackingPID.PIDEnable();
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

void ImageProcessor::setPIDTrackingParams(float KpAz, float KiAz, float KdAz, float Kff1Az, float Kff2Az,
                                          float KpEl, float KiEl, float KdEl, float Kff1El, float Kff2El)
{
    mutex->lock();
    this->getAzTrackingPID().PIDSetParams(KpAz, KiAz, KdAz, Kff1Az, Kff2Az);
    this->getElTrackingPID().PIDSetParams(KpEl, KiEl, KdEl, Kff1El, Kff2El);
    mutex->unlock();
}

void ImageProcessor::setPIDTrackingParam(int idx_Axis, int idx_Kx, float data)
{
    mutex->lock();
    PID *tmp;
    switch (idx_Axis)
    {
        case 0:
            tmp = &xTrackingPID;
            break;
        case 1:
            tmp = &yTrackingPID;
            break;
        default:
            return;
    }
    switch (idx_Kx) {
        case 0:
            tmp->PIDSetKp(data);
            break;
        case 1:
            tmp->PIDSetKi(data);
            break;
        case 2:
            tmp->PIDSetKd(data);
            break;
        case 3:
            tmp->PIDSetKff1(data);
            break;
        case 4:
            tmp->PIDSetKff2(data);
            break;
        default:
            break;
    }
    mutex->unlock();
}

PID ImageProcessor::getAzTrackingPID()
{
    return xTrackingPID;
}

PID ImageProcessor::getElTrackingPID()
{
    return yTrackingPID;
}
