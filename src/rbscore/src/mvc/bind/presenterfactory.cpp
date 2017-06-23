#include "presenterfactory.h"
#include "rbsapplication.h"
#include "Utils/src/metaobjectfactory.h"
#include "rbscore/src/mvc/bind/adecoratorfactory.h"
#include "rbscore/src/mvc/base/devicepresenter.h"
#include "rbscore/src/mvc/managers/devicepresentermanager.h"
#include "idevicepresenterfactory.h"
#include <logging.h>

#include "jstools/src/jsenginepool.h"
PresenterFactory::PresenterFactory()
{

}

PresenterFactory::~PresenterFactory()
{
    qDeleteAll(dp_factories_);
    dp_factories_.clear();
    qDeleteAll(decor_factories_);
    decor_factories_.clear();
}

DevicePresenter *PresenterFactory::initPresenter(Device *_device,
                                                 const QString &_classname,
                                                 bool _auto_read)
{
    QObject* view_obj = theMetaObjectFactory()->create(_classname);
    QWidget* view = qobject_cast<QWidget*>(view_obj);
    if (view)
    {
        view->setParent(mainWnd);
        view->setObjectName(_device->getAbsolutePath());

        qDebug(SUBMIC_LOG)<<"Создан редактор"<<view;
    }

    DevicePresenter* result = 0;
    foreach (IDevicePresenterFactory* f, dp_factories_)
    {
        result = f->initPresenter(_device,view,_auto_read);
        if (result) break;
    }

    if (!result)
    {
        qWarning()<<tr("Не могу создать презентер для документа %1 "
                       "с типом представления %2.").arg(_device->objectName())
                                                   .arg(_classname);
    }
    else
    {
        result->setObjectName(view->objectName());
        connect(view,
                &QWidget::objectNameChanged,
                [=](const QString& _name)
        {
            result->setObjectName(_name);
        });



        foreach (ADecoratorFactory* df, decor_factories_)
        {
            df->create(result);
        }

        connect(result,
                SIGNAL(closeRequested(void*)),
                theDevicePresenterManager(),
                SLOT(onViewCloseRequested(void*)));

        view->setWindowTitle(_device->getFileName());

        qDebug(SUBMIC_LOG)<<"Создан презентер"<<result;
        emit devicePresenterCreated(result);
    }



    return result;

}

void PresenterFactory::registerDPFactory(IDevicePresenterFactory *_factory)
{
    dp_factories_.append(_factory);
}

void PresenterFactory::registerDecoratorFactory(ADecoratorFactory *_factory)
{
    qDebug(SUBMIC_LOG)
            <<"Зарегистрирована фабрика декораторов"
           <<_factory->getFactoryName();

    decor_factories_.append(_factory);
}


PresenterFactory* thePresenterFactory()
{
    return PresenterFactory::instancePtr();
}


