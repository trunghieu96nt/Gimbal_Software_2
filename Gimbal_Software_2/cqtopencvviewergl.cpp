#include "cqtopencvviewergl.h"
#define DEFAULT_FRAMERATE 20.0
CQtOpenCVViewerGl::CQtOpenCVViewerGl(QWidget *parent) :
QOpenGLWidget(parent)
{
    mBgColor = QColor::fromRgb(150, 150, 150);
    //this->shared_cv_image = imread("D:\\hoc hanh\\image\\circuit.png", CV_LOAD_IMAGE_COLOR);
    this->sharedCVImage = new cv::Mat(ImageProcessor::FRAME_HEIGHT,
                                        ImageProcessor::FRAME_WIDTH, CV_8UC3, Scalar(0, 0, 0));
    this->draw_mutex = new QMutex();
    this->image_processing_thread = new ImageProcessingThread(this->sharedCVImage, this->draw_mutex);

    this->startImageProcessingThread();//run image processing thread

    /*This ensures that once the CVImageProcessor that is instantiated and owned by the image_processing_thread has been allocated,
    its "current_frame_updated()" signal will be connected to this CVWidget instance's "display_current_frame()" slot
    (the connection routine happens in the body of the "connect_image_processor_to_display_method()" slot -- a little hacky, I know, but seemed
    to be the simplest solution to avoiding NULL pointer exceptions within the CVWidget class.*/
    QObject::connect(this->image_processing_thread, SIGNAL(imageProcessorInstantiated()), this, SLOT(connectImageProcessorToDisplayMethod()));
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
    if (this->sharedCVImage->channels() == 3)
        cvtColor(*(this->sharedCVImage), mOrigImage, CV_BGR2RGBA);
    else if (this->sharedCVImage->channels() == 1)
        cvtColor(*(this->sharedCVImage), mOrigImage, CV_GRAY2RGBA);
    else if (this->sharedCVImage->channels() == 4)
        mOrigImage = *(this->sharedCVImage);
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


void CQtOpenCVViewerGl::connectImageProcessorToDisplayMethod()
{
    QObject::connect(this->image_processing_thread->getImageProcessor(), SIGNAL(currentFrameUpdated()), this, SLOT(showImage()));
}

void CQtOpenCVViewerGl::startImageProcessingThread()
{
    this->image_processing_thread->start();
}

/*terminates the image processing thread*/
void CQtOpenCVViewerGl::stopImageProcessingThread()
{
    this->image_processing_thread->terminate();
}

void CQtOpenCVViewerGl::startCapture()
{
    if(this->image_processing_thread->getImageProcessor() != NULL)
        this->image_processing_thread->getImageProcessor()->startCapture();
}

void CQtOpenCVViewerGl::stopCapture()
{
    if(this->image_processing_thread->getImageProcessor() != NULL)
        this->image_processing_thread->getImageProcessor()->stopCapture();
}

void CQtOpenCVViewerGl::setFramerate(double framerate)
{
    if(this->image_processing_thread->getImageProcessor() != NULL)
        this->image_processing_thread->getImageProcessor()->setFramerate(framerate);
}

void CQtOpenCVViewerGl::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        this->bbox_first_point = event->localPos();
    }
}

void CQtOpenCVViewerGl::mouseReleaseEvent(QMouseEvent *event)
{
    //if (event->buttons() == Qt::LeftButton)
    {
        this->bbox_second_point = event->localPos();
        if(this->image_processing_thread->getImageProcessor() != NULL)
            this->image_processing_thread->getImageProcessor()->setBoundingBox(
                    Rect2d(convert(this->bbox_first_point),convert(this->bbox_second_point)));
    }
}

Point_<double> CQtOpenCVViewerGl::convert(const QPointF & pt)
{
    return Point_<double>(pt.x(), pt.y());
}

ImageProcessingThread* CQtOpenCVViewerGl::getImageProcessingThread()
{
    return image_processing_thread;
}
