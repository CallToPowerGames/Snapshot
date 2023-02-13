/**
  * Snapshot
  * Copyright 2014 Denis Meyer, CallToPower Software
  */
#include "mainwindow.h"
#include <QApplication>
#include <QException>
#include <QSplashScreen>
#include <QDebug>

int main(int argc, char *argv[])
{
    int res = -1;

    try
    {
        QApplication a(argc, argv);

        QSplashScreen *splash = new QSplashScreen;
        splash->setPixmap(QPixmap(":/img/logo"));
        splash->show();

        MainWindow w;
        QTimer::singleShot(2000, splash, SLOT(close()));
        QTimer::singleShot(2000, &w, SLOT(show()));

        res = a.exec();
    }
    catch(QException &e)
    {
        qCritical() << QString("Exception: %1").arg( e.what() );
    }
    catch(...)
    {
        qCritical() << QString("Unhandled Exception");
    }

    return res;
}
