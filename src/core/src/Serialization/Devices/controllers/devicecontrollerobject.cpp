#include "devicecontrollerobject.h"
#include "devicecontroller.h"
#include "../device.h"
#include <logging.h>
#include <QMetaMethod>
#define LOG qDebug()<<objectName()
#define CLOG(C) qDebug(C)<<objectName()

DeviceControllerObject::DeviceControllerObject(DeviceController *_controller,
        Device *_device,
        bool _auto_read) :
    controller_(_controller),
    device_(_device)
{

    state_ = _auto_read ? PreRead : Default;

//    QList<DeviceControllerObject*> siblings =
//            _device->findChildren<DeviceControllerObject*>();

    QList<DeviceController*> controllers =
            DeviceController::getControllers(device_);
    controllers.removeAll(controller_);
    foreach (DeviceController* dcd, controllers)
    {
        DeviceControllerObject* obj = dcd->obj_;
        connect_sibling(obj);
    }

    connect(_device,
            SIGNAL(destroyed(QObject*)),
            SLOT(on_device_destroyed()),
            Qt::DirectConnection);

    connect(device_,
            SIGNAL(needActualData()),
            SLOT(on_need_actual_data()),
            Qt::DirectConnection);

//    connect(device_,
//            SIGNAL(saved()),
//            SLOT(force_read()),
//            Qt::QueuedConnection);

    connect(device_,
            SIGNAL(loaded()),
            SLOT(force_read()),
            Qt::QueuedConnection);

}

bool DeviceControllerObject::setState(DeviceState _state)
{
    if (state_ == _state)
    {
        return true;
    }

    bool css = can_set_state(_state);
    if (!css)
    {
        return false;
    }

    CLOG(SUBMIC_LOG)<<state_<<"->"<<_state;
    DeviceState old_state = state_;
    state_ = _state;


    switch(_state)
    {
    case Write:
        emit stateChanged(old_state,_state);
        do_write();
        break;
    case Read:
        emit stateChanged(old_state,_state);
        do_read();

        break;
    case PreWrite:
        CLOG(SUBMIC_LOG)<<"modify device"<<device_;
        device_->modify();
        emit stateChanged(old_state,_state);
        break;
    case PreRead:
        emit stateChanged(old_state,_state);
        if (controller_->_need_force_read())
            setState(Read);
        break;
    default:
        emit stateChanged(old_state,_state);
        break;
    }
    return true;
}

void DeviceControllerObject::init()
{
    QObject* controller_obj = dynamic_cast<QObject*>(controller_);
    if (controller_obj != NULL)
    {
        QString classname = controller_obj->metaObject()->className();
        static const QString mask = "%1(%3)(%2)(addr=0x%4)";

        QString ctrl_obj_name = controller_obj->objectName();
        QRegExp rx("\\((.*)\\)");
        int rx_index = rx.indexIn(ctrl_obj_name);
        if (rx_index != -1 && rx.matchedLength() == (ctrl_obj_name.length() - rx_index))
        {
            ctrl_obj_name = rx.cap(1);
        }
        else
            ctrl_obj_name.clear();

        QString result = mask.arg(classname)
                             .arg(device_->getFileName())
                             .arg(ctrl_obj_name)
                             .arg((int)controller_,0,16);

        setObjectName(result);

        connect(controller_obj,
                SIGNAL(objectNameChanged(QString)),
                SLOT(init()));

        if (!sender())
            qDebug(SUBMIC_LOG)<<"new dco"<<result;
    }


}

void DeviceControllerObject::connect_sibling(DeviceControllerObject *_sibling)
{
    connect(_sibling,
            SIGNAL(stateChanged(DeviceState,DeviceState)),
            SLOT(on_other_state_changed(DeviceState,DeviceState)),
            Qt::QueuedConnection);

    connect(this,
            SIGNAL(stateChanged(DeviceState,DeviceState)),
            _sibling,
            SLOT(on_other_state_changed(DeviceState,DeviceState)),
            Qt::QueuedConnection);

//    connect(this,
//            SIGNAL(wrote()),
//            _sibling,
//            SLOT(force_read()),
//            Qt::DirectConnection);
//    connect(_sibling,
//            SIGNAL(wrote()),
//            SLOT(force_read()),
//            Qt::DirectConnection);

}

void DeviceControllerObject::on_other_state_changed(DeviceState _old, DeviceState _new)
{
    if (_old == Write && _new == Default)
    {
        LOG<<": other wrote. Read";
        setState(Read);
    }

    if (_old == PreWrite && _new == Write)
    {
        LOG<<": other is preparing to write. Pre Read";
        setState(PreRead);
    }
}

void DeviceControllerObject::on_device_destroyed()
{
    qDebug()<<"device destroyed"<<sender()<<this;
    QObject* obj = dynamic_cast<QObject*>(controller_);
    qDebug()<<"delete controller"<<obj;
    delete controller_;
}

void DeviceControllerObject::on_need_actual_data()
{
    setState(Write);

}


void DeviceControllerObject::force_read()
{
    QObject* obj = sender();
    int index = senderSignalIndex();
    QMetaMethod method = obj->metaObject()->method(index);
    LOG<<"try read force"<<"(from signal "<<method.name()<<"of object"<<obj<<")";
    if (!setState(PreRead))
        qWarning()<<"force read error. PreRead";
    else if (!setState(Read))
        qWarning()<<"force read error. Read";
}

void DeviceControllerObject::do_write()
{
    CLOG(SUBMIC_LOG)<<this<<"write to buffer";
    bool result = controller_->_do_write();
    QByteArray data = device_->getData();
    CLOG(SUBMIC_LOG)<<"wrote"<<data.length()<<"bytes";

    if (result)
    {
        emit wrote();
        setState(Default);
    }
    else
        setState(PreRead);
}

void DeviceControllerObject::do_read()
{
    controller_->disconnectModification();
    bool result = controller_->_do_read();
    CLOG(SUBMIC_LOG)<<"read"<<device_->getData().length()<<"bytes";

    controller_->connectModification();
    setState(Default);

    if (!result)
    {
        if (setState(PreWrite))
            setState(Write);
    }
}

bool DeviceControllerObject::can_set_state(DeviceState _state)
{
    /// могут быть следующие случаи
    /// PreRead -> Read (ожидает загрузки - загружается)
    /// PreWrite -> Write (ожидает синхронизации - синх)
    /// Read -> Default (загрузился)
    /// Write -> Default( сохранился)
    /// PreWrite -> Default (сброс данных)
    /// PreWrite -> PreRead (файл был отредактирован, но внезапно
    /// изменился вне рбс)

    if ( (state_ == PreRead &&  _state == Read)
         || (state_ == PreWrite && _state == Write))
    {
        //ждал и сделал
        return true;
    }

    if (_state == Default
            && (state_ == Read || state_ == Write))
    {
        //сделал и расслабился
        return true;
    }

    if (state_ == Default
            && (_state == PreWrite || _state == PreRead))
    {
        //ничего не делал, а теперь ждет
        return true;
    }

    if (_state == PreRead && state_ == PreWrite)
    {
        qWarning()<<tr("Устройство %1 ожидало записи в буфер, "
                       "но данные были изменены извне.")
                    .arg(objectName());
        return true;
    }

    return false;
}
