#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <QMutex>
#include <QMouseEvent>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "imageprocessingthread.h"

class CQtOpenCVViewerGl : public QOpenGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit CQtOpenCVViewerGl(QWidget *parent = 0);
    virtual ~CQtOpenCVViewerGl();
    void setFramerate(double framerate);
//    void capture_frame();
    ImageProcessingThread* getImageProcessingThread();

signals:
    void    imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

public slots:
    void startCapture();
    void stopCapture();
    void connectImageProcessorToDisplayMethod();
    bool    showImage(); /// Used to set the image to be viewed

protected:
    void 	initializeGL(); /// OpenGL initialization
    void 	paintGL(); /// OpenGL Rendering
    void 	resizeGL(int width, int height);        /// Widget Resize Event

    void        updateScene();
    void        renderImage();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    ImageProcessingThread *image_processing_thread;
    QImage      mRenderQtImg;           /// Qt image to be rendered
    QImage      mResizedImg;
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown
    cv::Mat     *sharedCVImage;
    QColor      mBgColor;		/// Background color

    float       mImgRatio;             /// height/width ratio

    int mRenderWidth;
    int mRenderHeight;
    int mRenderPosX;
    int mRenderPosY;

    QMutex *draw_mutex;
    QPointF bbox_first_point;
    QPointF bbox_second_point;

    void recalculatePosition();
    void startImageProcessingThread();
    void stopImageProcessingThread();
    Point_<double> convert(const QPointF & pt);
};

#endif // CQTOPENCVVIEWERGL_H
