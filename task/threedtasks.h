#ifndef THREEDTASKS_H
#define THREEDTASKS_H

#include <QObject>
#include "camera3d.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QVector>


#include "resultdata.h"
#include <QString>
#include <QSettings>
#include <QVariant>



#include "camera3d.h"



class ThreeDTasks : public QObject
{
    Q_OBJECT
public:
    explicit ThreeDTasks(QObject *parent = nullptr);

    enum TaskType{
      SideLowerPart = 1,
      SideUpperPart = 2,
      SideAllPart = 3,
    };

    enum SideLowerType{
        SideLowerAllType = 0,
        SideLowerTask1 = 1,
        SideLowerTask2 = 2,
        SideLowerTask3 = 3,
        SideLowerTask4 = 4,
    };

    enum SideUpperType{
        SideUpperAllType = 0,
        SideUpperTask1 = 1,
        SideUpperTask2 = 2,
        SideUpperTask3 = 3,
        SideUpperTask4 = 4,
    };
    void setStationNum(int numGE);
    void setTask(Camera3D *camera3D,int objectIndex,TaskType taskType,SideLowerType sideLowerType,SideUpperType sideUpperType,int layer);
    void setCamera3d(Camera3D *camera3D);


signals:
    void setupCameraFinished(int numGW,bool);//工位号，相机是否启动成功
    void taskFinished();

    void checkSuccessFinished(bool isSideLower); //true:侧面下部  flase:侧面上部
    /*3D相机把结果传自动化，演示方案*/
    void checkSuccessFinished(int Result);

    void checkSuccessFinishedResult(QByteArray Result); //参数：一次检测的所有结果（暂时最多存3个）

    void cameraStatesSig(int States);

public slots:

    void initThreeDTask();
    void setTask(TaskType taskType);
    void setSideLowerType(SideLowerType sideLowerType);
    void setSideUpperType(SideUpperType sideUpperType);


   /*
    * 启动一次检测。
  */
    void setupCheck(int objectIndex,int taskType);

    /*启动一次检测，不区分侧面上部和下部*/
    void setupCheck(int objectIndex,bool isBatch);

    void setupCamera(bool isBatch);

    void stopCameraHandle();

    /*运行算法*/
    void runAlgorithm();

    /*模拟检测结果数据并发送给任务分配线程*/
    void simulationMackCheckResult(const quint8 sidePositionD);


    void doTask();


   /*执行3D相机拍照侧面下部*/
    bool sideLowerTask_handle();

    char sideLowerTask1_handle();
    char sideLowerTask2_handle();
    char sideLowerTask3_handle();
    char sideLowerTask4_handle();


   /*执行3D相机拍照侧面上部*/
    bool sideUpperTask_handle();

    char sideUpperTask1_handle();
    char sideUpperTask2_handle();
    char sideUpperTask3_handle();
    char sideUpperTask4_handle();
    QVector<int> getBodyLayers(cv::Mat &mat2D);

   cv::Mat checkImage(cv::Mat &mat3D,QVector<RESULT>&result,int threshold, int border,int roi_min,int roi_max);


 private:
   /*
    * 从配置文件读取配置数据
  */
   QVariant readkey(QString filemae, QString qstrnodename, QString qstrkeyname);

   /*
    * 测试使用的从指定配置文件获取检测数据
   */
    void makeCheckData(QVector<RESULT>&result,bool isDown,int objectNumber);
    /*
     *测试使用两次侧面的公共部分的检测结果只只使用第一次的结果
    */
    void checkResultHandle(QVector<RESULT>&result,bool isDown); //bool：true  下部     false : 上部


    QByteArray intToByteArray(const int intData);
    quint8 checkData(QByteArray data);


    TaskType m_taskType;
    SideLowerType m_sideLowerType;
    SideUpperType m_sideUpperType;
    int m_layer;
    Camera3D *m_camera3D;
    int m_numGW;
    int m_objectIndex;//端塞序号






};

#endif // THREEDTASKS_H
