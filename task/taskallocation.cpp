#include "taskallocation.h"
#include <QDebug>
#include <QSerialPortInfo>
#include "camera3d.h"

#include <QThread>

#include <QSerialPortInfo>
#include <QByteArray>
#include "resultdata.h"

#include <QQueue>
#include <QMutexLocker>
#include <QMutex>

#include "emitobject.h"

QQueue<int> only3DResultQueue;
QMutex only3DResultQueueMutex;
extern EmitObject resultToPLCemitObject;

TaskAllocation::TaskAllocation(QObject *parent) : QObject(parent)
{

}

void TaskAllocation::initTask()
{
    serialPort_plc = new QSerialPort();
    connect(serialPort_plc,SIGNAL(readyRead()),this,SLOT(recvPLCSerialDataHandle()));

    serialPort_2D = new QSerialPort();
    connect(serialPort_2D,SIGNAL(readyRead()),this,SLOT(recv2DSerialDataHandle()));

    serialPort_singlechip = new QSerialPort();
    connect(serialPort_singlechip,SIGNAL(readyRead()),this,SLOT(recvSinglechipSerialDataHandle()));

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        qDebug()<<"Name："<<info.portName();
        qDebug()<<"Description:"<<info.description();
        qDebug()<<"Manufacturer:"<<info.manufacturer();
        qDebug()<<"SerialNumber:"<<info.serialNumber();
        qDebug()<<"System Location"<<info.systemLocation();

        if(info.portName() == "COM1")
        {
             open2DSerial("COM1"); //COM1是与2D工控机通信
        }else if(info.portName() == "COM2")
        {
            openPCLSerial("COM2"); //COM1是与2D工控机通信
        }else
        {
            openSinglechipSerial(info.portName());
       }
    }





    timer = new QTimer(this);
  //外触发:io板加了单片机，单片机主动发送拍照指令（IO板上的X0亮信号）
   // connect(timer,SIGNAL(timeout()),this,SLOT(sendDataToPLC_getCheckCmd()));

    ThreeDTasks  *threeDtask = new ThreeDTasks();

    QThread *thread = new QThread();

   // connect(this,SIGNAL(setup3DcheckSig(int,int)),threeDtask,SLOT(setupCheck(int,int)));
    connect(this,SIGNAL(setup3DcheckSig(int,bool)),threeDtask,SLOT(setupCheck(int,bool)));


    //connect(threeDtask,SIGNAL(checkSuccessFinished(bool)),this,SIGNAL(checkSuccessFinished(bool)));
     connect(threeDtask,SIGNAL(checkSuccessFinished(int)),this,SLOT(checkSuccessFinishedHandle(int)));

    connect(threeDtask,SIGNAL(checkSuccessFinishedResult(QByteArray)),this,SLOT(sendCheckResultTo2D(QByteArray)));
    connect(threeDtask,SIGNAL(cameraStatesSig(int)),this,SIGNAL(cameraStatesSig(int)));

    connect(&resultToPLCemitObject,SIGNAL(camearFinished()),this,SLOT(sendDataToPLC_CheckResult()));

    threeDtask->moveToThread(thread);
     connect(thread,SIGNAL(started()),threeDtask,SLOT(initThreeDTask()));

     connect(this,SIGNAL(stopCameraSig()),threeDtask,SLOT(stopCameraHandle()));



    thread->start();



}

void TaskAllocation::checkTaskForTest(int checkPosition, bool isNonObjectForTest)
{
    qDebug()<<"into TaskAllocation::checkTaskForTest";
      timer->start(500);
     sendDataToPLC_setupMyself();

     sendDataToPLC_Y1ToY4_Close();

}

void TaskAllocation::isStartCheckAndPLC(bool isStart)
{
    if(isStart)
    {
        qDebug()<<"void TaskAllocation::startCheckAndPLC()";
         // timer->start(500);
         sendDataToPLC_setupMyself();
         sendDataToPLC_Y1ToY4_Close();
    }else {
      sendDataToPLC_closeMyself();
   }

}

void TaskAllocation::recvPLCSerialDataHandle()
{
    if(serialPort_plc)
    {
         QByteArray info = serialPort_plc->readAll();

         if(info.length()  <= 0)
         {
             qDebug()<<"当前接收的2D工位的数据长度小于0，异常";
             return;
         }
         if(recvSerialPort_PLCData .size() > 1024)
         {
             qDebug()<<"recvSerialPort_PLCData  size too big morethan 1024 , clearing";
             recvSerialPort_PLCData.clear();
         }
         recvSerialPort_PLCData += info;
         qDebug()<<"recvPLCSerialDataHandle()："<< recvSerialPort_PLCData.toHex();
         qint16 pos;
        // uchar buf[]={0x01,0x02,0x03,0x04,0xff,0x00,0x1,0x02};
        // uchar x0_on[]={0x01, 0x05, 0x00, 0x00, 0xff, 0x00, 0x8c, 0x3a};
         uchar x0_on[]={0x01, 0x02, 0x02, 0x01, 0x00, 0xb8, 0x28};
         uchar *p = x0_on;
        // 01 02 02 01 00 B8 28
         QByteArray checkMsg((char*)p,7);
        qDebug()<<"checkMsg:"<<checkMsg.toHex();

          static int objectIndex = 0;
         while (!recvSerialPort_PLCData.isEmpty() && recvSerialPort_PLCData.size() >= checkMsg.size())
         {
             pos = recvSerialPort_PLCData.indexOf(checkMsg);//查找检测完成数据包
             if(pos >= 0)
             {
               //  emit check2DFinishedAll();//调试2D使用，启动下一检测
                 objectIndex++;
                 //  emit setup3DcheckSig(objectIndex,true); //使用单片机直接发送X0
                 qDebug()<<"timer->stop();";
                 timer->stop();


                 qDebug()<<"cut before："<< recvSerialPort_PLCData.toHex();
                 recvSerialPort_PLCData = recvSerialPort_PLCData.mid(checkMsg.size());
                 qDebug()<<"cut after："<< recvSerialPort_PLCData.toHex();

                 break;
             }else {
                break;
            }
         }

    }


#if 0
    quint8 recvTest1[]={0xbb,0x20,0x07,0x03,0x01,0x01,0x01,0x00,0x55};    //底部
    quint8 recvTest2[]={0xbb,0x20,0x07,0x03,0x01,0x02,0x01,0x00,0x55};    //顶部
    quint8 recvTest0[]={0xbb,0x20,0x07,0x03,0x01,0x01,0x01,0x00,0x55};   //无端塞
    //quint8 recvTest[]={0xbb,0x20,0x07,0x03,0x01,0x01,0x01,0x00,0x55};

    // QByteArray info = m_serialPort->readAll();
    QByteArray info;
    if(m_isNonObjectForTest)
    {
        info.clear();
        info = QByteArray((char *)recvTest0,sizeof (recvTest0));
    }
    else if(m_checkPositionForTest == 1)
    {
        //  QByteArray info((char *)recvTest1,sizeof (recvTest1));
        info.clear();
        info = QByteArray((char *)recvTest1,sizeof (recvTest1));
    }else if(m_checkPositionForTest > 1)
    {
        info.clear();
        info = QByteArray((char *)recvTest2,sizeof (recvTest2));
    }




    QByteArray hexData = info.toHex();
    //这里面的协议 你们自己定义就行  单片机发什么 代表什么 我们这里简单模拟一下
    qDebug()<<"SerialPortTask::receiveInfoHandle():";//<<QString(hexData);
    if(info.length()  <= 0)
    {
        qDebug()<<"当前接收的数据长度小于0，异常";
        return;
    }
    if(recvDataByteArray.size() > 1024)
    {
        qDebug()<<"recvDataByteArraysize too big morethan 1024 , clearing";
        recvDataByteArray.clear();
    }

    recvDataByteArray += info;
    qint16 pos;
    qint16 pos_end;
    quint16 msgID, dataLen;
    QByteArray msg;
    static int objectIndex = 1;
    while(!recvDataByteArray.isEmpty() && (recvDataByteArray.size() >= 6))
    {
        pos = recvDataByteArray.indexOf(QByteArray("\xBB"));
        if(pos >= 0)
        {
            qDebug()<<"发现包头";
            pos_end = recvDataByteArray.indexOf(QByteArray("\x55"),pos + 1);
        }else
        {
            //   qDebug()<<"未发现0x7E:"<<revdata.toHex()<<"清空接收缓存";
            qDebug()<<"未发现包头";
            // recvDataByteArray.clear();
            break;
        }

        if (pos_end >= 0)
        {
            msg = recvDataByteArray.mid(pos,pos_end - pos + 1);
            recvDataByteArray = recvDataByteArray.mid(pos_end + 1);
            qDebug()<<"截取到数据："<< msg.toHex();
        }else {
            qDebug()<<"未发现包尾";
            break;
        }

        char cmd = msg.at(3);
        if(cmd == 0x01)
        {
            qDebug()<<"自动化心跳，故障码:"<<(qint8)msg.at(4);
            break;
        }else if (cmd == 0x02)
        {
            qDebug()<<"自动化 开始检测应答:"<<(qint8)msg.at(4);
            break;
        }else if (cmd == 0x03 && (msg.at(2) == 7 ||msg.at(2) == 9||msg.at(2) == 11||msg.at(2) == 13))
        {


            // qDebug()<<"thingIndex:";
            static int current3D_ObjectIndex = 0;
            int headLen = 4;
            if(msg.at(headLen) == 0x01) //1号工位
            {
                if(msg.at(headLen + 1) == 0x01) //侧面下部
                {

                    if(msg.at(headLen + 1 + 1) > 0)  //有端塞
                    {
                        qDebug()<<"receiveInfoHandle() ## ";//准备 1号工位,侧面下部 检测";
                        current3D_ObjectIndex++;
                       // setupThreeDTask(camera3d_1,objectIndex,ThreeDTasks::SideLowerPart,ThreeDTasks::SideLowerAllType,ThreeDTasks::SideUpperAllType,0);
                        emit setup3DcheckSig(123,1);

                        sendCheckMagTo2D_CheckStation_Serial(current3D_ObjectIndex,ThreeDTasks::SideLowerPart);
                    }else {      //没有端塞
                        qDebug()<<"receiveInfoHandle()1#";//号工位,侧面下部 检测 无端塞";
                        sendCheckMagTo2D_CheckStation_Serial(0,ThreeDTasks::SideLowerPart);
                    }

                }else if(msg.at(headLen + 1) == 0x02)//侧面上部
                {
                    if(msg.at(headLen + 1 + 1) > 0)  //有端塞
                    {
                       // qDebug()<<"准备 1号工位,侧面上部 检测";
                       // setupThreeDTask(camera3d_1,objectIndex,ThreeDTasks::SideUpperPart,ThreeDTasks::SideLowerAllType,ThreeDTasks::SideUpperAllType,0);
                        emit setup3DcheckSig(123,2);
                        sendCheckMagTo2D_CheckStation_Serial(current3D_ObjectIndex,ThreeDTasks::SideUpperPart);
                    }else
                    {
                      //  qDebug()<<"receiveInfoHandle()1号工位,侧面上部 检测 无端塞";
                        sendCheckMagTo2D_CheckStation_Serial(0,ThreeDTasks::SideUpperPart);
                    }

                }else
                {
                    qDebug()<<"一号工位无效的部位检测";
                }
            }
            break;
        }else if (cmd == 0x05)
        {
            qDebug()<<"检测完成，结果："<<(qint8)msg.at(4);
            break;
        }

    }
#endif
}

void TaskAllocation::recvSinglechipSerialDataHandle()
{
    if(serialPort_singlechip)
    {
         QByteArray info = serialPort_singlechip->readAll();

         if(info.length()  <= 0)
         {
             qDebug()<<"当前接收的Singlechip工位的数据长度小于0，异常";
             return;
         }
         if(recvSerialPort_SinglechipData.size() > 0)
         {
             qDebug()<<"recvSerialPort_2DData  size too big morethan 1024 , clearing";
             recvSerialPort_SinglechipData.clear();
         }
         recvSerialPort_SinglechipData += info;
         qDebug()<<"recvSinglechipSerialDataHandle()"<< recvSerialPort_SinglechipData.toHex();
         qint16 pos;
         QByteArray checkMsg;
         checkMsg.append(0x11);   //开启一次检测，即打开X0灯

         QByteArray closeMsg;
         closeMsg.append(0xca);   //关闭所有检测结果的指示灯（Y1-Y4）



         static int objectIndex = 0;
         while (!recvSerialPort_SinglechipData.isEmpty() )
         {
             pos = recvSerialPort_SinglechipData.indexOf(checkMsg);//查找检测完成数据包
             if(pos >= 0 && pos <= recvSerialPort_SinglechipData.size())
             {
                 objectIndex++;
                 //  emit setup3DcheckSig(objectIndex,true);
                 //qDebug()<<"timer->stop();";
                 //timer->stop();


                 qDebug()<<"recvSerialPort_SinglechipData cut before："<< recvSerialPort_SinglechipData.toHex();
                 recvSerialPort_SinglechipData = recvSerialPort_SinglechipData.mid(checkMsg.size());
                 qDebug()<<"recvSerialPort_SinglechipData cut after："<<recvSerialPort_SinglechipData.toHex();


                 break;
             }else
             {
                  pos = recvSerialPort_SinglechipData.indexOf(closeMsg);//
                  if(pos >= 0 && pos <= recvSerialPort_SinglechipData.size())
                  {
                      sendDataToPLC_Y1ToY4_Close();
                     break;
                  }else
                  {
                    qDebug()<<"recvSinglechipSerialDataHandle() none useful data!";
                    break;
                  }

             }
         }

    }
}

void TaskAllocation::recv2DSerialDataHandle()
{
    if(serialPort_2D)
    {
         QByteArray info = serialPort_2D->readAll();

         if(info.length()  <= 0)
         {
             qDebug()<<"当前接收的2D工位的数据长度小于0，异常";
             return;
         }
         if(recvSerialPort_2DData .size() > 1024)
         {
             qDebug()<<"recvSerialPort_2DData  size too big morethan 1024 , clearing";
             recvSerialPort_2DData .clear();
         }
         recvSerialPort_2DData += info;
         qDebug()<<"recv2DSerialDataHandle()："<< recvSerialPort_2DData.toHex();
         qint16 pos;
         QByteArray msg;
         while (!recvSerialPort_2DData.isEmpty() && recvSerialPort_2DData.size() >= 5 + 2) //5: 是数据包的LEN   2:是包头和包尾
         {
             pos = recvSerialPort_2DData.indexOf(QByteArray(("\0xCC\0x60\0x05\0x04")));//查找检测完成数据包
             if(pos >= 0 && pos + 5 + 2 <= recvSerialPort_2DData.size())
             {
                 emit check2DFinishedAll();//调试2D使用，启动下一检测

                 emit checkFinishedAll(recvSerialPort_2DData.at(pos + 4));

                 sendCheckResultToPLC(recvSerialPort_2DData.at(pos + 4));
                 qDebug()<<"cut before："<< recvSerialPort_2DData.toHex()<<"result:"<<(quint8)recvSerialPort_2DData.at(pos + 4);
                 qDebug()<<"recvSerialPort_2DData msg"<<recvSerialPort_2DData.mid(pos,pos + 5 + 2).toHex();
                 recvSerialPort_2DData = recvSerialPort_2DData.mid(pos + 5 + 2);
                 qDebug()<<"cut after："<< recvSerialPort_2DData.toHex();

                 break;
             }
         }

    }

}

void TaskAllocation::sendStartHandle()
{
    char buf[512];
    buf[0] = 0xbb;//头
    buf[1] = 0x20;//硬件地址
    buf[2] = 0x00; //长度
    buf[3] = 0x02;//cmd
    buf[4] = 0x00;//check;
    buf[5] = 0x55; //尾

    buf[2] = 0x04; //长度
    char check = checkData(buf,4);
    check ^= buf[5];
    buf[4] = check;
    serialPort_2D->write(buf,6);
}

void TaskAllocation::sendCheckResultTo2D(QByteArray resultData)
{
    if(serialPort_2D && serialPort_2D->isOpen())
    {
        int len = serialPort_2D->write(resultData);
        qDebug()<<"串口已发送3D检测结果数据到2D len："<<len;
    }else {
        qDebug()<<"与2D工控机之间的串口不存在或者未打开";
    }
}

void TaskAllocation::checkSuccessFinishedHandle(int result)
{
    emit checkSuccessFinished(true);
    if((ResultType)result == OK_Type)
    {
        sendDataToPLC_OK_Type();
    }else if((ResultType)result == Convex_Type) {
       sendDataToPLC_Convex_Type();
    }else if((ResultType)result == Concave_Type){
       sendDataToPLC_Concave_Type();
    }else if((ResultType)result == Cleanliness_Type){
        sendDataToPLC_Cleanliness_Type();
    }
    qDebug()<<"checkSuccessFinishedHandle ";
   // timer->setInterval(50);
 //   timer->start();
}

void TaskAllocation::sendDataToPLC_getCheckCmd()
{
    qDebug()<<"into sendDataToPLC_getCheckCmd()";
    if(serialPort_plc && serialPort_plc->isOpen())
    {

     char buf1[]={0x01,0x02,0x00,0x00,0x00,0x0c,0x78,0x0f};

        int len1 = serialPort_plc->write(QByteArray(buf1,8));
        qDebug()<<"sendDataToPLC_getCheckCmd len1:"<<len1<<QByteArray(buf1,8).toHex();
      //  timer->setInterval(500);
     //   timer->start();

    }else {
        qDebug()<<"sendDataToPLC_getCheckCmd faile!";
    }
}

void TaskAllocation::sendDataToPLC_CheckResult()
{
    static int index = 0;
    index++;

    qDebug()<<"TaskAllocation::sendDataToPLC_CheckResult():index:"<<index;
    only3DResultQueueMutex.lock();
    qDebug()<<"only3DResultQueue.size():"<<only3DResultQueue.size();
    if(only3DResultQueue.size()>=3)
    {
         int result =   only3DResultQueue.dequeue();
         only3DResultQueueMutex.unlock();
         if(result == 0)
         {
             sendDataToPLC_OK_Type();
         }else if (result == 1)
         {
             sendDataToPLC_Convex_Type();
         }else if (result == 2)
         {
              sendDataToPLC_Concave_Type();
         }else if (result == 3)
         {
              sendDataToPLC_Cleanliness_Type();
         }else
         {
              qDebug()<<"TaskAllocation::sendDataToPLC_CheckResult() invalid result!!";
         }
    }else {
      only3DResultQueueMutex.unlock();
   }

}

quint8 TaskAllocation::checkData(char *data, int len)
{
    int i;
    quint8 result = 0;
    for(i = 0;i <len;i++)
    {
        result ^= data[i];
    }

    return  result;
}

void TaskAllocation::open2DSerial(const QString com)
{
    if(serialPort_2D)
    {
        serialPort_2D->setPortName(com);

        if(!serialPort_2D->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
        {
            qDebug()<<com<<"打开失败!";
            return;
        }

        serialPort_2D->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
        serialPort_2D->setDataBits(QSerialPort::Data8);		//数据位为8位
        serialPort_2D->setFlowControl(QSerialPort::NoFlowControl);//无流控制
        serialPort_2D->setParity(QSerialPort::NoParity);	//无校验位
        serialPort_2D->setStopBits(QSerialPort::OneStop); //一位停止位
        qDebug()<<"open2DSerial OPEN OK"<<"打开成功!"<<com;
    }
}

void TaskAllocation::openPCLSerial(const QString com)
{
    if(serialPort_plc)
    {
        serialPort_plc->setPortName(com);

        if(!serialPort_plc->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
        {
            qDebug()<<com<<"serialPort_plc OPEN FAILE 打开失败!";
            return;
        }

        serialPort_plc->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
        serialPort_plc->setDataBits(QSerialPort::Data8);		//数据位为8位
        serialPort_plc->setFlowControl(QSerialPort::NoFlowControl);//无流控制
        serialPort_plc->setParity(QSerialPort::NoParity);	//无校验位
        serialPort_plc->setStopBits(QSerialPort::OneStop); //一位停止位
        qDebug()<<"openPCLSerial OPEN OK"<<"打开成功!"<<com;
    }
}

void TaskAllocation::openSinglechipSerial(const QString com)
{
    if(serialPort_singlechip)
    {
        serialPort_singlechip->setPortName(com);

        if(!serialPort_singlechip->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
        {
            qDebug()<<com<<"serialPort_singlechip OPEN FAILE 打开失败!";
            return;
        }

        serialPort_plc->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
        serialPort_plc->setDataBits(QSerialPort::Data8);		//数据位为8位
        serialPort_plc->setFlowControl(QSerialPort::NoFlowControl);//无流控制
        serialPort_plc->setParity(QSerialPort::NoParity);	//无校验位
        serialPort_plc->setStopBits(QSerialPort::OneStop); //一位停止位
        qDebug()<<"serialPort_singlechip OPEN OK"<<"打开成功!"<<com;
    }
}

void TaskAllocation::sendCheckResultToPLC(char resultC)
{
    if(serialPort_2D && serialPort_2D->isOpen())
    {
       // QByteArray()
        int len = serialPort_2D->write(QByteArray(&resultC,1));
        qDebug()<<"串口已发送检测结果数据到PLC len:"<<len;
    }else {
        qDebug()<<"与PLC之间的串口不存在或者未打开";
    }
}

void TaskAllocation::sendCheckMagTo2D_CheckStation_Serial(int current3D_ObjectIndex, ThreeDTasks::TaskType type)
{
    static quint16 oneStationObjectIndex = 0;
    static quint16 twoStationObjectIndex = 0;
    static quint16 threeStationObjectIndex = 0;
    static quint16 fourStationObjectIndex = 0;

    //static quint32 num = 0;
    // num++;

    //if(num )
    static int isCurrent3D_ObjectIndex_Empty_Num = 0; //当current3D_ObjectIndex == 0时 表示 3D工位没有端塞了
    static int isCurrent3D_ObjectIndex_NonEmpty_Num = 0;

    if(current3D_ObjectIndex > 0 && type == ThreeDTasks::SideLowerPart)
    {
        fourStationObjectIndex = threeStationObjectIndex;
        threeStationObjectIndex = twoStationObjectIndex;
        twoStationObjectIndex = oneStationObjectIndex;
        oneStationObjectIndex = (quint16)current3D_ObjectIndex;

    }else if(current3D_ObjectIndex == 0 && fourStationObjectIndex > 0 && type == ThreeDTasks::SideLowerPart)
    {
        fourStationObjectIndex = threeStationObjectIndex;
        threeStationObjectIndex = twoStationObjectIndex;
        twoStationObjectIndex = oneStationObjectIndex;
        oneStationObjectIndex = (quint16)current3D_ObjectIndex;
    }else if(current3D_ObjectIndex == 0 && fourStationObjectIndex == 0 && type == ThreeDTasks::SideLowerPart){
        qDebug()<<"~~~~~~~~~~~~~~~~~~~~~检测完成";
        return;
    }else
    {
        qDebug()<<"sendCheckMagTo2D_CheckStation_Serial  无效的数据 不处理直接返回";
    }

    char buf[1024];
    char *p = buf;

    *p++= 0xCC;
    *p++= 0x60; //设备地址
    *p++= 0x00;//长度
    *p++= 0x02;//命令
    *p++= 0xFF &twoStationObjectIndex;
    *p++= 0xFF &(twoStationObjectIndex >> 8);
    *p++= 0xFF &threeStationObjectIndex;
    *p++= 0xFF &(threeStationObjectIndex >> 8);
    *p++= 0xFF &fourStationObjectIndex;
    *p++= 0xFF &(fourStationObjectIndex >> 8);

    char check = checkData(buf,p - buf);

    *p++= check;
    *p++= 0x66;

    buf[2] = p - buf -2 ;

    static int index = 0;
    index++;
    // sendMagTo2D(QByteArray(buf,p - buf));
    if(serialPort_2D && serialPort_2D->isOpen())
    {
        int len = serialPort_2D->write(QByteArray(buf,p - buf));
        qDebug()<<"串口已发送数据到2D"<<len<<"index:"<<index;
        qDebug()<<"send to 2D check task:"<<QByteArray(buf,p - buf).toHex();
    }else {
        qDebug()<<"与2D工控机之间的串口不存在或者未打开";
    }


}

void TaskAllocation::sendDataToPLC_setupMyself()
{
    qDebug()<<"into sendDataToPLC_setupMyself()";
    if(serialPort_plc && serialPort_plc->isOpen())
    {

     uchar buf[]={0x01,0x05,0x00,0x00,0xff,0x00,0x8c,0x3a};

        int len = serialPort_plc->write(QByteArray((char*)buf,8));
        qDebug()<<"sendDataToPLC_setupMyself() len:"<<len;
       // timer->start(50);
    }else {
        qDebug()<<"sendDataToPLC_setupMyself() faile";
    }
}

void TaskAllocation::sendDataToPLC_OK_Type()
{
    if(serialPort_plc && serialPort_plc->isOpen())
    {
       QByteArray data;

     uchar buf1[]={0x01,0x05,0x00,0x01,0xff,0x00,0xdd,0xfa};
     uchar buf2[]={0x01,0x05,0x00,0x01,0x00,0x00,0x9c,0x0a};


        int len1 = serialPort_plc->write(QByteArray((char*)buf1,8));
        qDebug()<<"sendDataToPLC_OK_Type PLC len1:"<<len1;
        serialPort_plc->flush();
        QThread::sleep(2);
        int len2 = serialPort_plc->write(QByteArray((char*)buf2,8));
        serialPort_plc->flush();
        qDebug()<<"sendDataToPLC_OK_Type PLC len2:"<<len2;
    }else {
        qDebug()<<"sendDataToPLC_OK_Type faile";
    }
}
/*缺肉*/
void TaskAllocation::sendDataToPLC_Convex_Type()
{
    if(serialPort_plc && serialPort_plc->isOpen())
    {
       QByteArray data;

     uchar buf1[8]={0x01,0x05,0x00,0x02,0xff,0x00,0x2d,0xfa};//Y2亮
     uchar buf2[8]={0x01,0x05,0x00,0x02,0x00,0x00,0x6c,0x0a};


        int len1 = serialPort_plc->write(QByteArray((char*)buf1,8));
        qDebug()<<"sendDataToPLC_Convex_Type PLC len1:"<<len1<<"DATA:"<<QByteArray((char*)buf1,8);
        serialPort_plc->flush();
        QThread::sleep(2);
        int len2 = serialPort_plc->write(QByteArray((char*)buf2,8));
        qDebug()<<"sendDataToPLC_Convex_Type PLC len2:"<<len2<<"DATA:"<<QByteArray((char*)buf2,8);
        serialPort_plc->flush();
    }else {
        qDebug()<<"sendDataToPLC_Convex_Type faile";
    }
}
/*多肉*/
void TaskAllocation::sendDataToPLC_Concave_Type()
{
    if(serialPort_plc && serialPort_plc->isOpen())
    {
       QByteArray data;

     uchar buf1[]={0x01,0x05,0x00,0x03,0xff,0x00,0x7c,0x3a};
     uchar buf2[]={0x01,0x05,0x00,0x03,0x00,0x00,0x3d,0xca};


        int len1 = serialPort_plc->write(QByteArray((char*)buf1,8));
        qDebug()<<"sendDataToPLC_Concave_Type PLC len1:"<<len1<<"DATA:"<<QByteArray((char*)buf1,8);
        serialPort_plc->flush();
        QThread::sleep(2);
        int len2 = serialPort_plc->write(QByteArray((char*)buf2,8));
        qDebug()<<"sendDataToPLC_Concave_Type PLC len2:"<<len2<<"DATA:"<<QByteArray((char*)buf2,8);
        serialPort_plc->flush();
    }else {
        qDebug()<<"sendDataToPLC_Concave_Type faile";
    }
}

void TaskAllocation::sendDataToPLC_Cleanliness_Type()
{
    if(serialPort_plc && serialPort_plc->isOpen())
    {
       QByteArray data;
     uchar buf1[]={0x01,0x05,0x00,0x04,0xff,0x00,0xcd,0xfb};
     uchar buf2[]={0x01,0x05,0x00,0x04,0x00,0x00,0x8c,0x0b};




        int len1 = serialPort_plc->write(QByteArray((char*)buf1,8));
        qDebug()<<"sendDataToPLC_Cleanliness_Type PLC len1:"<<len1<<"DATA:"<<QByteArray((char*)buf1,8);
        serialPort_plc->flush();
         QThread::sleep(2);
        int len2 = serialPort_plc->write(QByteArray((char*)buf2,8));
        qDebug()<<"sendDataToPLC_Cleanliness_Type PLC len2:"<<len2<<"DATA:"<<QByteArray((char*)buf2,8);
        serialPort_plc->flush();
    }else {
        qDebug()<<"sendDataToPLC_Cleanliness_Type faile";
    }
}



void TaskAllocation::sendDataToPLC_Y1ToY4_Close()
{
    if(serialPort_plc && serialPort_plc->isOpen())
    {

     uchar buf1[]={0x01 ,0x05 ,0x00 ,0x01 ,0x00 ,0x00 ,0x9c ,0x0a}; //Y1灭
     uchar buf2[]={0x01 ,0x05 ,0x00 ,0x02 ,0x00 ,0x00 ,0x6c ,0x0a }; //Y2灭
     uchar buf3[]={0x01 ,0x05 ,0x00 ,0x03 ,0x00 ,0x00 ,0x3d ,0xca }; //Y3灭
     uchar buf4[]={0x01 ,0x05 ,0x00 ,0x04 ,0x00 ,0x00 ,0x8c ,0x0b }; //Y4灭



        int len1 = serialPort_plc->write(QByteArray((char*)buf1,8));
        qDebug()<<"sendDataToPLC_Y1ToY4_Close PLC len1:"<<len1;

        int len2 = serialPort_plc->write(QByteArray((char*)buf2,8));
        qDebug()<<"sendDataToPLC_Y1ToY4_Close PLC len2:"<<len2;

        int len3 = serialPort_plc->write(QByteArray((char*)buf3,8));
        qDebug()<<"sendDataToPLC_Y1ToY4_Close PLC len3:"<<len3;

        int len4 = serialPort_plc->write(QByteArray((char*)buf4,8));
        qDebug()<<"sendDataToPLC_Y1ToY4_Close PLC len4:"<<len4;
        serialPort_plc->flush();
    }else {
        qDebug()<<"sendDataToPLC_Y1ToY4_Close PLC faile";
    }
}

void TaskAllocation::sendDataToPLC_closeMyself()
{
    if(serialPort_plc && serialPort_plc->isOpen())
    {

      sendDataToPLC_Y1ToY4_Close();

     uchar buf1[]={0x01 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0xcd ,0xca}; //Y0灭

        int len1 = serialPort_plc->write(QByteArray((char*)buf1,8));
        qDebug()<<"sendDataToPLC_closeMyself PLC len1:"<<len1;

    }else {
        qDebug()<<"sendDataToPLC_closeMyself PLC faile";
    }
}
