/**
  * Snapshot
  * Copyright 2014 Denis Meyer, CallToPower Software
  */
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QSound>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libimgfilter.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(0),
    about(0),
    currTimerTimeLeft(3),
    updateImage(true),
    grey(false),
    blur(false),
    invert(false),
    fancy1(false),
    fancy2(false),
    foggyRetro(false),
    switchRB(false),
    mirror(false),
    many(false),
    flipHorizontally(false),
    flipVertically(false),
    rotateAngle(0),
    partyEdgesThreshold(0),
    partyEdgesSteps(30),
    edgeThickness(1),
    nrOfImages(0),
    nrOfImageRows(0),
    mouseX(0),
    mouseY(0)
{
    ui->setupUi(this);

    ui->buttonSnap->setText("");

    timer = new QTimer(this);

    about = new TextWindow(this);
    about->setTitle("About");
    about->setText(":/text/about");

    connect(ui->widget_img, SIGNAL(mouseMoved(QMouseEvent*)), this, SLOT(on_mouseMoved(QMouseEvent*)));
    connect(ui->widget_img, SIGNAL(mouseReleased(QMouseEvent*)), this, SLOT(on_mouseReleased(QMouseEvent*)));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));

    emit on_actionStart_triggered();
}

MainWindow::~MainWindow()
{
    if(timer) {
        delete timer;
    }
    if(about) {
        delete about;
    }
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    about->setVisible(true);
}

void MainWindow::on_mouseMoved(QMouseEvent *event)
{
    mouseX = event->pos().x();
    mouseY = event->pos().y();
}

void MainWindow::on_mouseReleased(QMouseEvent *event)
{
    // on_actionSaveImage_triggered();
}

void MainWindow::on_buttonSnap_released()
{
    if(!timer->isActive()) {
        ui->buttonSnap->setEnabled(false);
        timer->setInterval(1000);
        timer->start();
        ui->buttonSnap->setText(QString("%1").arg(currTimerTimeLeft));
        QSound::play(":/audio/dot");
    }
}

void MainWindow::timerTimeout()
{
    --currTimerTimeLeft;
    if(currTimerTimeLeft <= 0) {
        timer->stop();
        currTimerTimeLeft = 3;
        ui->buttonSnap->setText("");
        ui->buttonSnap->setEnabled(true);
        QSound::play(":/audio/snap");
        on_actionSaveImage_triggered();
    } else {
        ui->buttonSnap->setText(QString("%1").arg(currTimerTimeLeft));
        QSound::play(":/audio/dot");
    }
}

void MainWindow::on_actionSaveImage_triggered()
{
    try {
        image = filter_switchRB(image);
        image = filter_flip(image, 0);
        ui->widget_img->showImage(image);
        QImage img = ui->widget_img->getImage();

        timerEvent(0);

        updateImage = false;

        QString saveFilename = QFileDialog::getSaveFileName(this, "Save as", "Filename", "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");

        if(!saveFilename.trimmed().isEmpty()) {
            QString saveExtension = "PNG";
            int pos = saveFilename.lastIndexOf('.');
            if (pos >= 0) {
                saveExtension = saveFilename.mid(pos + 1);
            }

            if(!img.save(saveFilename)) {
                QMessageBox::warning(this, "Error", "File could not be saved", QMessageBox::Ok);
            }
        }
        updateImage = true;
    } catch(...) {
        updateImage = true;
        qCritical() << QString("Unhandled Exception in on_actionSaveImage_triggered");
    }
}

void MainWindow::on_actionStart_triggered()
{
    if(!mCapture.isOpened()) {
        if(!mCapture.open(0)) {
            QMessageBox::warning(this, "Error", "No camera found. Quitting...", QMessageBox::Ok);
            exit(0);
            return;
        }

        startTimer(0);
    }
}

void MainWindow::on_actionResetFilters_triggered()
{
    grey = false;
    blur = false;
    invert = false;
    fancy1 = false;
    fancy2 = false;
    foggyRetro = false;
    switchRB = false;
}

void MainWindow::on_actionResetNumberOfImages_triggered()
{
    emit on_action1_image_triggered();
}

void MainWindow::on_actionResetEffects_triggered()
{
    flipHorizontally = false;
    flipVertically = false;
    mirror = false;
    many = false;
}

void MainWindow::on_actionResetAll_triggered()
{
    on_actionResetFilters_triggered();
    on_actionResetEffects_triggered();
    on_actionResetNumberOfImages_triggered();
    ui->sliderRotate->setValue(0);
    ui->sliderPartyEdges->setValue(0);
    ui->sliderEdgeThickness->setValue(1);
}

void MainWindow::on_actionGrey_triggered()
{
    grey = !grey;
    if(grey) {
        fancy1 = false;
        fancy2 = false;
        on_action1_image_triggered();
    }
}

void MainWindow::on_actionBlur_triggered()
{
    blur = !blur;
}

void MainWindow::on_actionInvert_triggered()
{
    invert = !invert;
}

void MainWindow::on_actionFancy1_triggered()
{
    fancy1 = !fancy1;
    if(fancy1) {
        grey = false;
        fancy2 = false;
    }
}

void MainWindow::on_actionFancy2_triggered()
{
    fancy2 = !fancy2;
    if(fancy2) {
        grey = false;
        fancy1 = false;
    }
}

void MainWindow::on_actionFoggyRetro_triggered()
{
    foggyRetro = !foggyRetro;
}

void MainWindow::on_actionSwitchRB_triggered()
{
    switchRB = !switchRB;
}

void MainWindow::on_actionMirror_triggered()
{
    mirror = !mirror;
    if(mirror) {
        many = false;
    }
}

void MainWindow::on_actionMany_triggered()
{
    many = !many;
    if(many) {
        mirror = false;
    }
}

void MainWindow::on_sliderRotate_valueChanged(int val)
{
    rotateAngle = val;
}

void MainWindow::on_sliderPartyEdges_valueChanged(int val)
{
    partyEdgesThreshold = val * partyEdgesSteps;
}

void MainWindow::on_sliderEdgeThickness_valueChanged(int val)
{
    edgeThickness = val;
}

void MainWindow::on_action1_image_triggered()
{
    nrOfImages = 0;
    nrOfImageRows = 0;
}

void MainWindow::on_action4_images_triggered()
{
    nrOfImages = 4;
    nrOfImageRows = 2;
    grey = false;
}

void MainWindow::on_action9_images_triggered()
{
    nrOfImages = 9;
    nrOfImageRows = 3;
    grey = false;
}

void MainWindow::on_action12_images_triggered()
{
    nrOfImages = 12;
    nrOfImageRows = 4;
    grey = false;
}

void MainWindow::on_action16_images_triggered()
{
    nrOfImages = 16;
    nrOfImageRows = 4;
    grey = false;
}

void MainWindow::on_actionFlipHorizontally_triggered()
{
    flipHorizontally = !flipHorizontally;
}

void MainWindow::on_actionFlipVertically_triggered()
{
    flipVertically = !flipVertically;
}

void MainWindow::on_actionFlipHorizontallyAndVertically_triggered()
{
    on_actionFlipHorizontally_triggered();
    on_actionFlipVertically_triggered();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(updateImage) {
        mCapture >> image;

        ui->widget_img->resizeGL(ui->widget_img->size().width(), ui->widget_img->size().height());

        try {
            if(blur) {
                image = filter_blur(image);
            }
            if(invert) {
                image = filter_invert(image);
            }
            if(fancy1) {
                image = filter_fancy1(image);
            }
            if(fancy2) {
                image = filter_fancy2(image);
            }
            if(foggyRetro) {
                image = filter_foggyRetro(image);
            }
            if(switchRB) {
                image = filter_switchRB(image);
            }
            if(grey) {
                image = filter_grey(image);
            }
            if(mirror) {
                image = filter_mirror(image);
            }
            if(many) {
                image = filter_many(image);
            }
            if(partyEdgesThreshold != 0) {
                image = filter_partyEdges(image, partyEdgesThreshold, edgeThickness, grey);
            }
            if((rotateAngle != 0) && (rotateAngle != 360)) {
                image = filter_rotate(image, rotateAngle);
            }
            if(flipHorizontally && flipVertically) {
                image = filter_flip(image, -1);
            } else if(flipHorizontally) {
                image = filter_flip(image, 0);
            } else if(flipVertically) {
                image = filter_flip(image, 1);
            }
            if(nrOfImages != 0) {
                image = filter_manyInOne(image, nrOfImages, nrOfImageRows);
            }
            ui->widget_img->showImage(image);
        } catch(...) {
            qCritical() << QString("Unhandled Exception in timerEvent");
        }
    }
}
