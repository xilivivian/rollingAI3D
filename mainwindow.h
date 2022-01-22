#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mimageview.h"
#include "model/mmodel.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct rgb_t
    {
       unsigned char   red;
       unsigned char green;
       unsigned char  blue;
    };

signals:
   // void setCheckTaskForTest2(int postion,bool isNon);//postion：1：底部  2：顶部    isNon: true  无端塞   false :有端塞
    void isStartCheck(bool isStart); //开始检测

    void stopCameraSig();


private slots:

    void checkSuccessFinishedHandle(bool isSideLower);

    void testTimeoutHandle();


    void on_stopPushButton_clicked();

    void on_startPushButton_clicked();



    void setCameraStates(int states);

private:
    void setupImageViewers();

     void setupModel1(QWidget*);  //3D工位
     void viewHandle1();

     void setupTaskAllocation3D();

     void upDataImageViewers(bool isSideLower);
     void upDataRealTimeCheckResult();
     void heightMatToImage(cv::Mat heightMat,QImage &image);
    Ui::MainWindow *ui;

    MModel *model1;
    //MImageView *up2DImageViewer;//3D工位
   // MImageView *up3DImageViewer;//3D工位
   // MImageView *down2DImageViewer;//3D工位
   // MImageView *down3DImageViewer;//3D工位

    MImageView *imageViewer2D;
    MImageView *imageViewer3D;

    QTimer *timer;
};

#endif // MAINWINDOW_H
