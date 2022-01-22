#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <QObject>





class Camera3D : public QObject
{
    Q_OBJECT
public:
    explicit Camera3D(QObject *parent = nullptr);

signals:

public slots:
       /*初始化相机*/
    int initCamera3D();
    /*执行一次拍照*/
    bool setupCamera3D();
    bool stopCamera3D();

public slots:
     void focalQuit();
     void focalSTART_3D();
     void focalSTART_RAW();
     void focalSTART_LINE();
     void focalSTART_BATCH();
     void focalSTOP();
     void focalSTOP_GRABBING();
     void focalSTART_GRABBING();
     int getInitStatus();

private:
    int initStatus;

};

#endif // CAMERA3D_H
