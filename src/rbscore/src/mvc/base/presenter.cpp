#include "presenter.h"
#include "src/mvc/managers/widgetshandler.h"
#include <logging.h>
Presenter::Presenter(QWidget *_view) :
    QObject(_view),
    view_(_view)
{
    connect(theWidgetsHandler(),
            SIGNAL(activated(QWidget*)),
            SLOT(widget_activated(QWidget*)),
            Qt::DirectConnection);

    connect(theWidgetsHandler(),
            SIGNAL(deactivated(QWidget*)),
            SLOT(widget_deactivated(QWidget*)),
            Qt::DirectConnection);

    connect(this,
            &Presenter::destroyed,
            [=]()
    {
        qDebug(SUBMIC_LOG)<<"Презентер уничтожен. Закрываем представление"<<_view;
        _view->close();
    });
}

Presenter::~Presenter()
{
    qDebug(SUBMIC_LOG)<<this<<"уничтожен";
}

QWidget *Presenter::getWidget() const
{
    return view_;
}

void Presenter::widget_activated(QWidget *_widget)
{
    if (_widget == view_)
    {
        _activated();
    }
}

void Presenter::widget_deactivated(QWidget *_widget)
{
    if (_widget == view_)
    {
        _deactivated();
    }
}
