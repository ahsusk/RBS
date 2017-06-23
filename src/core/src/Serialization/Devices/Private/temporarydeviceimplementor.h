#ifndef TEMPORARYDEVICEIMPLEMENTOR_H
#define TEMPORARYDEVICEIMPLEMENTOR_H

#include "directlydeviceimplementor.h"
class TemporaryDeviceImplementor : public DirectlyDeviceImplementor
{
public:
    TemporaryDeviceImplementor(const QString& _abs_path);

public:
    bool needAskForSaving() const final;
    bool isWriteOnlyDevice() const final;

    void close() final;
};

#endif // TEMPORARYDEVICEIMPLEMENTOR_H
