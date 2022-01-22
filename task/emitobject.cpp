#include "emitobject.h"

EmitObject::EmitObject(QObject *parent) : QObject(parent)
{
    currentFrameIndex = 0;
}

void EmitObject::emitCamearFinished(int currentFrameIndex)
{
    emit camearFinished(currentFrameIndex);
}



void EmitObject::emitCamearFinished()
{
   emit camearFinished();
}


