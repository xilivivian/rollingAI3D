#include "threedtasks.h"
#include <QThread>
#include <QDebug>
//#include "Python.h"
#include <iostream>

#include "resultdata.h"
#include <QQueue>
#include <QMap>

#include <QDir>

#include <QString>

#include "resultdata.h"



//#include "sqliteBox/sqlbox.h"


#include "algorithmCC/main_recognise_for_point_cloud_2D.h"
#include "algorithmCC/main_recognise_for_point_cloud_3D.h"
#include "algorithmCC/point_cloud_recognise_function.h"

#include "algorithmCC/main_recognise_for_point_cloud.h"

#include<iostream>
#include <fstream>
#include <sstream>

#include <QTime>
#include "emitobject.h"
#include "API/PeakStructure.h"

#include <QMutex>

using namespace std;
#if 1
extern float* zValuesCopy_Batch ;/*new float[lineLength * batchLength]*/
extern float* intensityValuesCopy_Batch ;/* new float[lineLength * batchLength]*/
extern HEADER *headersCopy_Batch ;
#endif
extern int image_width_Batch;
extern int image_height_Batch;
extern double xStep_Batch;
extern double yStep_Batch;



extern EmitObject emitObject;
extern QMutex only3DResultQueueMutex;
extern QQueue<int> only3DResultQueue;



void read_csv(string filename, Mat& img_2D, Mat& img_3D, int x_rate, int y_rate);

void read_csv(string filename, Mat& img_2D, Mat& img_3D, int x_rate, int y_rate)
{
    ifstream inFile(filename, ios::in);
    string lineStr;
    vector<vector<double>> strArray;

    double max_x = -10000000.0, max_y = -10000000.0;

    double min_x = 10000000.0, min_y = 10000000.0;

    while (getline(inFile, lineStr))
    {
        // ��ӡ�����ַ���
        //cout << lineStr << endl;
        // ��ɶ�ά��ṹ
        stringstream ss(lineStr);
        string str;
        vector<double> lineArray;
        // ����' '�ָ�

        int i = 0;
        while (getline(ss, str, ' '))
        {
            double data = stof(str);
            if (i == 0)
            {
                data *= 1000000.0;
                max_x = max(max_x, data);
                min_x = min(min_x, data);
            }
            else if (i == 1)
            {
                data *= 1000000.0;
                max_y = max(max_y, data);
                min_y = min(min_y, data);
            }
            lineArray.push_back(data);
            i += 1;
        }
        strArray.push_back(lineArray);
    }

    int strArray_length = strArray.size();

    int image_col = int((max_y - min_y) / y_rate) + 1;
    int image_row = int((max_x - min_x) / x_rate) + 1;

    img_3D = Mat::ones(Size(image_row, image_col), CV_64FC1);

    img_3D *= -2;

    img_2D = Mat::zeros(Size(image_row, image_col), CV_8UC1);

    for (int i = 0; i < strArray_length; i++)
    {

        int x = (strArray[i][0] - min_x) / x_rate;
        int y = (strArray[i][1] - min_y) / y_rate;

        img_3D.at<double>(y, x) = strArray[i][2];

        img_2D.at<uchar>(y, x) = int(strArray[i][3]);
    }
}

/// <summary>
/// flat
/// ���� 0  3D�ϰ���
/// ���� 1  3D�°���
/// </summary>
/// <param name="img_3D"></param>
/// <param name="img_2D"></param>
/// <param name="flat"></param>
void main_recognise_for_point_cloud(Mat& img_3D, Mat& img_2D, recogn_OUT_for_point_cloud& out, int flat)
{

    main_recognise_for_point_cloud_3D(img_3D, img_2D, out, flat);
  //  main_recognise_for_point_cloud_2D(img_2D, out, flat);
}




static const char * checkPosition[CHECK_3D_ITEM_COUNT] ={"A4","A3","A3","A13","A13","A18","A21","A21"};


extern QQueue<RESULT> resultDataQueue;     //3D

extern QQueue<cv::Mat> sideLower2D_MatQueue;
extern QQueue<cv::Mat> sideLower3D_MatQueue;
extern QQueue<cv::Mat> sideUpper2D_MatQueue;
extern QQueue<cv::Mat> sideUpper3D_MatQueue;

QMap<int,bool>resultBoolDataMap; //�洢3D��2�μ�����ֱ����ϲ����²� int �� 1���ϲ�  2���²����� bool:true �ϸ� false ���ϸ�

static char resultUpDataChar[CHECK_3D_ITEM_COUNT]={0};//��¼�ϲ�ÿ�����Ƿ��Ѿ���⣬��ʼ��Ϊ0��1��0k,2:ȱ�⣬3�����⣬4�����ȣ�    �洢��0-6
static char resultDownDataChar[CHECK_3D_ITEM_COUNT]={0};//��¼�²�ÿ�����Ƿ��Ѿ���⣬��ʼ��Ϊ0��1��0k,2:ȱ�⣬3�����⣬4�����ȣ�  �洢��1-7

ThreeDTasks::ThreeDTasks(QObject *parent) : QObject(parent),m_taskType(SideAllPart),m_sideLowerType(SideLowerTask1),m_sideUpperType(SideUpperTask1)
{

}

void ThreeDTasks::setStationNum(int numGE)
{
    m_numGW = numGE;
}

void ThreeDTasks::setTask(Camera3D *camera3D, int objectIndex, ThreeDTasks::TaskType taskType, ThreeDTasks::SideLowerType sideLowerType, ThreeDTasks::SideUpperType sideUpperType, int layer)
{
    m_camera3D = camera3D;
    m_objectIndex = objectIndex;
    m_taskType = taskType;
    m_sideLowerType = sideLowerType;
    m_sideUpperType = sideUpperType;
    m_layer = layer;
}

void ThreeDTasks::setCamera3d(Camera3D *camera3D)
{
    this->m_camera3D = camera3D;
}

void ThreeDTasks::initThreeDTask()
{
    qDebug()<<"do  ThreeDTasks::initThreeDTask()";
    Camera3D *camera3D = new Camera3D;
    setCamera3d(camera3D);
    if(camera3D->initCamera3D() == 0)
    {
        qDebug()<<"~~~~~~~~~~~~~~do  ThreeDTasks::initThreeDTask()    ok";
        emit cameraStatesSig(0);
    }else
        {
        emit cameraStatesSig(1);
        qDebug()<<"~~~~~~~~~~~~~~do  ThreeDTasks::initThreeDTask()   faile";
    }

    connect(&emitObject,SIGNAL(camearFinished()),this,SLOT(runAlgorithm()));

}



void ThreeDTasks::setTask(ThreeDTasks::TaskType taskType)
{
    m_taskType = taskType;
}

void ThreeDTasks::setSideLowerType(ThreeDTasks::SideLowerType sideLowerType)
{
    m_sideLowerType = sideLowerType;
}

void ThreeDTasks::setSideUpperType(ThreeDTasks::SideUpperType sideUpperType)
{
    m_sideUpperType = sideUpperType;
}

void ThreeDTasks::setupCheck(int objectIndex, int taskType)
{
    qDebug()<<"ThreeDTasks::setupCheck(int objectIndex, int taskType) objectIndex,"
           <<objectIndex<<"taskType"<<taskType<<"QThread::currentThreadId()"<<QThread::currentThreadId();
    //setup  3D camera
    //cheching
    if((ThreeDTasks::TaskType)taskType == ThreeDTasks::SideUpperPart)
    {
        sideUpperTask_handle();
#if 0
        if(sideUpperTask_handle())
        {
            Mat img_3D, img_2D;
            int x_rate = 5506;
            int y_rate = 33333;
            QString filename("E:/XX_PRO/3D/csvImage/O-0.csv");
            //clock_t start = clock();
            qDebug()<<"SideUpperPart start recv csv";
            read_csv(filename.toStdString(), img_2D, img_3D, x_rate, y_rate);
            qDebug()<<"SideUpperPart stop recv csv";
            //clock_t end = clock();
            // printf("%d s\n", (end - start) / CLOCKS_PER_SEC);
            QTime time;
            time.start();
            vector<int> max_compare;
            vector<Mat> img_res;
            vector<flawimformation>flaw_total;

            recogn_OUT_for_point_cloud out(max_compare, img_res, flaw_total);

            main_recognise_for_point_cloud(img_3D, img_2D, out, 0);
             qDebug()<<"SideUpperPart main_recognise_for_point_cloud"<<time.elapsed()/1000.0<<"s";

            if(out.flaw_total.size() > 0)
            {

                RESULT result;
                result.objectNumber = m_objectIndex;
                result.StationNumber = 1/*m_numGW*/;
                result.resultType = (ResultType)out.flaw_total.at(0).type;
                result.positionInt = out.flaw_total.at(0).position;                       //��¼���εļ��������� Ŀ�ģ�����Ϳͻ�����������Ӧ
                result.Area = out.flaw_total.at(0).area;
                result.Deep = out.flaw_total.at(0).depth;
                result.Length = out.flaw_total.at(0).length;
                resultDataQueue.push_back(result);
            }

            if(sideUpper3D_MatQueue.size() <= QUEUE_SHOW_MAX_SIZE)
            {
                if(out.image.size() > 0)
                {
                    sideUpper3D_MatQueue.push_back(out.image.at(0)); //3Dͼ��
                }
            }

            if(sideUpper2D_MatQueue.size() <= QUEUE_SHOW_MAX_SIZE)
                if(out.image.size() > 1)
                {
                    sideUpper2D_MatQueue.push_back(out.image.at(1)); //2Dͼ��
                }

            emit checkSuccessFinished(false);


        }
#endif
      simulationMackCheckResult((quint8)ThreeDTasks::SideUpperPart); //1:�����²���2�����ϲ�

    }else if((ThreeDTasks::TaskType)taskType == ThreeDTasks::SideLowerPart)
    {
        sideLowerTask_handle();
#if 0
        if(sideLowerTask_handle())
        {
            Mat img_3D, img_2D;
            int x_rate = 5506;
            int y_rate = 33333;
            QString filename("E:/XX_PRO/3D/csvImage/O-0.csv");
            //clock_t start = clock();
            qDebug()<<"SideLowerPart start recv csv";
            read_csv(filename.toStdString(), img_2D, img_3D, x_rate, y_rate);
            //clock_t end = clock();
            // printf("%d s\n", (end - start) / CLOCKS_PER_SEC);
            qDebug()<<"SideLowerPart stop recv csv";
            QTime time;
            time.start();
            vector<int> max_compare;
            vector<Mat> img_res;
            vector<flawimformation>flaw_total;

            recogn_OUT_for_point_cloud out(max_compare, img_res, flaw_total);

            main_recognise_for_point_cloud(img_3D, img_2D, out, 1);
            qDebug()<<"SideLowerPart main_recognise_for_point_cloud"<<time.elapsed()/1000.0<<"s";

            if(out.flaw_total.size() > 0)
            {

                RESULT result;
                result.objectNumber = m_objectIndex;
                result.StationNumber = 1/*m_numGW*/;
                result.resultType = (ResultType)out.flaw_total.at(0).type;
                result.positionInt = out.flaw_total.at(0).position;                       //��¼���εļ��������� Ŀ�ģ�����Ϳͻ�����������Ӧ
                result.Area = out.flaw_total.at(0).area;
                result.Deep = out.flaw_total.at(0).depth;
                result.Length = out.flaw_total.at(0).length;
                resultDataQueue.push_back(result);

            }

            if(sideLower3D_MatQueue.size() <= QUEUE_SHOW_MAX_SIZE)
            {
                if(out.image.size() > 0)
                {
                    sideLower3D_MatQueue.push_back(out.image.at(0)); //3Dͼ��
                    qDebug()<<"sideLower3D_MatQueue.push_back";
                }
            }

            if(sideLower2D_MatQueue.size() <= QUEUE_SHOW_MAX_SIZE)
            {
                if(out.image.size() > 1)
                {
                    sideLower2D_MatQueue.push_back(out.image.at(1)); //2Dͼ��
                    qDebug()<<"sideLower2D_MatQueue.push_back";
                }
            }
            emit checkSuccessFinished(true);
        }

#endif
        simulationMackCheckResult((quint8)ThreeDTasks::SideLowerPart); //1:�����²���2�����ϲ�
    }

}

void ThreeDTasks::setupCheck(int objectIndex, bool isBatch)
{
    m_objectIndex = objectIndex;
    setupCamera(isBatch);
}



void ThreeDTasks::setupCamera(bool isBatch)
{
    if(isBatch)
    {
        m_camera3D->focalSTART_BATCH();
    }else
    {
        m_camera3D->focalSTART_LINE();
    }
}

void ThreeDTasks::stopCameraHandle()
{
     m_camera3D->stopCamera3D();
     qDebug()<<"m_camera3D->stopCamera3D();�����ֹͣ";
}

void ThreeDTasks::runAlgorithm()
{
    #if 0
    Mat img_3D, img_2D;
    int x_rate = 5506;
    int y_rate = 33333;
    QString filename("E:/XX_PRO/3D/csvImage/O-0.csv");
    //clock_t start = clock();
    qDebug()<<"SideUpperPart start recv csv";
    read_csv(filename.toStdString(), img_2D, img_3D, x_rate, y_rate);
    qDebug()<<"SideUpperPart stop recv csv";
    //clock_t end = clock();
    // printf("%d s\n", (end - start) / CLOCKS_PER_SEC);
    QTime time;
    time.start();
    #endif
    vector<int> max_compare;
    vector<Mat> img_res;
    vector<flawimformation>flaw_total;

    recogn_OUT_for_point_cloud out(max_compare, img_res, flaw_total);

  //  main_recognise_for_point_cloud(img_3D, img_2D, out, 0);

    main_recognise_for_point_cloud(zValuesCopy_Batch, intensityValuesCopy_Batch, image_width_Batch, image_height_Batch, xStep_Batch,  yStep_Batch,  out, 0);
   //  qDebug()<<"SideUpperPart main_recognise_for_point_cloud"<<time.elapsed()/1000.0<<"s";
    // QThread::sleep(5);
     qDebug()<<"runAlgorithm()  main_recognise_for_point_cloud ";
    // int checkResult = 0;
#if 1
    if(out.flaw_total.size() > 0)
    {

        RESULT result;
        result.objectNumber = m_objectIndex;
        result.StationNumber = 1/*m_numGW*/;
        result.resultType = (ResultType)out.flaw_total.at(0).type;
        result.positionInt = out.flaw_total.at(0).position;                       //��¼���εļ��������� Ŀ�ģ�����Ϳͻ�����������Ӧ
        result.Area = out.flaw_total.at(0).area;
        result.Deep = out.flaw_total.at(0).depth;
        result.Length = out.flaw_total.at(0).length;
        resultDataQueue.push_back(result);
        qDebug()<<"out.flaw_total.at(0).type"<<out.flaw_total.at(0).type
               <<out.flaw_total.at(0).position
              <<out.flaw_total.at(0).area
             <<out.flaw_total.at(0).depth
            <<out.flaw_total.at(0).length;
       // checkResult = out.flaw_total.at(0).type;
       only3DResultQueueMutex.lock();
       only3DResultQueue.push_back(out.flaw_total.at(0).type);
       only3DResultQueueMutex.unlock();

    }else
    {

        RESULT result;
        result.objectNumber = m_objectIndex;
        result.StationNumber = 1/*m_numGW*/;
        result.resultType = ResultType::OK_Type;
        result.positionInt = 0;                       //��¼���εļ��������� Ŀ�ģ�����Ϳͻ�����������Ӧ
        result.Area = 0;
        result.Deep = 0;
        result.Length = 0;
        resultDataQueue.push_back(result);
       // checkResult = out.flaw_total.at(0).type;
       only3DResultQueueMutex.lock();
       only3DResultQueue.push_back(0);
       only3DResultQueueMutex.unlock();
    }
#endif
    if(sideLower3D_MatQueue.size() <= QUEUE_SHOW_MAX_SIZE)
    {
        if(out.image.size() > 0)
        {          
            sideLower3D_MatQueue.push_back(out.image.at(0)); //3Dͼ��
        }
    }

    if(sideLower2D_MatQueue.size() <= QUEUE_SHOW_MAX_SIZE)
        if(out.image.size() > 1)
        {
            sideLower2D_MatQueue.push_back(out.image.at(1)); //2Dͼ��
           // QDateTime current_date_time =QDateTime::currentDateTime();
           // QString current_date =current_date_time.toString("yyyy-MM-dd-hh-mm-ss")+".bmp";
          //  qDebug()<<"out.image.at(0).cols:"<<out.image.at(0).cols<<"out.image.at(0).rows:"<<out.image.at(0).rows;
          //  cv::imwrite(current_date.toStdString(),out.image.at(0));
        }

static int checkResult = 0;

    emit checkSuccessFinished(checkResult);
#if 1
  checkResult++;
  if(checkResult == 4)
      checkResult = 0;
#endif

     delete zValuesCopy_Batch ;/*new float[lineLength * batchLength]*/
     delete intensityValuesCopy_Batch ;/* new float[lineLength * batchLength]*/
    delete headersCopy_Batch ;
//QThread::sleep(2);

}

void ThreeDTasks::simulationMackCheckResult(const quint8 sidePositionD)
{
    quint8 sidePosition = sidePositionD; // �����ϲ�:2  �����²�:1
    quint8 resultNum = 3;  //�������
    quint8 position = 1;   //���ϸ�λ��
    quint8 type = 1;     //ȱ������
    float  len = 1.234; //����
    float  deep = 1.145;
    float  area = 1.443;
    int lenD,deepD,areaD;

    QByteArray resultBy;
    resultBy.append((char)(sidePosition));
    resultBy.append((char)(resultNum));
    for (int i = 0;i < resultNum;i++) {
        if(i == 1)
        {
            position = 2;   //���ϸ�λ��
            type = 2;     //ȱ������
            len = 2.234; //����
            deep = 2.145;
            area = 2.443;
        }else if(i == 2)
        {
            position = 3;   //���ϸ�λ��
            type = 2;     //ȱ������
            len = 3.234; //����
            deep = 3.145;
            area = 3.443;
        }
         resultBy.append((char)(position));
         resultBy.append((char)(type));
         lenD = (int)(len * 1000);
         deepD =  (int)(deep * 1000);
         areaD =  (int)(area * 1000);
         resultBy.append(intToByteArray(lenD));
         resultBy.append(intToByteArray(deepD));
         resultBy.append(intToByteArray(areaD));
    }

    uchar data0 = 0xCC;
    uchar data1 = 0x60; //Ӳ����ַ
    uchar data2 = 4 + resultBy.size(); //������ʼ��ĩ�ַ����ַ�������,����4+���ݳ���,����ݳ���<=240
    uchar data3 = 0x05; //���3D���ͼ����
    QByteArray sendData;
    sendData.append((char)(data0));
    sendData.append((char)(data1));
    sendData.append((char)(data2));
    sendData.append((char)(data3));
    sendData.append(resultBy);

   quint8 checkD = checkData(sendData);
   sendData.append((char)(checkD));
   uchar data4 = 0x66;
   sendData.append((char)(data4));



     qDebug()<<"ThreeDTasks::simulationMackCheckResult:"<<sendData.toHex();
    emit checkSuccessFinishedResult(sendData);
}

void ThreeDTasks::doTask()
{
    if(m_taskType == TaskType::SideLowerPart)
    {
        emit setupCameraFinished(1, m_camera3D->setupCamera3D());
        if(m_sideLowerType == SideLowerAllType)
        {
            char state =  sideLowerTask_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }
        else if(m_sideLowerType == SideLowerTask1)
        {
            char state =  sideLowerTask1_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();

        }else if (m_sideLowerType == SideLowerTask2)
        {

            char state =  sideLowerTask2_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else if (m_sideLowerType == SideLowerTask3)
        {
            char state =  sideLowerTask3_handle();
            //emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else if (m_sideLowerType == SideLowerTask4)
        {
            char state =  sideLowerTask4_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else
        {
            qDebug()<<"3D����ײ����Էֲ�ʧ�ܣ�"<<m_sideLowerType;
        }
    }else if(m_taskType == TaskType::SideUpperPart)
    {
        emit setupCameraFinished(1,m_camera3D->setupCamera3D());
        if(m_sideUpperType == SideUpperAllType)
        {
            char state =  sideUpperTask_handle();
            //  emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }
        else if(m_sideUpperType == SideUpperTask1)
        {
            char state =  sideUpperTask1_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else if (m_sideUpperType == SideUpperTask2)
        {
            char state =  sideUpperTask2_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else if (m_sideUpperType == SideUpperTask3)
        {
            char state =   sideUpperTask3_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else if (m_sideUpperType == SideUpperTask4)
        {

            char state =  sideUpperTask4_handle();
            // emit taskFinished((int)m_sideLowerType,state);
            emit taskFinished();
        }else
        {
            qDebug()<<"3D�����ϲ����Էֲ�ʧ�ܣ�"<<m_sideUpperType;
        }
    }else
    {
        qDebug()<<"3D����ֲ����԰ܣ�"<<m_taskType;
    }
}

bool ThreeDTasks::sideLowerTask_handle()
{
    //ִ��3D�������
    qDebug()<<"bool ThreeDTasks::sideLowerTask_handle() 3D����Ĳ����²�";
    return true;
}

char ThreeDTasks::sideLowerTask1_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideLowerTask1_handle()";
    return 0;
}

char ThreeDTasks::sideLowerTask2_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideLowerTask2_handle()";
    return 0;
}

char ThreeDTasks::sideLowerTask3_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideLowerTask3_handle()";
    return 0;
}

char ThreeDTasks::sideLowerTask4_handle()
{


    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideLowerTask4_handle()";
    return 0;
}

bool ThreeDTasks::sideUpperTask_handle()
{
    //ִ��3D�������
    m_camera3D->focalSTART_BATCH();
    qDebug()<<"bool ThreeDTasks::sideUpperTask_handle() m_camera3D->focalSTART_BATCH();  3D����Ĳ����ϲ�";
    return true;
}

char ThreeDTasks::sideUpperTask1_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideUpperTask1_handle()";
    return 0;
}

char ThreeDTasks::sideUpperTask2_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideUpperTask2_handle()";
    return 0;
}

char ThreeDTasks::sideUpperTask3_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideUpperTask3_handle()";
    return 0;
}

char ThreeDTasks::sideUpperTask4_handle()
{
    QThread::sleep(2);
    qDebug()<<"void ThreeDTasks::sideUpperTask4_handle()";
    return 0;
}
/*
�÷�����2Dͼ���滮������
mat2D: ����ԭʼ��2Dͼ��
����ֵ��ͼ�������ĸ�����������ķֽ���

*/

QVector<int> ThreeDTasks::getBodyLayers(cv::Mat &mat2D)
{
    QVector<int> vector;
    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);
    vector.push_back(5);
    vector.push_back(6);
    vector.push_back(7);
    vector.push_back(8);

    return  vector;
}




/**
  �÷�����ȡ3Dͼ��ָ�������ȱ��
  mat3D �������3Dԭʼͼ��
  result�����Ľ����
  threshold�����������ֵ
  border���߽�Ŀ��
  roi_min:��������Ķ���
  roi_max����������ĵײ�
  ����ֵ��ȱ�ݱ�Ǻ��ͼ��
  */
cv::Mat ThreeDTasks::checkImage(cv::Mat &mat3D, QVector<RESULT> &result, int threshold, int border, int roi_min, int roi_max)
{

    RESULT result1;
    result1.StationNumber = 1;

    result.push_back(result1);

    cv::Mat mat = cv::imread(QString("D:\\image\\new.png").toStdString());
    // cv::Mat M(2,2, CV_8UC3, cv::Scalar(0,0,255));
    // resultMat3D.create(2,2,CV_8SC3);
    //  M.copyTo(resultMat3D);
    qDebug()<<"ThreeDTasks::checkImage"<<"roi_min:"<<roi_min<<"roi_max:"<<roi_max;
    return mat;
}

QVariant ThreeDTasks::readkey(QString filemae, QString qstrnodename, QString qstrkeyname)
{
    QVariant qvar = QSettings(filemae,QSettings::IniFormat).value(QString("%1/%2").arg(qstrnodename).arg(qstrkeyname));
    return qvar;
}

void ThreeDTasks::makeCheckData(QVector<RESULT> &result, bool isDown, int objectNumber)
{
    //��д�������
    if(isDown)
    {
        for(int i = 0;i<result.size();i++)
        {
            result[i].StationNumber = m_numGW;
            ResultType type;
            type = (ResultType)readkey(QDir::currentPath() +"/checkitemopetion.ini","DOWN",QString("resultType%1").arg(i)).toInt();
            qDebug()<<QString("�²�%1���������:").arg(i)<<type<<QDir::currentPath() +"/checkitemopetion.ini";
            result[i].positionInt = readkey(QDir::currentPath() +"/checkitemopetion.ini","DOWN",QString("positionInt%1").arg(i)).toInt();
            result[i].resultType = type;
            result[i].objectNumber = objectNumber;
            if(type == Convex_Type)
            {
                result[i].Deep = readkey(QDir::currentPath() +"/checkitemopetion.ini","DOWN",QString("Deep%1").arg(i)).toFloat();
                result[i].Length = readkey(QDir::currentPath() +"/checkitemopetion.ini","DOWN",QString("Length%1").arg(i)).toFloat();
                result[i].Area =  readkey(QDir::currentPath() +"/checkitemopetion.ini","DOWN",QString("Area%1").arg(i)).toFloat();
                qDebug()<<QString("�²�%1���������:").arg(i)<<result[i].Deep <<result[i].Length<<result[i].Area;
            }
        }
    }else {
        for(int i = 0;i<result.size();i++)
        {
            result[i].StationNumber = m_numGW;
            ResultType type;
            type = (ResultType)readkey(QDir::currentPath() +"/checkitemopetion.ini","UP",QString("resultType%1").arg(i+1)).toInt();
            qDebug()<<QString("�ϲ�%1���������:").arg(i)<<type<<QDir::currentPath() +"/checkitemopetion.ini";
            result[i].positionInt = readkey(QDir::currentPath() +"/checkitemopetion.ini","UP",QString("positionInt%1").arg(i+1)).toInt();
            result[i].resultType = type;
            result[i].objectNumber = objectNumber;
            if(type == Convex_Type)
            {
                result[i].Deep = readkey(QDir::currentPath() +"/checkitemopetion.ini","UP",QString("Deep%1").arg(i+1)).toFloat();
                result[i].Length = readkey(QDir::currentPath() +"/checkitemopetion.ini","UP",QString("Length%1").arg(i+1)).toFloat();
                result[i].Area =  readkey(QDir::currentPath() +"/checkitemopetion.ini","UP",QString("Area%1").arg(i+1)).toFloat();
                qDebug()<<QString("�ϲ�%1���������:").arg(i)<<result[i].Deep <<result[i].Length<<result[i].Area;
            }
        }
    }

}



void ThreeDTasks::checkResultHandle(QVector<RESULT>&result,bool isDown)
{
    // static char resultUpDataChar[CHECK_3D_ITEM_COUNT]={0};//��¼�ϲ�ÿ�����Ƿ��Ѿ���⣬��ʼ��Ϊ0��1��0k,2:ȱ�⣬3�����⣬4�����ȣ�    �洢��0-6
    // static char resultDownDataChar[CHECK_3D_ITEM_COUNT]={0};//��¼�²�ÿ�����Ƿ��Ѿ���⣬��ʼ��Ϊ0��1��0k,2:ȱ�⣬3�����⣬4�����ȣ�  �洢��1-7
    if(isDown)
    {
        bool isDownEmpty = true;
        bool isUpEmpty = true;
        for (int i = 0;i < CHECK_3D_ITEM_COUNT;i++)
        {
            if(resultDownDataChar[i] != 0)
                isDownEmpty = false;
        }

        for (int i = 0;i < CHECK_3D_ITEM_COUNT;i++)
        {
            if(resultUpDataChar[i] != 0)
                isUpEmpty = false;
        }



        if(!isDownEmpty)
        {
            qDebug()<<"�ظ�����²���";
            return;
        }
        for(int i = 0;i<result.size();i++)
        {
            if( result.at(i).positionInt < CHECK_3D_ITEM_COUNT - 1 && result.at(i).positionInt >= 0)  //�ȴ洢��������λ��  ��0��7��
            {
                resultDownDataChar[result.at(i).positionInt] = result.at(i).resultType;
            }

            if(result[i].resultType != OK_Type) //
            {
                //�жϼ���ϲ���Ӧ�Ľ���뱾���²���Ӧ��ͬһ���� �Ƿ���ͬһ�ֲ�����ͬһ��������ν����ͬ��Ѳ���������浽���ս������
                qDebug()<<"resultUpDataChar:"<<(qint8)resultUpDataChar[0]
                       <<(qint8)resultUpDataChar[1]
                      <<(qint8)resultUpDataChar[2]
                     <<(qint8)resultUpDataChar[3]
                    <<(qint8)resultUpDataChar[4]
                   <<(qint8)resultUpDataChar[5]
                  <<(qint8)resultUpDataChar[6]
                 <<(qint8)resultUpDataChar[7];
                qDebug()<<"resultDownDataChar[result.at(i).positionInt]:" <<(qint8)resultDownDataChar[result.at(i).positionInt];
                qDebug()<<"resultUpDataChar[result.at(i).positionInt + 1]:" <<(qint8)resultUpDataChar[result.at(i).positionInt ];
                if(resultDownDataChar[result.at(i).positionInt] != resultUpDataChar[result.at(i).positionInt]) //result.at(i).positionInt + 1  ��֤�Ƕ�Ӧ���ϲ���Ľ��
                {
                    strcpy(result[i].position, checkPosition[result.at(i).positionInt]);
                    resultDataQueue.enqueue(result.at(i));
                    qDebug()<<"����ͬ�ֲ���"<<"��ʾ�������"<<i;
                }else {
                    qDebug()<<"��ͬ�ֲ���"<<"resultDownDataChar[result.at(i).positionInt]��"<<(qint8)resultDownDataChar[result.at(i).positionInt]
                           <<"resultUpDataChar[result.at(i).positionInt + 1]:"<<(qint8)resultUpDataChar[result.at(i).positionInt + 1];
                }

            }
        }

        qDebug()<<"�״δ洢��resultDownDataChar:"<<(qint8)resultDownDataChar[0]
               <<(qint8)resultDownDataChar[1]
              <<(qint8)resultDownDataChar[2]
             <<(qint8)resultDownDataChar[3]
            <<(qint8)resultDownDataChar[4]
           <<(qint8)resultDownDataChar[5]
          <<(qint8)resultDownDataChar[6]
         <<(qint8)resultDownDataChar[7];

        //����Ƿ��Ǻϸ����μ������Ϊ1 ���Ǻϸ�
        if(!isUpEmpty)
        {
            bool isOK = true;
            for (int i = 0;i < CHECK_3D_ITEM_COUNT;i++)
            {
                if(resultDownDataChar[i] == Cleanliness_Type || resultDownDataChar[i] ==   Concave_Type || resultDownDataChar[i] ==  Convex_Type)
                {
                    isOK = false;

                }
                if(resultUpDataChar[i] == Cleanliness_Type || resultUpDataChar[i] ==  Concave_Type || resultUpDataChar[i] == Convex_Type)
                {
                    isOK = false;
                }
            }
            if(isOK)
            {
                result[0].resultType = OK_Type;
                resultDataQueue.enqueue(result.at(0));
            }

            memset(resultDownDataChar,0,sizeof(resultDownDataChar));
            memset(resultUpDataChar,0,sizeof(resultDownDataChar));
            qDebug()<<"�²����³�ʼ�������¼";
        }
    } else if (!isDown)  //�ϲ����
    {
        bool isDownEmpty = true;
        bool isUpEmpty = true;
        for (int i = 0;i < CHECK_3D_ITEM_COUNT;i++)
        {
            if(resultDownDataChar[i] != 0)
                isDownEmpty = false;
        }

        for (int i = 0;i < CHECK_3D_ITEM_COUNT;i++)
        {
            if(resultUpDataChar[i] != 0)
                isUpEmpty = false;
        }



        if(!isUpEmpty)
        {
            qDebug()<<"�ظ�����ϲ���";
            return;
        }
        for(int i = 0;i<result.size();i++)
        {
            if( result.at(i).positionInt < CHECK_3D_ITEM_COUNT - 0 && result.at(i).positionInt >= 1)  //�ȴ洢��� ���λ�á�1��8��
            {
                qDebug()<<"resultUpDataChar[result.at(i).positionInt] = result.at(i).resultType;"<<result.at(i).positionInt;
                resultUpDataChar[result.at(i).positionInt] = result.at(i).resultType;
            }

            if(result[i].resultType != OK_Type) //
            {
                //�жϼ���ϲ���Ӧ�Ľ���뱾���²���Ӧ��ͬһ���� �Ƿ���ͬһ�ֲ�����ͬһ��������ν����ͬ��Ѳ���������浽���ս������
                if(resultUpDataChar[result.at(i).positionInt] != resultDownDataChar[result.at(i).positionInt]) //result.at(i).positionInt  ��֤�Ƕ�Ӧ���ϲ���Ľ��
                {
                    strcpy(result[i].position, checkPosition[result.at(i).positionInt]);
                    resultDataQueue.enqueue(result.at(i));
                    qDebug()<<"����ͬ�ֲ���"<<"��ʾ�������"<<i<<result.at(i).positionInt;
                }else {
                    qDebug()<<"��ͬ�ֲ���"<<"resultUpDataChar[result.at(i).positionInt]��"<<(qint8)resultUpDataChar[result.at(i).positionInt]
                           <<"resultDownDataChar[result.at(i).positionInt + 1]:"<<(qint8)resultDownDataChar[result.at(i).positionInt];
                }

            }
        }
        qDebug()<<"�״δ洢��resultUpDataChar:"<<(qint8)resultUpDataChar[0]
               <<(qint8)resultUpDataChar[1]
              <<(qint8)resultUpDataChar[2]
             <<(qint8)resultUpDataChar[3]
            <<(qint8)resultUpDataChar[4]
           <<(qint8)resultUpDataChar[5]
          <<(qint8)resultUpDataChar[6]
         <<(qint8)resultUpDataChar[7];


        //����Ƿ��Ǻϸ����μ������Ϊ1 ���Ǻϸ�
        if(!isDownEmpty)
        {
            bool isOK = true;
            for (int i = 0;i < CHECK_3D_ITEM_COUNT;i++)
            {
                if(resultDownDataChar[i] == Cleanliness_Type || resultDownDataChar[i] ==   Concave_Type || resultDownDataChar[i] ==  Convex_Type)
                {
                    isOK = false;

                }
                if(resultUpDataChar[i] == Cleanliness_Type || resultUpDataChar[i] ==  Concave_Type || resultUpDataChar[i] == Convex_Type)
                {
                    isOK = false;
                }
            }
            if(isOK)
            {
                result[0].resultType = OK_Type;
                resultDataQueue.enqueue(result.at(0));
            }
            memset(resultDownDataChar,0,sizeof(resultDownDataChar));
            memset(resultUpDataChar,0,sizeof(resultDownDataChar));
            qDebug()<<"�ϲ����������³�ʼ�������¼";
        }
    }
}


QByteArray ThreeDTasks::intToByteArray(const int intData)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar) (0x000000ff & intData);
    abyte0[1] = (uchar) ((0x0000ff00 & intData) >> 8);
    abyte0[2] = (uchar) ((0x00ff0000 & intData) >> 16);
    abyte0[3] = (uchar) ((0xff000000 & intData) >> 24);
    return abyte0;
}

quint8 ThreeDTasks::checkData(QByteArray data)
{
            int i;
            quint8 sumlen = 0;
            for(i = 0;i< data.size();i++)
            {
                sumlen ^= data[i];
            }
            return sumlen;
}

