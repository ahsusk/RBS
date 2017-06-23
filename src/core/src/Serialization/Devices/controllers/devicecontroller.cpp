#include "devicecontroller.h"
#include <core/src/Serialization/Devices/device.h>
#include <logging.h>

typedef DeviceController DCD;

QList<DeviceController*> DeviceController::all_controllers_;

DeviceController::DeviceController(
        Device *_device,
        bool _auto_read) :
    device_(_device),
    obj_(0)
{
    obj_ = new DeviceControllerObject(this,_device, _auto_read);
    all_controllers_ << this;
}

DeviceController::~DeviceController()
{
    __clear();
    all_controllers_.removeAll(this);
}

void DeviceController::attach(Device *_device,
                              bool _auto_read)
{
    if (obj_)
        __clear();

    _detach(device_);
    detached(device_);

    obj_ = new DeviceControllerObject(this,_device, _auto_read);

    device_ = _device;
    _device_attached(device_);
    attached(device_);
}

Device *DeviceController::getDevice() const
{
    return device_;
}

bool DeviceController::setState(DeviceState _state)
{
    return obj_->setState(_state);
}

bool DeviceController::hasOtherControllers() const
{
    QList<DCD*> controllers = getControllers(device_);
    return controllers.count() > 1;
}

void DeviceController::_init()
{
    obj_->init();
}

void DeviceController::__clear()
{
    bool has_other = hasOtherControllers();
    qDebug(SUBMIC_LOG)<<(has_other?"Найдены":"Не найдены")<<"другие контроллеры"
                     <<"для устройства"<<device_;
    if (!has_other)
    {
        qDebug(SUBMIC_LOG)<<"Уничтожен последний контроллер устройства"<<device_;
        device_->disconnect(obj_);
        delete device_;
        device_ = 0;
    }
    delete obj_;
    obj_ = 0;
}

QList<DeviceController *> DeviceController::getControllers(
        Device *_w)
{
    QList<DCD*> result;
    foreach (DCD* d, all_controllers_)
    {
        if (d->getDevice() == _w)
        {
            result << d;
        }
    }
    return result;
}

