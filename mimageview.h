#ifndef MIMAGEVIEW_H
#define MIMAGEVIEW_H

#include <QWidget>
#include <QScrollArea>
#include <QString>
#include <QImage>
#include <QLabel>
class MImageView : public QWidget
{
    Q_OBJECT
public:
    explicit MImageView(QWidget *parent = nullptr);
     bool loadFile(const QString &);

      void setImage(const QImage &newImage);

signals:

public slots:
  private:

    QImage image;
    QLabel *imageLabel;
   // QLabel *imageLabel;
    QScrollArea *scrollArea;
};

#endif // MIMAGEVIEW_H
