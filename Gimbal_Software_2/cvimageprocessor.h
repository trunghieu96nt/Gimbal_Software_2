#ifndef CVIMAGEPROCESSOR_H
#define CVIMAGEPROCESSOR_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QTimer>
#include <QString>
#include <QImage>
#include <QMutex>
#include <QPoint>
#include <QFile>
#include <QFileInfo>

#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include <opencv2/core/ocl.hpp>
//#include <opencv/cxcore.h>
//#include <opencv/cv.h>

//#include "cvimage.h"
using namespace std;
using namespace cv;
class CVImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit CVImageProcessor(cv::Mat shared_cv_image, QMutex *mutex, QObject *parent = 0);
    virtual ~CVImageProcessor();
    void set_framerate(double framerate);

    /*public member variables*/
    QRect get_object_location();
    cv::Mat shared_cv_image;

signals:
    void current_frame_updated();

public slots:
    void capture_frame();
    void start_capture();
    void stop_capture();

protected:
    double framerate;
    cv::Mat cur_image;
    cv::VideoCapture capture_device;
    QTimer *timer;
    QMutex *mutex;
    QRect object_location;
private:
    Ptr<Tracker> tracker;
    bool b_start_tracking;
    Rect2d bbox;
};

#endif // CVIMAGEPROCESSOR_H
