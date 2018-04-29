#include "cvimageprocessor.h"
#define DEFAULT_FRAMERATE 29.0
#define DEFAULT_X_DEVIATION_THRESHOLD 19
#define DEFAULT_Y_DEVIATION_THRESHOLD 190
// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace cv;

CVImageProcessor::CVImageProcessor(cv::Mat shared_cv_image, QMutex *mutex, QObject *parent) :
    QObject(parent)
{
    /*bind our instance's shared_cv_image and mutex member vars to the CVImage* and QMutex* passed to this constructor*/
    this->shared_cv_image = shared_cv_image;
    this->mutex = mutex;

    /*timer governs capture interval*/
    this->timer = new QTimer(this);

    /*defaults*/
    this->framerate = DEFAULT_FRAMERATE;

    /*capture from first detected device*/
    //this->capture_device = cvCaptureFromCAM( CV_CAP_ANY );
    //TODO: check if device is connected
    //this->capture_device.open( 0 );
    this->capture_device.open( "D:\\ObjectTrackingTest.mp4" );

    /*default to NULL for instance's IplImage and QImage pointers*/
    //this->cur_image = NULL;

    /*set the capture interval*/
    this->timer->setInterval((1000/this->framerate));

    /*bind QTimer::timeout() for timer to our frame-grabbing function*/
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(capture_frame()));

    //this->start_capture();
    string trackerTypes[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
    // Create a tracker
    string trackerType = trackerTypes[2];
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

CVImageProcessor::~CVImageProcessor()
{
    /*delete heap-allocated objects*/
    delete this->timer;
    //cvReleaseMemStorage(&this->storage);
}

void CVImageProcessor::start_capture()
{
    this->b_start_tracking = false;
    this->timer->start();
}

void CVImageProcessor::stop_capture()
{
    this->timer->stop();
}

void CVImageProcessor::capture_frame()
{
    bool img_proc_result;
    this->capture_device >> this->cur_image;
    // run algorithm
    if (!b_start_tracking)
    {        
        bbox.x = 300;
        bbox.y = 200;
        bbox.width = 30;
        bbox.height = 50;
        //bbox = selectROI(cur_image, false);
        b_start_tracking = true;
        tracker->init(cur_image, bbox);
    }
    else
    {
        float fps;
        double timer = (double)getTickCount();
        img_proc_result = this->tracker->update(cur_image, bbox);
        fps = getTickFrequency() / ((double)getTickCount() - timer);
        if (img_proc_result)
        {
            // Tracking success : Draw the tracked object
            rectangle(cur_image, bbox, Scalar( 255, 0, 0 ), 2, 1 );
            /*write coords to terminal*/
            qDebug()<<"object coord: x="<<bbox.x<<"   y="<<this->bbox.y;
        }
        else
        {
            // Tracking failure detected.
            putText(cur_image, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        // Display FPS on frame
        putText(cur_image, "FPS : " + SSTR(int(fps)), Point(100,50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50,170,50), 2);
    }
    // display result
    /*write to the shared cv image object (lock before writing)...*/
    mutex->lock();
    this->shared_cv_image = this->cur_image;
    mutex->unlock();
    emit current_frame_updated();
}

void CVImageProcessor::set_framerate(double framerate)
{
    this->framerate = framerate;
    this->timer->setInterval(1000/this->framerate);
}

QRect CVImageProcessor::get_object_location()
{
    return this->object_location;
}
