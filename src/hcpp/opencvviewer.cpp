/**
  * Snapshot
  * Copyright 2014 Denis Meyer, CallToPower Software
  */
#include "opencvviewer.h"

OpenCVViewer::OpenCVViewer(QWidget *parent) :
    QGLWidget(parent)
{
    mSceneChanged = false;
    mBgColor = QColor::fromRgb(150, 150, 150);

    mOutH = 0;
    mOutW = 0;
    mImgratio = 4.0f/3.0f; // Default image ratio

    mPosX = 0;
    mPosY = 0;
}

void OpenCVViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (this->rect().contains(event->pos())) {
        emit mouseMoved(event);
    }
}

void OpenCVViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->rect().contains(event->pos())) {
        emit mouseReleased(event);
    }
}

void OpenCVViewer::resized(int width, int height)
{
    resizeGL(width, height);
}

QImage OpenCVViewer::getImage()
{
    return QImage(mRenderQtImg);
}

void OpenCVViewer::initializeGL()
{
    makeCurrent();
    qglClearColor(mBgColor.darker());
}

void OpenCVViewer::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, 0, height, 0, 1); // To Draw image in the center of the area

    glMatrixMode(GL_MODELVIEW);

    // Scaled Image Sizes
    mOutH = width / mImgratio;
    mOutW = width;

    if(mOutH>height)
    {
        mOutW = height*mImgratio;
        mOutH = height;
    }

    emit imageSizeChanged(mOutW, mOutH);

    mPosX = (width-mOutW)/2;
    mPosY = (height-mOutH)/2;

    mSceneChanged = true;

    updateScene();
}

void OpenCVViewer::updateScene()
{
    if(mSceneChanged && this->isVisible()) {
        updateGL();
    }
}

void OpenCVViewer::paintGL()
{
    makeCurrent();

    if(!mSceneChanged) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();

    mSceneChanged = false;
}

void OpenCVViewer::renderImage()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull()) {
        glLoadIdentity();

        QImage image;

        glPushMatrix(); {
            int imW = mRenderQtImg.width();
            int imH = mRenderQtImg.height();

            // The image is to be resized to fit the widget?
            if((imW != this->size().width()) && (imH != this->size().height())) {
                image = mRenderQtImg.scaled(QSize(mOutW,mOutH), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            }
            else {
                image = mRenderQtImg;
            }
            // Centering image in draw area
            glRasterPos2i(mPosX, mPosY);

            imW = image.width();
            imH = image.height();

            glDrawPixels(imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        }
        glPopMatrix();

        glFlush();
    }
}

bool OpenCVViewer::showImage(cv::Mat image)
{
    image.copyTo(mOrigImage);

    mImgratio = (float)image.cols/(float)image.rows;

    if(mOrigImage.channels() == 3) {
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),  mOrigImage.cols, mOrigImage.rows, mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    }
    else if(mOrigImage.channels() == 1) {
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data), mOrigImage.cols, mOrigImage.rows, mOrigImage.step, QImage::Format_Indexed8);
    }
    else {
        return false;
    }

    mRenderQtImg = QGLWidget::convertToGLFormat(mRenderQtImg);

    mSceneChanged = true;

    updateScene();

    return true;
}
