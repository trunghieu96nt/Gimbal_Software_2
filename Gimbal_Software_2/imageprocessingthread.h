#ifndef IMAGEPROCESSINGTHREAD_H
#define IMAGEPROCESSINGTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTimer>

#include <opencv2/opencv.hpp>
//#include <opencv2/core/utility.hpp>
//#include <opencv2/core/core.hpp>

#include "imageprocessor.h"

using namespace cv;

class ImageProcessingThread : public QThread
{
    Q_OBJECT
public:
    explicit ImageProcessingThread(cv::Mat *sharedCVImage, QMutex *mutex, QObject *parent = 0);
    virtual ~ImageProcessingThread();
    void run();
    ImageProcessor* getImageProcessor();

signals:
   void imageProcessorInstantiated();

private:
    cv::Mat *sharedCVImage;
    QMutex *mutex;
    ImageProcessor *imageProcessor;
};

#endif // IMAGEPROCESSINGTHREAD_H
