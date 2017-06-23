#include "device.h"
#include "private/adeviceimplementor.h"
#include <logging.h>
#include "pool/devicemap.h"
Device::Device(ADeviceImplementor *_impl) :
    QIODevice(),
    impl_(_impl),
    modified_(false),
    state_(Default),
    always_save_(false)
{
    setObjectName(impl_->objectName());

    connect(_impl,
            SIGNAL(rbsOpenFile()),
            SIGNAL(rbsOpenFile()),
            Qt::DirectConnection);

    connect(_impl,
            SIGNAL(rbsCloseFile()),
            SIGNAL(rbsCloseFile()),
            Qt::DirectConnection);

    connect(_impl,
            SIGNAL(needActualData()),
            SIGNAL(needActualData()),
            Qt::DirectConnection);

    theDeviceMap()->__push_me(this);
}

Device::State Device::_get_state() const
{
    return state_;
}

Device::~Device()
{
    disconnect(impl_,0,this,0);
    delete impl_;
    qDebug(SUBMIC_LOG)<<this<<"уничтожен";

}

bool Device::save()
{
    qDebug(SUBMIC_LOG)<<"сохранение устройства"<<this;
    if (!isModified())
    {
        qDebug(SUBMIC_LOG)<<"Устройство не модифицировано"<<this;
        return true;
    }

    qDebug(SUBMIC_LOG)<<this<<"подготовка к записи на диск";
    emit aboutToBeSaved();

    state_ = Saving;

    bool result = impl_->save();
    if (result)
    {
        qDebug(SUBMIC_LOG)<<"Устройство сохранено"<<this;
        set_modified(false);
        emit saved();
    }

    state_ = Default;
    return result;
}

bool Device::load()
{
    if (impl_->isWriteOnlyDevice())
        return true;

    qDebug(SUBMIC_LOG)<<this<<"подготовка к загрузке с диска";
    emit aboutToBeLoaded();

    state_ = Loading;

    bool result = impl_->load();

    if (result)
    {
        qDebug(SUBMIC_LOG)<<this<<"данные загружены";
        set_modified(false);
        emit loaded();
    }
    else
        modify();

    state_ = Default;
    return result;
}

bool Device::open(QIODevice::OpenMode _open_mode)
{
    bool result = impl_->open(_open_mode);

    result = result && QIODevice::open(_open_mode);

    return result;
}

void Device::close()
{
    impl_->close();
    QIODevice::close();
}

qint64 Device::readData(char *data, qint64 maxlen)
{
    return impl_->read(data,maxlen);
}

qint64 Device::writeData(const char *data, qint64 len)
{
    qint64 result = impl_->write(data,len);
    if (_get_state() != Loading)
    {
        modify();
    }
    emit dataChanged();
    return result;
}

bool Device::needAskForSaving()
{
    if (always_save_)
    {
        save();
        return false;
    }
    return impl_->needAskForSaving();
}

void Device::setAlwaysSave(bool _as)
{
    always_save_ = _as;
}

bool Device::fileIsOpen(OpenMode _mode) const
{
    return impl_->checkFile(_mode);
}

void Device::set_modified(bool _modified)
{
    if (modified_ != _modified)
    {
        modified_ = _modified;
        qInfo(SUBMIC_LOG).noquote()<<this<<": модификация:"<<modified_;
        emit modificationChanged(modified_);
    }
}


void Device::modify()
{
    set_modified(true);
}

QByteArray Device::getData()
{
    QByteArray data;
    if (open(ReadOnly))
        data = readAll();
    close();
    return data;
}

