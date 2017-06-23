#ifndef DIRECTLYDEVICEIMPLEMENTOR_H
#define DIRECTLYDEVICEIMPLEMENTOR_H

#include "adeviceimplementor.h"
class DirectlyDeviceImplementor : public ADeviceImplementor
{
public:
    DirectlyDeviceImplementor(const QString& _abs_path);

    // ADeviceImplementor interface
public:
    bool open(QIODevice::OpenMode mode);
    qint64 read(char *_data, qint64 _maxlen);
    qint64 write(const char *_data, qint64 _len);
    void close();

    bool load() override;
    bool save() override;


};

#endif // DIRECTLYDEVICEIMPLEMENTOR_H
