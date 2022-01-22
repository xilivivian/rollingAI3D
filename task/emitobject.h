#ifndef EMITOBJECT_H
#define EMITOBJECT_H

#include <QObject>

class EmitObject : public QObject
{
    Q_OBJECT
public:
    explicit EmitObject(QObject *parent = nullptr);
  //  void setCurrentFrameIndex(int currentFrameIndex);
    void emitCamearFinished(int currentFrameIndex);

    void emitCamearFinished();

signals:
    void camearFinished(int currentFrameIndex);
    void camearFinished();


public slots:

private:
    int currentFrameIndex;

};

#endif // EMITOBJECT_H
