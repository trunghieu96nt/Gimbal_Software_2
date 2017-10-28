#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <QMutex>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "imageprocessingthread.h"

class CQtOpenCVViewerGl : public QOpenGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit CQtOpenCVViewerGl(QWidget *parent = 0);
    virtual ~CQtOpenCVViewerGl();
    void set_framerate(double framerate);
    void capture_frame();
signals:
    void    imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

public slots:
    void start_capture();
    void stop_capture();
    void connect_image_processor_to_display_method();
    bool    showImage(); /// Used to set the image to be viewed

protected:
    void 	initializeGL(); /// OpenGL initialization
    void 	paintGL(); /// OpenGL Rendering
    void 	resizeGL(int width, int height);        /// Widget Resize Event

    void        updateScene();
    void        renderImage();

private:
    ImageProcessingThread *image_processing_thread;
    QImage      mRenderQtImg;           /// Qt image to be rendered
    QImage      mResizedImg;
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown
    cv::Mat     shared_cv_image;
    QColor      mBgColor;		/// Background color

    float       mImgRatio;             /// height/width ratio

    int mRenderWidth;
    int mRenderHeight;
    int mRenderPosX;
    int mRenderPosY;

    QMutex *draw_mutex;

    void recalculatePosition();
    void start_image_processing_thread();
    void stop_image_processing_thread();
};

#endif // CQTOPENCVVIEWERGL_H
