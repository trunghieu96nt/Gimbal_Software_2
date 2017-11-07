#ifndef CVIMAGEPROCESSOR_H
#define CVIMAGEPROCESSOR_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
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
#include <opencv2/core/ocl.hpp>

#include "pid.h"

using namespace std;
using namespace cv;

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    static const int FRAME_WIDTH;
    static const int FRAME_HEIGHT;
    static const int INVALID_COORDINATE;
    static const float MAX_VEL_CMD_DEG_S;

    explicit ImageProcessor(cv::Mat *sharedCVImage, QMutex *mutex, QObject *parent = 0);
    virtual ~ImageProcessor();
    void setFramerate(double framerate);
    void setBoundingBox(Rect2d bbox);
    Point2d getObjectLocation();
    PID getAzTrackingPID();
    PID getElTrackingPID();
    void setPIDTrackingParam(int idx_Axis, int idx_Kx, float data);
    void setPIDTrackingParams(float KpAz, float KiAz, float KdAz, float Kff1Az, float Kff2Az,
                                              float KpEl, float KiEl, float KdEl, float Kff1El, float Kff2El);

signals:
    void currentFrameUpdated();
    void velCmdUpdated(float az_vel, float el_vel);

public slots:
    void startCapture();
    void startCapture(bool start);
    void stopCapture();

private slots:
    void captureFrame();

private:
    static const string TRACKER_TYPES[6];
    double framerate;
    cv::Mat currentImage;
    cv::VideoCapture captureDevice;
    cv::Mat *sharedCVImage;
    QMutex *mutex;
    QTimer *timer;
    Ptr<Tracker> tracker;
    bool isInitiated;
    bool gotFirstOKFrame;
    double lastCaptureTime;
    int cntLossFrame;
    Rect2d bbox;
    Point2d objectLocation;
    float omegaZCmd, omegaYCmd;
    PID xTrackingPID, yTrackingPID;

    void createTracker(void);
    void initPIDTracking(void);

};

#endif // CVIMAGEPROCESSOR_H
