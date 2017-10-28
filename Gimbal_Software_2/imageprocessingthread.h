#ifndef IMAGEPROCESSINGTHREAD_H
#define IMAGEPROCESSINGTHREAD_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class ImageProcessingThread : public QThread
{
    Q_OBJECT
public:
    explicit ImageProcessingThread(cv::Mat shared_cv_image, QMutex *mutex, QObject *parent = 0);
    virtual ~ImageProcessingThread();
    void run();
    void set_framerate(double framerate);
    /*public member variables*/
    Point2d get_object_location();
signals:
    void current_frame_updated();

public slots:
    void capture_frame();
    void start_capture();
    void stop_capture();

private:
    double framerate;
    cv::Mat cur_image;
    cv::VideoCapture capture_device;
    cv::Mat shared_cv_image;
    QMutex *mutex;
    QTimer *timer;
    Ptr<Tracker> tracker;
    bool b_start_tracking;
    Rect2d bbox;
    Point2d object_location;
};

#endif // IMAGEPROCESSINGTHREAD_H
