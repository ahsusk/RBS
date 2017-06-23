#ifndef IDECORATORFACTORY_H
#define IDECORATORFACTORY_H

#include <rbscore/src/mvc/base/devicepresenter.h>
#include <rbscore/src/rbscore_global.h>
class AWidgetDecorator;

class RBSCORESHARED_EXPORT ADecoratorFactory : public QObject
{
    Q_OBJECT
public:
    ADecoratorFactory();
    AWidgetDecorator* create(DevicePresenter* _presenter);

    virtual QString getFactoryName() const;

    template<class DecoratorT>
    DecoratorT getActiveDecoratorT() const
    {
        DevicePresenter* active_dp = DevicePresenter::getActive();
        if (active_dp)
        {
            return active_dp->getDecorator<DecoratorT>();
        }

        return 0;
    }

private slots:
    void on_activated();
    void on_deactivated();

protected:
    virtual AWidgetDecorator* _do_create(DevicePresenter* _presenter) = 0;
    virtual void _watch(AWidgetDecorator* ){}
    virtual void _sync(AWidgetDecorator* ){}

protected:

private:
    QList<AWidgetDecorator*> my_decorators_;
};

#define GET_ACTIVE_DECORATOR_FUNC(DecoratorT) \
public:\
    DecoratorT getActiveDecorator() const \
{\
    return ADecoratorFactory::getActiveDecoratorT<DecoratorT>();\
}\


template<class DecoratorT>
class TemplateDecoratorFactory : public ADecoratorFactory
{
    GET_ACTIVE_DECORATOR_FUNC(DecoratorT*)
};

template<class DocT, class ViewT, class DecoratorT>
class SpecificDecoratorFactory : public TemplateDecoratorFactory<DecoratorT>
{
protected:
    typedef SpecificDecoratorFactory<DocT,ViewT,DecoratorT> SuperClass;
    GET_ACTIVE_DECORATOR_FUNC(DecoratorT*)

public:
    QString getFactoryName() const override
    {
        QString result = "SpecificDecoratorFactory < %1, %2, %3 >";
        int doc_type = qMetaTypeId<DocT*>();
        int view_type = qMetaTypeId<ViewT*>();
        int dec_type = qMetaTypeId<DecoratorT*>();
        result = result.arg(QMetaType::typeName(doc_type))
                       .arg(QMetaType::typeName(view_type))
                       .arg(QMetaType::typeName(dec_type));
        return result;
    }

protected:
    DecoratorT* _do_create(DevicePresenter * _presenter)
    {
        DocT* doc = _presenter->getCastedDevice<DocT*>();
        ViewT* view = _presenter->getCastedView<ViewT*>();
        DecoratorT* result = 0;
        if (doc && view)
            result = new DecoratorT(view);
        return result;
    }
};


#endif // IDECORATORFACTORY_H
