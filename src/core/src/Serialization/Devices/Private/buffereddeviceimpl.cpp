#include "buffereddeviceimpl.h"
#include "realdevice.h"
#include <logging.h>
#include <QMetaMethod>

BufferedDeviceImplementor::BufferedDeviceImplementor(const QString &_abs_path) :
    ADeviceImplementor(_abs_path)
{
    load();
}

BufferedDeviceImplementor::~BufferedDeviceImplementor()
{
}

bool BufferedDeviceImplementor::open(QIODevice::OpenMode mode)
{
    if (mode & QIODevice::ReadOnly)
        emit needActualData();

    return buffer_.open(mode);
}

qint64 BufferedDeviceImplementor::read(char *_data, qint64 _maxlen)
{
    return buffer_.read(_data,_maxlen);
}

qint64 BufferedDeviceImplementor::write(const char *_data, qint64 _len)
{
    return buffer_.write(_data,_len);
}


bool BufferedDeviceImplementor::save()
{
    if (_check_or_open(QIODevice::WriteOnly))
    {
        if (open(QIODevice::ReadOnly))
        {
            QByteArray buff_data = readAll();

            _get_real_file().write(buff_data);
            _get_real_file().close();

            return true;
        }
        else
        {
            qCritical(SUBMIC_LOG)<<"ошибка чтения из буфера"<<this;
        }
    }
    else
    {
        bool try_again = false;
        emit cannotOpenForWrite(&try_again);

        if (try_again)
            return save();
    }


    return false;
}

bool BufferedDeviceImplementor::load()
{
    if (_check_or_open(QIODevice::ReadOnly))
    {
        if (open(QIODevice::WriteOnly))
        {
            QByteArray buff_data = _get_real_file().readAll();
            const char* data = buff_data.constData();
            write(data,strlen(data));
            _get_real_file().close();
            close();
            return true;
        }
        else
        {
            qCritical(SUBMIC_LOG)<<"ошибка записи в буфер"<<this;
        }
    }
    else
    {
        QFile& file = _get_real_file();
        if (file.exists())
            qWarning(SUBMIC_LOG)<<"не получилось открыть на чтение"<<this;
        else
            qCritical(SUBMIC_UR).noquote()<<tr("Файл %1 пропал!!!").arg(file.fileName());
    }
    return false;
}

void BufferedDeviceImplementor::close()
{
    buffer_.close();
}
