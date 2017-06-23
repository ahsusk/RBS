#ifndef BUFFERED_DEVICE_IMPL_H
#define BUFFERED_DEVICE_IMPL_H

#include "adeviceimplementor.h"
#include <QBuffer>

class BufferedDeviceImplementor final : public ADeviceImplementor
{
    Q_OBJECT
public:
    explicit BufferedDeviceImplementor(const QString& _abs_path);
    ~BufferedDeviceImplementor();

signals:
    void cannotOpenForWrite(void* _try_again);
    // ADeviceImplementor interface
public:
    bool open(QIODevice::OpenMode mode) final;
    qint64 read(char *_data, qint64 _maxlen) final;
    qint64 write(const char *_data, qint64 _len) final;
    void close() final;

    bool save() final;
    bool load() final;
private:
    QBuffer buffer_;
};

#endif // BUFFERED_DEVICE_IMPL_H
