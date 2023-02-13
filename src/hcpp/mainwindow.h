/**
  * Snapshot
  * Copyright 2014 Denis Meyer, CallToPower Software
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <QTimer>
#include "textwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    cv::VideoCapture mCapture;
    TextWindow *about;
    cv::Mat image;
    int currTimerTimeLeft;
    bool updateImage;
    bool grey;
    bool blur;
    bool invert;
    bool fancy1;
    bool fancy2;
    bool foggyRetro;
    bool switchRB;
    bool mirror;
    bool many;
    bool flipHorizontally;
    bool flipVertically;
    int rotateAngle;
    int partyEdgesThreshold;
    int partyEdgesSteps;
    int edgeThickness;
    int nrOfImages;
    int nrOfImageRows;
    int mouseX;
    int mouseY;

private slots:
    void timerTimeout();
    void on_actionAbout_triggered();
    void on_mouseMoved(QMouseEvent *event);
    void on_mouseReleased(QMouseEvent *event);
    void on_buttonSnap_released();
    void on_actionSaveImage_triggered();
    void on_actionStart_triggered();
    void on_actionGrey_triggered();
    void on_actionBlur_triggered();
    void on_actionInvert_triggered();
    void on_actionFancy1_triggered();
    void on_actionFancy2_triggered();
    void on_actionFoggyRetro_triggered();
    void on_actionSwitchRB_triggered();
    void on_actionMirror_triggered();
    void on_actionMany_triggered();
    void on_actionResetFilters_triggered();
    void on_actionResetNumberOfImages_triggered();
    void on_actionResetEffects_triggered();
    void on_actionResetAll_triggered();
    void on_sliderRotate_valueChanged(int val);
    void on_sliderPartyEdges_valueChanged(int val);
    void on_sliderEdgeThickness_valueChanged(int val);
    void on_action1_image_triggered();
    void on_action4_images_triggered();
    void on_action9_images_triggered();
    void on_action12_images_triggered();
    void on_action16_images_triggered();
    void on_actionFlipHorizontally_triggered();
    void on_actionFlipVertically_triggered();
    void on_actionFlipHorizontallyAndVertically_triggered();

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
