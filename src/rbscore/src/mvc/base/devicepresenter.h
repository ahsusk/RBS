#ifndef DEVICE_PRESENTER_BASE_H
#define DEVICE_PRESENTER_BASE_H

#include "core/src/Serialization/Devices/controllers/devicecontroller.h"
#include "presenter.h"
#include "awidgetdecorator.h"
#include <QObject>
#include <rbscore/src/rbscore_global.h>
///
/// \brief The DevicePresenter class
///
class RBSCORESHARED_EXPORT DevicePresenter : public Presenter,
                                             public DeviceController
{
    Q_OBJECT
    Q_PROPERTY(QWidget* view READ getWidget)

signals:
    DEVICE_CONTROLLER_SIGNALS

public:
    explicit DevicePresenter(Device* _device,
                             QWidget* _view,
                             bool _auto_read);

    ~DevicePresenter();

    void registerDecorator(AWidgetDecorator* _decorator);

    template <class T>
    T getCastedView() const
    {
        return qobject_cast<T>(getWidget());
    }

    template <class T>
    T getCastedDevice() const
    {
        return qobject_cast<T>(getDevice());
    }

    template <class T>
    bool hasDecorator() const
    {
        T decorator = getDecorator<T>();
        return decorator != NULL;
    }

    template <class T>
    T getDecorator() const
    {
        foreach (AWidgetDecorator* d, decorators_)
        {
            T casted = qobject_cast<T>(d);
            if (casted != NULL)
                return casted;
        }
        return 0;
    }
public slots:
    void update();
    void setForceWrite(bool _fw);
signals:
    void activated();
    void deactivated();
    // DeviceController interface
protected:
    void _detach(Device *_old) override;
    void _device_attached(Device *_device) override;
    bool _need_force_read() const override;
    bool _need_force_write() const final;
protected slots:
    void _modified_by_view();

protected slots:
    void _activated() final;
    void _deactivated() final;

private:
    QList<AWidgetDecorator*> decorators_;
    bool force_write_;
private:
    static QList<DevicePresenter*> all_device_presenters_;

public:
    static DevicePresenter* getPresenter(QWidget* _view);
    template <class T>
    static T getDecorator(QWidget* _view)
    {
        T result = 0;
        DevicePresenter* p = getPresenter(_view);
        if (p)
            result = p->getDecorator<T>();
        return result;
    }

    static QList<DevicePresenter*> getPresentersForDevice(Device* _device);

    template <class T>
    static QList<T> getDecorators(Device* _device)
    {
        QList<T> result;
        QList<DevicePresenter*> presenters = getPresentersForDevice(_device);
        foreach (DevicePresenter* p, presenters)
        {
            T d = p->getDecorator<T>();
            if (d)
                result << d;
        }
        return result;
    }

    static QList<DevicePresenter*> getAllPresenters();
    static DevicePresenter* getActive();

    static Device* getDeviceByView(QWidget* _view)
    {
        DevicePresenter* dp = DevicePresenter::getPresenter(_view);
        if (dp)
            return dp->getDevice();
        return 0;
    }

private:
    static DevicePresenter* active_;

};

template <class DecoratorT>
QList<DecoratorT> getAllDecorators()
{
    QList<DevicePresenter*> all = DevicePresenter::getAllPresenters();
    QList<DecoratorT> result;
    foreach (DevicePresenter* dp, all)
    {
        DecoratorT d = dp->getDecorator<DecoratorT>();
        if (d != 0)
            result << d;
    }

    return result;
}

template <class DecoratorT>
DecoratorT getActiveDecorator()
{
    DecoratorT result = 0;
    DevicePresenter* active = DevicePresenter::getActive();
    if (active)
    {
        result = active->getDecorator<DecoratorT>();
    }
    return result;

}

template <class DecoratorT>
bool decoratorExists(Device* _device)
{
    QList<DevicePresenter*> presenters = DevicePresenter::getPresentersForDevice(_device);
    foreach (DevicePresenter* dp, presenters)
    {
        if (dp->hasDecorator<DecoratorT>())
            return true;
    }

    return false;
}



#endif // DEVICEPRESENTER_BASE_H
