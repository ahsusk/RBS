#ifndef IDEVICEPRESENTERFACTORY_H
#define IDEVICEPRESENTERFACTORY_H

class DevicePresenter;
class Device;
class QWidget;
class IDevicePresenterFactory
{
public:
    virtual DevicePresenter* initPresenter(Device* _dev, QWidget* _view, bool _auto_read) = 0;
    virtual ~IDevicePresenterFactory() {}
};

#endif // IDEVICEPRESENTERFACTORY_H
