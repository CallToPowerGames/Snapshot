/**
  * Snapshot
  * Copyright 2014 Denis Meyer, CallToPower Software
  */
#ifndef OPENCVVIEWER_H
#define OPENCVVIEWER_H

#include <QGlWidget>
#include <QMouseEvent>
#include <opencv2/core/core.hpp>

class OpenCVViewer : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenCVViewer(QWidget *parent = 0);
    void resized(int width, int height);
    void resizeGL(int width, int height); // Widget Resize Event
    QImage getImage();

private:
    bool mSceneChanged; // Indicates when OpenGL view is to be redrawn

    QImage mRenderQtImg; // Qt image to be rendered
    cv::Mat mOrigImage; // original OpenCV image to be shown

    QColor mBgColor; // Background color

    int mOutH; // Resized Image height
    int mOutW; // Resized Image width
    float mImgratio; // height/width ratio

    int mPosX; // Top left X position to render image in the center of widget
    int mPosY; // Top left Y position to render image in the center of widget

signals:
    void imageSizeChanged(int outW, int outH); // Used to resize the image outside the widget
    void mouseMoved(QMouseEvent *event);
    void mouseReleased(QMouseEvent *event);

public slots:
    bool showImage( cv::Mat image ); // Used to set the image to be viewed

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void initializeGL(); // OpenGL initialization
    void paintGL(); // OpenGL Rendering

    void updateScene(); // Forces a scene update
    void renderImage(); // Render image on openGL frame
};

#endif // OPENCVVIEWER_H
