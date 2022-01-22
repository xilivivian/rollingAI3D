#ifndef TASKALLOCATION_H
#define TASKALLOCATION_H

#include <QObject>
#include<QSerialPort>
#include <QString>
#include <QByteArray>

#include "threedtasks.h"

#include <QTimer>

class TaskAllocation : public QObject
{
    Q_OBJECT
public:
    explicit TaskAllocation(QObject *parent = nullptr);

signals:
    /*所有的2D检测完成，调试2D使用，槽函数启动下一检测*/
    void check2DFinishedAll();

    /*3D和2D都已经检测完成，启动下一次检测*/
    void checkFinishedAll(char resultC);

     /*3D的一次检测完成，槽函数更新界面图像和实时检测数据*/
     void checkSuccessFinished(bool isSideLower); //true:侧面下部  flase:侧面上部


    /*启动3D检测，槽函数是3D检测线程的启动检测
    */
    void setup3DcheckSig(int objectIndex,int type);

    /*启动3D检测：段塞序号，是否是批量模式拍照*/
    void setup3DcheckSig(int objectIndex,bool isBatch);


    void stopCameraSig();

    void cameraStatesSig(int States);

public slots:
    /*
     * 初始化分配任务线程，开启串口，启动3D检测线程
   */
    void initTask();
/*
 * 测试使用，点击开始检测后：接收检测的位置（侧面下部/侧面上部， 有无端塞）
 * 调用接收到自动化数据函数：recvPLCSerialDataHandle()启动一次3D检测
*/

    /*该函数暂时改为设备启动后给执行给PLC发启动数据*/
     void checkTaskForTest(int checkPosition,bool isNonObjectForTest);
     void isStartCheckAndPLC(bool isStart);

     /*
      * 接收到自动化数据函数：recvPLCSerialDataHandle()启动一次3D检测
     */
    void recvPLCSerialDataHandle();

    /*
     * 接受来自单片机的串口数据：单片机主动发送段塞就位信息（通知软件启动拍照）
*/
    void recvSinglechipSerialDataHandle();


    /*
     * 接收2D传来的数据，当前只有2D检测完成信号
    */
    void recv2DSerialDataHandle();

    void sendStartHandle();

    /*检测结果发送到2D工控机*/
    void sendCheckResultTo2D(QByteArray resultData);


    /*不区分上部和下部，检测完成后*/
    void checkSuccessFinishedHandle(int result);


    void sendDataToPLC_getCheckCmd();

    /*从检测结果的队列中获取数据并发送到PLC*/
    void sendDataToPLC_CheckResult();



 private:
    quint8 checkData(char *data, int len);
    void open2DSerial(const QString com);
    void openPCLSerial(const QString com);
    void openSinglechipSerial(const QString com);
    /*发送检测结果到PLC*/
    void sendCheckResultToPLC(char resultC);

    /*
     * 根据3D工位的端塞序号和侧面下部或上部 计算出2D工位上端塞序号并发送给2D工控机。
   */
    void sendCheckMagTo2D_CheckStation_Serial(int current3D_ObjectIndex,ThreeDTasks::TaskType type);

    void sendDataToPLC_setupMyself();//启动一次检测

    void sendDataToPLC_OK_Type();
    void sendDataToPLC_Convex_Type();//缺肉 凹
    void sendDataToPLC_Concave_Type();//多肉 凸
    void sendDataToPLC_Cleanliness_Type();//清洁度

    void sendDataToPLC_Y1ToY4_Close();//关闭所有的灯（指示检测结果的灯 都关闭）

    void sendDataToPLC_closeMyself();  //关闭一次检测




    QSerialPort *serialPort_plc;  //与plc链接的串口
    QSerialPort *serialPort_2D;   //与2D工控机链接的串口

    QSerialPort *serialPort_singlechip; //与单片机链接的串口
    QByteArray recvSerialPort_SinglechipData;  //接收的单片机数据

    QByteArray recvSerialPort_2DData;
    bool m_isNonObjectForTest;
    int m_checkPositionForTest;



   // QByteArray recvDataByteArray;
     QByteArray recvSerialPort_PLCData;

     QTimer *timer;
};

#endif // TASKALLOCATION_H
