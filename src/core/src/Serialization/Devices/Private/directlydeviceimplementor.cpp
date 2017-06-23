#include "directlydeviceimplementor.h"

DirectlyDeviceImplementor::DirectlyDeviceImplementor(const QString &_abs_path) :
    ADeviceImplementor(_abs_path)
{
}

bool DirectlyDeviceImplementor::open(QIODevice::OpenMode mode)
{
    return _check_or_open(mode);
}


void DirectlyDeviceImplementor::close()
{
    _get_real_file().close();
}

bool DirectlyDeviceImplementor::load()
{
    return true;
}

bool DirectlyDeviceImplementor::save()
{
    return true;
}

qint64 DirectlyDeviceImplementor::read(char *data, qint64 maxlen)
{
    return _get_real_file().read(data,maxlen);
}

qint64 DirectlyDeviceImplementor::write(const char *data, qint64 len)
{
    qint64 result = _get_real_file().write(data,len);
    _get_real_file().flush();
    return result;
}

