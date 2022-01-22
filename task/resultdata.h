#ifndef RESULTDATA_H
#define RESULTDATA_H
#include <QPoint>
enum ResultType{
  OK_Type = 0,
  Convex_Type = 1,//凹
  Concave_Type = 2,//凸
  Cleanliness_Type = 3, //清洁度
};
struct RESULT{
    qint32 objectNumber;
    qint8 StationNumber;
    ResultType resultType;
    int positionInt;     //记录本次的检测的面的序号 目的：方便和客户定义的区域对应
    char position[32];
    float Deep;
    float Length;
    float Area;

};

#define CHECK_3D_ITEM_COUNT 8     //3D检测共分8次（8个面）检测，其中下部检测 7 个面，上部检测7个面  存在大量重复的面

#define QUEUE_SHOW_MAX_SIZE 3 //供显示的队列最大可缓存的图像数量

#define SET_BATCH_LENGTH 1790
#define SET_BATCH_FREQ  600

/*
 * 硬触发：产生脉冲的设备旋转一圈产生1600条脉冲，设置这个设备一次触发 1800条脉冲，实际设置（SET_BATCH_LENGTH） 1790条脉冲，其余触发的扫描数据在回调函数中清空相机缓存（拷贝结束后）
 * 刚进相机回调时，给出第四个工位上段塞的结果。
*/


#endif // RESULTDATA_H
