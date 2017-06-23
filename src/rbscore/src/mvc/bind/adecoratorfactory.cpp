#include "adecoratorfactory.h"

ADecoratorFactory::ADecoratorFactory()
{

}

AWidgetDecorator*  ADecoratorFactory::create(DevicePresenter *_presenter)
{
    AWidgetDecorator* decorator = _do_create(_presenter);
    if (decorator)
    {
        _presenter->registerDecorator(decorator);

        connect(decorator,
                SIGNAL(activated()),
                SLOT(on_activated()));
        connect(decorator,
                SIGNAL(deactivated()),
                SLOT(on_deactivated()));
    }

    return decorator;
}

QString ADecoratorFactory::getFactoryName() const
{
    return metaObject()->className();
}

void ADecoratorFactory::on_activated()
{
    AWidgetDecorator* wd = qobject_cast<AWidgetDecorator*>(sender());
    _watch(wd);
}

void ADecoratorFactory::on_deactivated()
{
    AWidgetDecorator* wd = qobject_cast<AWidgetDecorator*>(sender());
    _sync(wd);
}

