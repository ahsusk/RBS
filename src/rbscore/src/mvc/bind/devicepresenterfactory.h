#ifndef DEVICEPRESENTERFACTORY_H
#define DEVICEPRESENTERFACTORY_H

#include <QObject>
#include <QWidget>

#include "Utils/src/StaticChildAdder.h"
#include "idevicepresenterfactory.h"

#include "presenterfactory.h"

template <class ViewT, class DocumentT, class PresenterT>
class DevicePresenterFactory : public IDevicePresenterFactory
{
public:
    DevicePresenterFactory()
    {
        qRegisterMetaType<ViewT*>();
        thePresenterFactory()->registerDPFactory(this);
    }
    // IDevicePresenterFactory interface
public:
    DevicePresenter *initPresenter(Device *_dev,
                                   QWidget *_view,
                                   bool _auto_read) override
    {
        DocumentT* doc = qobject_cast<DocumentT*>(_dev);
        ViewT* view = qobject_cast<ViewT*>(_view);
        DevicePresenter* result = 0;
        if (doc != NULL && view != NULL)
        {
            result = new PresenterT(doc,view,_auto_read);
            result->init();
        }
        return result;
    }
};

#endif // DEVICEPRESENTERFACTORY_H
