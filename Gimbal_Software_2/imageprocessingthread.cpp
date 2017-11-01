#include "imageprocessingthread.h"

ImageProcessingThread::ImageProcessingThread(cv::Mat *sharedCVImage, QMutex *mutex, QObject *parent) :
    QThread(parent)
{
    this->sharedCVImage = sharedCVImage;
    this->mutex = mutex;
}

ImageProcessingThread::~ImageProcessingThread()
{

}

void ImageProcessingThread::run()
{
    this->imageProcessor = new ImageProcessor(this->sharedCVImage, this->mutex);
    emit imageProcessorInstantiated();
    exec();
}

ImageProcessor* ImageProcessingThread::getImageProcessor()
{
    return this->imageProcessor;
}
