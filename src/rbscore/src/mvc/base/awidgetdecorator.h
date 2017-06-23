#ifndef A_WIDGET_DECORATOR_H
#define A_WIDGET_DECORATOR_H
#include <QWidget>
#include "core/src/Serialization/Devices/device.h"

///
/// Класс декоратора представления
///
/// Этот класс необходим для добавления особенностей к отображению редакторов
/// \sa \ref controllers
///
class AWidgetDecorator : public QObject
{
    Q_OBJECT
public:
    AWidgetDecorator(QWidget* _view);

    ///
    /// Функция на получения фокуса редактором.
    /// Генерит сигнал activated() и вызывает виртуальную функцию
    /// _do_on_widget_got_focus
    ///
    void onWidgetGotFocus(Device* _device);

    ///
    /// Функция на потерю фокуса редактором.
    /// Генерит сигнал deactivated() и вызывает виртуальную функцию
    /// _do_on_widget_lost_focus
    ///
    void onWidgetLostFocus(Device* _device);

public slots:
    ///
    /// Функция для установки данных из устройства в представление.
    /// Например, написать название файла на вкладке.
    ///
    /// Функция сохраняет устройство в \c device_
    /// и вызывает виртуальную функцию _set_device_info
    ///
    void decorate(Device* _device);

    ///
    /// Функция получения виджета
    ///
    QWidget* getView() const;

signals:
    void activated();
    void deactivated();

protected:
    virtual void _do_on_widget_got_focus(Device* ){}
    virtual void _do_on_widget_lost_focus(Device*){}
    virtual void _set_device_info(Device*){}
protected slots:
    ///
    /// Функция для обновления редактора. вызывает _set_device_info с
    /// сохраненным устройством
    ///
    void _reset();

private:
    QWidget* view_;
    Device* device_;
};

#define CASTED_VIEW_GETTER(T) \
public: \
    T* get##T() const { return qobject_cast<T*>(getView()); }

#endif // A_WIDGET_DECORATOR_H
