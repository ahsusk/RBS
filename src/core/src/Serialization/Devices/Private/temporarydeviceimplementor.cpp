#include "temporarydeviceimplementor.h"
#include <QDebug>
#include <iostream>
TemporaryDeviceImplementor::TemporaryDeviceImplementor(const QString &_abs_path) :
    DirectlyDeviceImplementor(_abs_path)
{
    open(QIODevice::WriteOnly);
}


bool TemporaryDeviceImplementor::needAskForSaving() const
{
    return false;
}

bool TemporaryDeviceImplementor::isWriteOnlyDevice() const
{
    return true;
}

void TemporaryDeviceImplementor::close()
{
    std::cout<<"close tmp device failed"<<objectName().toUtf8().constData()
               <<std::endl;
    qDebug()<<"закрытие файла"<<objectName()<<"запрещено.";
}
