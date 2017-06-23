#include "DevicePresenter.h"
#include "rbscore/src/mvc/managers/widgetshandler.h"
#include "core/src/Serialization/Devices/device.h"
#include "src/mvc/managers/DevicePresentermanager.h"
#include <QWidget>
#include <QDebug>
#include "logging.h"

DevicePresenter* DevicePresenter::active_ = 0;
QList<DevicePresenter*> DevicePresenter::all_device_presenters_;

DevicePresenter::DevicePresenter(Device *_device,
                                 QWidget *_view,
                                 bool _auto_read) :
    Presenter(_view),
    DeviceController(_device,_auto_read),
    force_write_(false)
{
    _init();

    connect(_view,
            SIGNAL(closeRequested(void*)),
            SIGNAL(closeRequested(void*)),
            Qt::DirectConnection);

    all_device_presenters_.append(this);
}

DevicePresenter::~DevicePresenter()
{
    if (active_ == this)
        active_ = 0;
    all_device_presenters_.removeAll(this);

    emit deactivated();
    qDebug(SUBMIC_LOG)<<this<<"уничтожен";
}

void DevicePresenter::registerDecorator(AWidgetDecorator *_decorator)
{
    decorators_.append(_decorator);
    _decorator->decorate(getDevice());
}

void DevicePresenter::update()
{
    Device* wrapper = getDevice();
    foreach (AWidgetDecorator* wd, decorators_)
        wd->decorate(wrapper);
}

void DevicePresenter::setForceWrite(bool _fw)
{
    force_write_ = _fw;
}

void DevicePresenter::_detach(Device *_old)
{
    if (!_old) return;

    qDebug()<<"disconnect"<<_old<<"from"<<this;
    disconnect(_old,0,this,0);
    foreach (AWidgetDecorator* d, decorators_)
    {
        qDebug()<<"disconnect"<<_old<<"from"<<d;
        disconnect(_old,0,d,0);
    }
}

void DevicePresenter::_device_attached(Device *_device)
{
    update();
}

bool DevicePresenter::_need_force_read() const
{
    return true;
}

bool DevicePresenter::_need_force_write() const
{
    return force_write_;
}

void DevicePresenter::_modified_by_view()
{
    setState(PreWrite);
}

void DevicePresenter::_activated()
{
    qDebug(SUBMIC_LOG)<<"Активное представление:"<<getWidget();
    active_ = this;
    setState(Read);

    foreach (AWidgetDecorator* wd, decorators_)
        wd->onWidgetGotFocus(getDevice());

    emit activated();
}

void DevicePresenter::_deactivated()
{
    qDebug(SUBMIC_LOG)<<"Деактивирован:"<<getWidget();
    setState(Write);

    foreach (AWidgetDecorator* wd, decorators_)
        wd->onWidgetLostFocus(getDevice());

    emit deactivated();
}

DevicePresenter *DevicePresenter::getPresenter(QWidget *_view)
{
    return _view->findChild<DevicePresenter*>(QString(),
                                              Qt::FindDirectChildrenOnly);
}

QList<DevicePresenter *> DevicePresenter::getPresentersForDevice(
        Device *_device)
{
    QList<DevicePresenter *> result;
    foreach (DevicePresenter* p, all_device_presenters_)
    {
        if (p->getDevice() == _device)
        {
            result.append(p);
        }
    }
    return result;

}

QList<DevicePresenter *> DevicePresenter::getAllPresenters()
{
    return all_device_presenters_;
}

DevicePresenter *DevicePresenter::getActive()
{
    return active_;
}

