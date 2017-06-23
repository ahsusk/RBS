#ifndef PRESENTER_H
#define PRESENTER_H

#include <QWidget>
#include <rbscore/src/rbscore_global.h>

///
/// Класс презентера представления.
///
/// Презентер - представитель.
/// Этот класс реагирует на изменение фокуса приложения.
/// \sa WidgetsHandler
///
/// Если активирован/деактивирован виджет презентера, вызывается слот
/// _activated/_deactivated
///
///
class RBSCORESHARED_EXPORT Presenter : public QObject
{
    Q_OBJECT
public:
    Presenter(QWidget* _view);
    virtual ~Presenter();
    virtual void init(){}

    QWidget* getWidget() const;

signals:
    void closeRequested(void* _result);
protected slots:
    virtual void _activated(){}
    virtual void _deactivated(){}

private slots:
    void widget_activated(QWidget *_widget);
    void widget_deactivated(QWidget* _widget);

private:
    QWidget* view_;

};

#endif // PRESENTER_H
