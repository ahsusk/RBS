#include "devicemap.h"
#include "src/Serialization/Devices/devicefactory.h"
#include "src/Serialization/Devices/private/adeviceimplementor.h"
#include "src/Serialization/Devices/device.h"
#include <QThread>
#include <QApplication>
#include <logging.h>
#include "devicewrapperfactory.h"
#include <QMessageBox>
#include "../controllers/devicecontroller.h"
DeviceMap::DeviceMap()
{


}

Device *DeviceMap::getDevice(const QString &_path)
{
    Device* result = 0;
    if (contains(_path))
        result = value(_path);
    else
    {
        // в конструкторе класса RealDevice объект сам себя добавить
        // в карту DeviceMap
        result = theDeviceFactory()->createBufferedDevice(_path);
    }

    return result;
}

QList<Device *> DeviceMap::getDevices() const
{
    return map_.values();
}


void DeviceMap::__push_me(Device *_device)
{
    QString abs_path = _device->getAbsolutePath();
    connect(_device,
            SIGNAL(destroyed(QObject*)),
            SLOT(device_detroyed(QObject*)));

    map_[abs_path] = _device;

    if (qApp && QThread::currentThread() != qApp->thread())
    {
        qDebug(SUBMIC_LOG)<<tr("Перемещаем документ %1 в поток приложения.")
                            .arg(abs_path);
        _device->moveToThread(qApp->thread());
    }

    emit deviceMapped(_device);
}

QString DeviceMap::get_clean_path(const QString &_path) const
{
    QFileInfo finfo(_path);
    return finfo.absoluteFilePath();
}

Device *DeviceMap::value(const QString &_path) const
{
    QString path = get_clean_path(_path);
    return map_.value(path);
}

void DeviceMap::remove(Device *_device)
{
    QString key = _device->getAbsolutePath();
    QList<DeviceController*> controllers =
            DeviceController::getControllers(_device);
    if (!controllers.isEmpty())
        qDebug()<<"для файла"<<key<<"было создано"<<controllers.count()
           <<"контроллеров";
    foreach (DeviceController* dcd, controllers)
    {
        qDebug()<<"удаляем контроллер"<<dcd<<dynamic_cast<QObject*>(dcd);
        delete dcd;
    }
    if (!controllers.isEmpty())
        qDebug()<<"Все контроллеры файла"<<key<<"удалены" ;
}

void DeviceMap::remove(const QString &_path)
{
    Device* dw = value(_path);
    remove(dw);
}

bool DeviceMap::contains(const QString &_path) const
{
    QString path = get_clean_path(_path);
    return map_.contains(path);
}



void DeviceMap::device_detroyed(QObject *_device)
{
    bool found = false;
    QMapIterator <QString, Device*> iter(map_);
    while (iter.hasNext())
    {
        iter.next();
        if (_device == iter.value())
        {
            QString key = iter.key();
            map_.remove(key);
            emit deviceRemoved(key);
            found = true;
            break;
        }
    }
    if (!found)
    {
        qWarning(SUBMIC_LOG)<<"Устройство"<<_device<<"не найдено";
    }
}


SM_MYERS_SINGLETON_DEF(DeviceMap)
