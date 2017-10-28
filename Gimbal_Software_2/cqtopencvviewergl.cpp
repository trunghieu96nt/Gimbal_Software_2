#include "cqtopencvviewergl.h"
#define DEFAULT_FRAMERATE 20.0
CQtOpenCVViewerGl::CQtOpenCVViewerGl(QWidget *parent) :
QOpenGLWidget(parent)
{
    mBgColor = QColor::fromRgb(150, 150, 150);
    //this->shared_cv_image = new CVImage();
    this->draw_mutex = new QMutex();
    this->image_processing_thread = new ImageProcessingThread(this->shared_cv_image, this->draw_mutex);

    this->start_image_processing_thread();//run image processing thread

    this->image_processing_thread->set_framerate(DEFAULT_FRAMERATE);

    /*This ensures that once the CVImageProcessor that is instantiated and owned by the image_processing_thread has been allocated,
    its "current_frame_updated()" signal will be connected to this CVWidget instance's "display_current_frame()" slot
    (the connection routine happens in the body of the "connect_image_processor_to_display_method()" slot -- a little hacky, I know, but seemed
    to be the simplest solution to avoiding NULL pointer exceptions within the CVWidget class.*/
    //QObject::connect(this->image_processing_thread, SIGNAL(image_processor_instantiated()), this, SLOT(connect_image_processor_to_display_method()));
    connect_image_processor_to_display_method();
}

void CQtOpenCVViewerGl::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();

    float r = ((float)mBgColor.darker().red())/255.0f;
    float g = ((float)mBgColor.darker().green())/255.0f;
    float b = ((float)mBgColor.darker().blue())/255.0f;
    glClearColor(r,g,b,1.0f);
}

void CQtOpenCVViewerGl::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    recalculatePosition();

    emit imageSizeChanged(mRenderWidth, mRenderHeight);

    updateScene();
}

void CQtOpenCVViewerGl::updateScene()
{
    if (this->isVisible()) update();
}

void CQtOpenCVViewerGl::paintGL()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();
}

void CQtOpenCVViewerGl::renderImage()
{
    draw_mutex->lock();
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        glPushMatrix();
        {
            if (mResizedImg.width() <= 0)
            {
                if (mRenderWidth == mRenderQtImg.width() && mRenderHeight == mRenderQtImg.height())
                    mResizedImg = mRenderQtImg;
                else
                    mResizedImg = mRenderQtImg.scaled(QSize(mRenderWidth, mRenderHeight),
                                                      Qt::IgnoreAspectRatio,
                                                      Qt::SmoothTransformation);
            }

            // ---> Centering image in draw area

            glRasterPos2i(mRenderPosX, mRenderPosY);

            glPixelZoom(1, -1);
			
            glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_RGBA, GL_UNSIGNED_BYTE, mResizedImg.bits());
			
        }
        glPopMatrix();

        // end
        glFlush();
    }

    draw_mutex->unlock();
}

void CQtOpenCVViewerGl::recalculatePosition()
{
    mImgRatio = (float)mOrigImage.cols/(float)mOrigImage.rows;

    mRenderWidth = this->size().width();
    mRenderHeight = floor(mRenderWidth / mImgRatio);

    if (mRenderHeight > this->size().height())
    {
        mRenderHeight = this->size().height();
        mRenderWidth = floor(mRenderHeight * mImgRatio);
    }

    mRenderPosX = floor((this->size().width() - mRenderWidth) / 2);
    mRenderPosY = -floor((this->size().height() - mRenderHeight) / 2);

    mResizedImg = QImage();
}

bool CQtOpenCVViewerGl::showImage()
{
    draw_mutex->lock();
    if (this->shared_cv_image.channels() == 3)
        cvtColor(this->shared_cv_image, mOrigImage, CV_BGR2RGBA);
    else if (this->shared_cv_image.channels() == 1)
        cvtColor(this->shared_cv_image, mOrigImage, CV_GRAY2RGBA);
    else if (this->shared_cv_image.channels() == 4)
        mOrigImage = this->shared_cv_image;
    else return false;

    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                          mOrigImage.cols, mOrigImage.rows,
                          mOrigImage.step1(), QImage::Format_RGB32);
	
    recalculatePosition();

    updateScene();
    draw_mutex->unlock();
    return true;
}

CQtOpenCVViewerGl::~CQtOpenCVViewerGl()
{
    this->image_processing_thread->exit();
    //wait for image proc thr to properly exit...
    while(this->image_processing_thread->isRunning()){}
    delete this->image_processing_thread;
}


void CQtOpenCVViewerGl::connect_image_processor_to_display_method()
{
    QObject::connect(this->image_processing_thread, SIGNAL(current_frame_updated()), this, SLOT(showImage()));
}

void CQtOpenCVViewerGl::start_image_processing_thread()
{
    this->image_processing_thread->start();
}

/*terminates the image processing thread*/
void CQtOpenCVViewerGl::stop_image_processing_thread()
{
    this->image_processing_thread->terminate();
}

void CQtOpenCVViewerGl::start_capture()
{
    if(this->image_processing_thread != NULL)
        this->image_processing_thread->start_capture();
}

void CQtOpenCVViewerGl::stop_capture()
{
    if(this->image_processing_thread != NULL)
        this->image_processing_thread->stop_capture();
}

void CQtOpenCVViewerGl::set_framerate(double framerate)
{
    if(this->image_processing_thread != NULL)
        this->image_processing_thread->set_framerate(framerate);
}

void CQtOpenCVViewerGl::capture_frame()
{
    if(this->image_processing_thread != NULL)
        this->image_processing_thread->capture_frame();
}
