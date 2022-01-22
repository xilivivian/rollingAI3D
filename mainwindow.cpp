#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScroller>
#include <QGuiApplication>
#include <QScreen>
#include "task/taskallocation.h"

#include <QThread>
#include "task/resultdata.h"
#include <QQueue>



#include <QDebug>
#include <QImage>
#include <QDateTime>

#pragma execution_character_set("utf-8")


QQueue<RESULT> resultDataQueue;

/*存储3D相机拍的亮度灰度图和 深度灰度图*/
QQueue<cv::Mat> sideLower2D_MatQueue;
QQueue<cv::Mat> sideLower3D_MatQueue;
QQueue<cv::Mat> sideUpper2D_MatQueue;
QQueue<cv::Mat> sideUpper3D_MatQueue;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   // QFont ft ("Microsoft YaHei", 10, 75);
  //  ft.setPointSize(10);
 //   ui->pushButton_2->setFont(ft);

    setupImageViewers();
     setupModel1(ui->groupBox);
     viewHandle1();


     resize(QGuiApplication::primaryScreen()->availableSize() * 7 / 8);
     setWindowIcon(QIcon(":/images/LOGO_Alpha.png"));
     //setWindowTitle("3D检测");
   //  setWindowTitle("3D检测");
     setWindowTitle("3D");
     setupTaskAllocation3D();

     timer = new QTimer(this);
     connect(timer,SIGNAL(timeout()),this,SLOT(testTimeoutHandle()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupImageViewers()
{
    //up2DImageViewer = new MImageView(this);
   // up3DImageViewer  = new MImageView(this);
   // down2DImageViewer  = new MImageView(this);
   // down3DImageViewer  = new MImageView(this);
   imageViewer2D = new MImageView(this);
   imageViewer3D = new MImageView(this);



    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setFrameShape(QFrame::NoFrame);
    QGridLayout *gridLayout = new QGridLayout(ui->scrollAreaWidgetContents_2);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    // ui->scrollAreaWidgetContents->setLayout(gridLayout);
  //  QHBoxLayout *upHBoxLayout = new  QHBoxLayout;
  //  QHBoxLayout *downHBoxLayout = new  QHBoxLayout;


   // QLabel *upLabel1 = new QLabel("侧面上部(A21、A18、A13)2D实时图",ui->scrollAreaWidgetContents_2);
   // QLabel *upLabel2 = new QLabel("侧面上部(A21、A18、A13)3D实时图",ui->scrollAreaWidgetContents_2);
   // QLabel *upLabel3 = new QLabel(ui->scrollAreaWidgetContents_2);
  //  upLabel3->setPixmap(QPixmap(":/images/ceShang.png"));
  //  QLabel *downLabel1 = new QLabel("侧面下部(A3、A4)2D实时图",ui->scrollAreaWidgetContents_2);
  //  QLabel *downLabel2 = new QLabel("侧面下部(A3、A4)3D实时图",ui->scrollAreaWidgetContents_2);
  //  QLabel *downLabel3 = new QLabel(ui->scrollAreaWidgetContents_2);
  //  downLabel3->setPixmap(QPixmap(":/images/ceXia.png"));
    QLabel *label2D = new QLabel("侧面(A21、A18、A13)2D实时图",ui->scrollAreaWidgetContents_2);
    QLabel *label2D_Icon = new QLabel(ui->scrollAreaWidgetContents_2);
    label2D_Icon->setPixmap(QPixmap(":/images/ceShang.png"));

    QLabel *label3D = new QLabel("侧面(A21、A18、A13)3D实时图",ui->scrollAreaWidgetContents_2);
    QLabel *label3D_Icon = new QLabel(ui->scrollAreaWidgetContents_2);
    label2D_Icon->setPixmap(QPixmap(":/images/ceShang.png"));

    //upHBoxLayout->addWidget(upLabel2);
    //  QSpacerItem * horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //   upHBoxLayout->addItem(horizontalSpacer);
   // upHBoxLayout->addWidget(upLabel3);

   // downHBoxLayout->addWidget(downLabel2);
    // QSpacerItem * horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    //   downHBoxLayout->addItem(horizontalSpacer2);
    //downHBoxLayout->addWidget(downLabel3);




    gridLayout->addWidget(label2D,0,0,Qt::AlignTop);
    gridLayout->addWidget(label2D_Icon,0,1,Qt::AlignTop | Qt::AlignLeft);
    gridLayout->addWidget(label3D,0,2,Qt::AlignTop | Qt::AlignRight);
    gridLayout->addWidget(label3D_Icon,0,3,Qt::AlignTop | Qt::AlignLeft);


    gridLayout->addWidget(imageViewer2D,1,0,1,2);
    gridLayout->addWidget(imageViewer3D,1,2,1,2);





    gridLayout->setColumnStretch(0,1);
    gridLayout->setColumnStretch(2,1);


    //  QSize screenSize = graph->screen()->size();
    //  ui->imagesShowWidget->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
    //  ui->imagesShowWidget->setMaximumSize(screenSize);
    //  ui->imagesShowWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //  ui->imagesShowWidget->setFocusPolicy(Qt::StrongFocus);
    // ui->verticalLayout.sets

    // ui->centralWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    //   ui->clientXSet->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    // ui->listView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->groupBox->setMinimumWidth(550);//430
    ui->groupBox->setMaximumWidth(550);//430
    // up2DImageViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //up2DImageViewer->resize(800,800);
    imageViewer2D->loadFile("E:/image/Frame0c_intensity.bmp");
    imageViewer3D->loadFile("E:/image/Frame0c_height.bmp");
    // up2DImageViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //down2DImageViewer->loadFile("E:/image/Frame0c_intensity.bmp");
    //down3DImageViewer->loadFile("E:/image/Frame0c_height.bmp");
    cv::Mat mat1 = cv::imread("E:/image/Frame0c_intensity.bmp");
    cv::Mat mat2 = cv::imread("E:/image/Frame0c_height.bmp");
    cv::cvtColor(mat2,mat2,cv::COLOR_BGR2RGB);

    cv::flip(mat1,mat1,1);
    cv::transpose(mat1,mat1);

    cv::flip(mat2,mat2,1);
    cv::transpose(mat2,mat2);

    QImage image1(mat1.data,mat1.cols,mat1.rows, QImage::Format_RGB888);
    imageViewer2D->setImage(image1);//QImage::Format_Grayscale8

    QImage image2(mat2.data,mat2.cols,mat2.rows, QImage::Format_RGB888);
    imageViewer3D->setImage(image2);//QImage::Format_Grayscale8
}

void MainWindow::setupModel1(QWidget *widget)
{
    QDateTime dateTime(QDateTime::currentDateTime());

    ui->label_2->setText(QString("日期：")+dateTime.toString("yyyy/MM/dd"));
    model1 = new  MModel(0,7,widget);
    model1->setHeaderData(0,Qt::Horizontal,QString(" 序号 "));
    model1->setHeaderData(1,Qt::Horizontal,QString("不合格位置"));
    model1->setHeaderData(2,Qt::Horizontal,QString("清洁度"));
    model1->setHeaderData(3,Qt::Horizontal,QString("缺陷类型"));
    model1->setHeaderData(4,Qt::Horizontal,QString("缺陷深度(mm)"));
    model1->setHeaderData(5,Qt::Horizontal,QString("缺陷长度(mm)"));
    model1->setHeaderData(6,Qt::Horizontal,QString("缺陷面积(mm2)"));

}

void MainWindow::viewHandle1()
{
    {
        ui->tableView->setModel(model1);
        QFont ft ("Microsoft YaHei", 10, 30);
        ft.setPointSize(10);
        // QStandardItem *headerItem = model->verticalHeaderItem(0);
        // headerItem->setFont(ft);


        QHeaderView *pHorizontal = new QHeaderView(Qt::Horizontal,this);//使用自定义的列头
        if (pHorizontal)
        {
            pHorizontal->setFont(ft);
            pHorizontal->setDefaultAlignment(Qt::AlignLeft);
            pHorizontal->setStyleSheet("QHeaderView::section {border:0px solid #000000;"
                                       "background-color: #ffffff;"
                                       "color:#000000;"
                                       "}");

            ui->tableView->setHorizontalHeader(pHorizontal);//设置水平的表头
        }

#if 0
        ui->tableView->setStyleSheet("QTableView{border:1px solid #000000;"
                                     "background-color: #ffffff"
                                     "}"); //当 tableView 没有加载数据的部分可以背景黑色生效,加载数据部分颜色由mode的data()决定.
#endif

        ui->tableView->setFont(ft);
        ui->tableView->setShowGrid(true);
        QItemSelectionModel  *SelectionModel = new QItemSelectionModel(model1);
        ui->tableView->setSelectionModel(SelectionModel);
        connect(SelectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),ui->tableView,SLOT(selectionChanged(QItemSelection,QItemSelection)));
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->verticalHeader()->hide();
        ui->tableView->resizeColumnsToContents(); //根据内容自动调整列宽
        // ui->tableView->setColumnWidth(0, 20);
        //tableView1->setColumnWidth(1, 200);

        QScroller *pScroller = QScroller::scroller(ui->tableView);
        pScroller->grabGesture(ui->tableView,QScroller::LeftMouseButtonGesture);
        QScrollerProperties scrPrt_tableView =  pScroller->scrollerProperties();//https://blog.csdn.net/wuyu92877/article/details/112175856
        scrPrt_tableView.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.1);  // 0.1=100/1000  总共1000个像素，只允许拖拽100个像素
        pScroller->setScrollerProperties(scrPrt_tableView);

        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    }
}

void MainWindow::setupTaskAllocation3D()
{
    TaskAllocation *task = new TaskAllocation;
    QThread *thread = new QThread;
    task->moveToThread(thread);


    connect(thread,SIGNAL(started()),task,SLOT(initTask()));

    connect(this, SIGNAL(isStartCheck(bool)),task,SLOT(isStartCheckAndPLC(bool)));//
    connect(task,SIGNAL(checkSuccessFinished(bool)),this,SLOT(checkSuccessFinishedHandle(bool)));


    connect(this,SIGNAL(stopCameraSig()),task,SIGNAL(stopCameraSig()));
    connect(task,SIGNAL(cameraStatesSig(int)),this,SLOT(setCameraStates(int)));

    //测试使用，所有2D检测完成开始下一次检测。
   // connect(task,SIGNAL(check2DFinishedAll()),this,SLOT(on_pushButton_2_clicked()));
    thread->start();
}

void MainWindow::checkSuccessFinishedHandle(bool isSideLower)
{
    upDataImageViewers(isSideLower);
    upDataRealTimeCheckResult();

}

void MainWindow::testTimeoutHandle()
{
    qDebug()<<"void MainWindow::testTimeoutHandle()";
}

void MainWindow::upDataImageViewers(bool isSideLower)
{
    qDebug()<<"upDataImageViewers更新图像";
  //  if(isSideLower)
       if(sideLower2D_MatQueue.size() > 0)
       {
           cv::Mat mat = sideLower2D_MatQueue.dequeue();
          // cv::imshow("1",mat);
          // cv::waitKey(0);
           qDebug()<<"mat.rows:"<<mat.rows<<"mat.cols:"<<mat.cols<<"mat.channels:"<<mat.channels();
           cv::flip(mat,mat,1);
           cv::transpose(mat,mat);//逆时针旋转90度
            imageViewer2D->setImage(QImage(mat.data,mat.cols,mat.rows,mat.cols*3, QImage::Format_RGB888));//QImage::Format_Grayscale8
       }
       if(sideLower2D_MatQueue.size() > QUEUE_SHOW_MAX_SIZE)
       {
           sideLower2D_MatQueue.clear();
       }

       if(sideLower3D_MatQueue.size() > 0)
       {
           cv::Mat mat = sideLower3D_MatQueue.dequeue();
           cv::cvtColor(mat,mat,cv::COLOR_BGR2RGB);
           cv::flip(mat,mat,1);
           cv::transpose(mat,mat);//逆时针旋转90度

           QImage image(mat.cols,mat.rows,QImage::Format_RGB888);
          // heightMatToImage(mat, image);
          imageViewer3D->setImage(QImage(mat.data,mat.cols,mat.rows,mat.cols*3, QImage::Format_RGB888));//QImage::Format_Grayscale8
          // imageViewer3D->setImage(image);//QImage::Format_Grayscale8
       }
       if(sideLower3D_MatQueue.size() > QUEUE_SHOW_MAX_SIZE)
       {
           sideLower3D_MatQueue.clear();
       }

}

void MainWindow::upDataRealTimeCheckResult()
{
    qDebug()<<"void MainWindow::check2DTaskFinshedHandle()";
    int size = resultDataQueue.size();
   // static int index = 0;
    if(size > 0)
    {
       qDebug()<<"MainWindow upData CheckResult@@@@@@@@@@@@@@@@@@@@@@@";

        for(int i = 0;i < size;i++)
        {
           RESULT result =  resultDataQueue.dequeue();

            if(result.resultType ==   Cleanliness_Type)
            {
                    qDebug()<<"MainWindow upData CheckResult  Cleanliness_Type~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~1"<<size;
                    int count = model1->rowCount();
                    model1->insertRows(count,1);
                    model1->setData(model1->index(count,0),result.objectNumber);      //序号
                   // model1->setData(model1->index(count,1),QString(result2DDataQueue.at(i).position)); //不合格位置
                    model1->setData(model1->index(count,1),QString("#")); //不合格位置
                    model1->setData(model1->index(count,2),QString("NG")); //清洁度                            //脏污
                    model1->setData(model1->index(count,3),QString("清洁度"));                              //缺陷类型
                    model1->setData(model1->index(count,4),"/");       //缺陷长度
                    model1->setData(model1->index(count,5),result.Length);
                    model1->setData(model1->index(count,6),result.Area);
            }
            else if(result.resultType ==   Convex_Type)  //缺肉
            {

                qDebug()<<"MainWindow upData CheckResult  Convex_Type~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~2"<<size;
                int count = model1->rowCount();
                model1->insertRows(count,1);
                model1->setData(model1->index(count,0),result.objectNumber);      //序号
               // model1->setData(model1->index(count,1),QString(result2DDataQueue.at(i).position)); //不合格位置
                model1->setData(model1->index(count,1),QString("#")); //不合格位置
                model1->setData(model1->index(count,2),QString("OK"));                             //脏污
                model1->setData(model1->index(count,3),QString("缺肉"));                              //缺陷类型
                model1->setData(model1->index(count,4),result.Deep);       //缺陷长度
                model1->setData(model1->index(count,5),result.Length);
                model1->setData(model1->index(count,6),result.Area);

            }else if(result.resultType ==   OK_Type)
            {
                qDebug()<<"MainWindow upData CheckResult  OK_Type~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~3"<<size;
                int count = model1->rowCount();
                model1->insertRows(count,1);
                model1->setData(model1->index(count,0),result.objectNumber);      //序号
                qDebug()<<"~~~~~~~~~~~~~~~~~~~~~~";
                model1->setData(model1->index(count,1),"/"); //不合格位置
                model1->setData(model1->index(count,2),QString("OK"));                             //脏污
                model1->setData(model1->index(count,3),"/");                              //缺陷类型
                model1->setData(model1->index(count,4),"/");
                model1->setData(model1->index(count,5),"/");   //缺陷长度
                model1->setData(model1->index(count,6),"/");
            }
        }
       // ui->tableView->sortByColumn(0,Qt::SortOrder::AscendingOrder);
       // for(int i = 0;i<size;i++)
       // {
            //inbox::saveCheckData(result2DDataQueue.dequeue());
        //    (result2DDataQueue.dequeue());  //清除
       // }
    }
}

void MainWindow::heightMatToImage(cv::Mat heightMat, QImage &image)
{
    float MAX_Z_VALUE = 2500.0;
    //bitmap_image image;
    //image.setwidth_height(m_width, m_height, true);

    const int NUMBER_OF_COLORS = 256;
    rgb_t palette[NUMBER_OF_COLORS];

    memset(palette, 0, sizeof(rgb_t)); // first color: (0, 0, 0)

    // create the color palette
    for (auto i = 1; i < NUMBER_OF_COLORS; i++)
    {
        {
            palette[i].blue = 50;
            palette[i].green = i;
            palette[i].red = 255 - i;
        }
    }

    for (auto i = 0; i < heightMat.rows; i++)
    {
       // float* row;
        float* row_3D = heightMat.ptr<float>(i);
        {
           // row = m_zValues + i * m_width;
            for (int j = 0; j < heightMat.cols; j++)
            {
                // Scale Z value (micrometers) to palette color
                int paletteIndex = static_cast<int>((NUMBER_OF_COLORS / 2) * row_3D[j] / MAX_Z_VALUE + (NUMBER_OF_COLORS / 2));
                if ((paletteIndex < 0) || (paletteIndex > 255))
                {
                    // out of range or not measured point
                   // image.set_pixel(j, i, palette[0]);
                    image.setPixelColor(j,i,QColor(palette[0].red,palette[0].green,palette[0].blue));
                }
                else
                {
                    image.setPixelColor(j,i,QColor(palette[paletteIndex].red,palette[paletteIndex].green,palette[paletteIndex].blue));
                }
            }
        }
    }
}

void MainWindow::on_stopPushButton_clicked()
{
  // emit stopCameraSig(false);
    emit isStartCheck(false);
#if 0
 static int index = 0;
 index++;
 if(index == 1)
 {
     timer->start(50);
 }else if(index %2 == 0)
 {
     timer->stop();
 }else {
   //timer->setInterval(100);
   timer->start();
}
#endif


}

void MainWindow::on_startPushButton_clicked()
{
    qDebug()<<"MainWindow::on_startPushButton_clicked()";

    emit isStartCheck(true);
#if 0
    static int position = 0;

     static int num = 0 ; //检测的端塞数量
    int checkNum =  100000;  //计划检测的总量
    position++;
    if(position == 3)
      position = 1;


    if(num >= checkNum)
    {
        emit setCheckTaskForTest2(1,true);
        //ui->pushButton_2->setText("无端塞");
    }else if(position == 1){
      emit setCheckTaskForTest2(position,false);
       // ui->pushButton_2->setText("检测底部");
   }else if(position == 2)
    {
        emit setCheckTaskForTest2(position,false);
          //ui->pushButton_2->setText("检测顶部");
    }


    if(position == 1)
    {
        num++;
    }
#endif
}

void MainWindow::setCameraStates(int states)
{
    if(states != 0)
    {
        ui->label_4->setText("相机状态：异常");
    }else {
       ui->label_4->setText("相机状态：正常");
    }
}
