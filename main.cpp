#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#ifdef _WIN64
#pragma comment(lib,"Vevo.lib")
//#pragma comment(lib, "D:/Focalspec/FocalSpecSoftware Development Kit/API/x64/vevo.lib")
#else
//#pragma comment(lib, "Vevo.lib")
//#pragma comment(lib, "E:/Focalspec/FocalSpecSoftware Development Kit/API/Vevo.lib")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // QTextCodec *codec = QTextCodec::codecForName("GBK");
    //QTextCodec::
    MainWindow w;
    w.show();

    return a.exec();
}
