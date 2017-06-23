#ifndef PRESENTERFACTORY_H
#define PRESENTERFACTORY_H

#include <QObject>
#include "Utils/src/MyersSingleton.h"

#include "rbscore/src/rbscore_global.h"
class DevicePresenter;
class Device;
class IDevicePresenterFactory;
class ADecoratorFactory;
class RBSCORESHARED_EXPORT PresenterFactory :
        public QObject,
        public MyersSingleton<PresenterFactory>
{
    Q_OBJECT
    friend class MyersSingleton<PresenterFactory>;
    PresenterFactory();
public:
    ~PresenterFactory();
    template <class T>
    T findDecoratorFactory()
    {
        foreach (ADecoratorFactory* df, decor_factories_)
        {
            T casted = qobject_cast<T>(df);
            if (casted != 0)
                return casted;
        }
        return 0;
    }

signals:
    void devicePresenterCreated(DevicePresenter* _presenter);

public slots:
    DevicePresenter* initPresenter(Device* _device,
                                   const QString& _classname,
                                   bool _auto_read = true);

    void registerDPFactory(IDevicePresenterFactory* _factory);
    void registerDecoratorFactory(ADecoratorFactory* _factory);
private:
    QList<IDevicePresenterFactory*> dp_factories_;
    QList<ADecoratorFactory*> decor_factories_;

};

RBSCORESHARED_EXPORT PresenterFactory* thePresenterFactory();
template <class T>
T findDecoratorFactory()
{
    return thePresenterFactory()->findDecoratorFactory<T>();
}

#include <rbscore/src/mvc/bind/adecoratorfactory.h>

template<class F>
class StaticFactoryRegister
{
public:
    StaticFactoryRegister()
    {
        thePresenterFactory()->registerDecoratorFactory(new F);
    }
private:
    static StaticFactoryRegister<F> r;
};

#define DECLARE_DECORATOR_FACTORY(T) \
    template<>\
    StaticFactoryRegister<T> StaticFactoryRegister<T>::r;

#define DECLARE_SPECIFIC_DECORATOR_FACTORY(DocT, ViewT, DecoratorT) \
    template<>\
    StaticFactoryRegister<SpecificDecoratorFactory<DocT,ViewT,DecoratorT> >\
    StaticFactoryRegister<SpecificDecoratorFactory<DocT,ViewT,DecoratorT> >::r;

#endif // PRESENTERFACTORY_H
