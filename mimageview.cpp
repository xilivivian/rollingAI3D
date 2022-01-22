#include "mimageview.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QImageReader>
#include <QMessageBox>
#include <QDir>
#include <QGuiApplication>

#include <QPixmap>
#include <QDebug>

MImageView::MImageView(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainHLayout = new QVBoxLayout(this); //上下布局
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget *scrollAreaWidget = new QWidget(scrollArea);
    scrollArea->setWidget(scrollAreaWidget);
    mainHLayout->addWidget(scrollArea);

    QVBoxLayout *hLayout1 = new QVBoxLayout(); //上下布局
    scrollAreaWidget->setLayout(hLayout1);

    imageLabel = new QLabel(this);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    // imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    imageLabel->setScaledContents(true);
   // scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Dark);

    hLayout1->addWidget(imageLabel);


    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏竖向滚动条



}

bool MImageView::loadFile(const QString &fileName)
{
  //  QPixmap pixmap(fileName);
  //  imageLabel->setPixmap(pixmap);
    qDebug()<<"Width:"<<this->width()<<"height:"<<this->height();
  //  return  true;

#if 1
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }
//! [2]

    setImage(newImage);

   // setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
   // statusBar()->showMessage(message);
    return true;
#endif
}

void MImageView::setImage(const QImage &newImage)
{
    image = newImage;
   // image = image.scaled(524,352,Qt::KeepAspectRatio);
    qDebug()<<"image.scaled(524,352);::"<<image.width()<<image.height();
    imageLabel->setPixmap(QPixmap::fromImage(image));
//! [4]
   // scaleFactor = 1.0;

    scrollArea->setVisible(true);
   // printAct->setEnabled(true);

   // fitToWindowAct->setEnabled(true);
   //// updateActions();
   // if (!fitToWindowAct->isChecked())
       // imageLabel->adjustSize();
        imageLabel->setScaledContents(true);

}
