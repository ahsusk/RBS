#ifndef DEVICECONTROLLEROBJECT_H
#define DEVICECONTROLLEROBJECT_H

#include <QObject>
class Device;
class DeviceController;

enum DeviceState
{
    Default,
    PreRead,
    PreWrite,
    Read,
    Write
};
Q_DECLARE_METATYPE(DeviceState)

class DeviceControllerObject : public QObject
{
    Q_OBJECT

public:
    explicit DeviceControllerObject(DeviceController* _controller,
                                    Device* _device,
                                    bool _auto_read = true);


    bool setState(DeviceState _state);

public slots:
    ///
    /// \brief Функция инициализации объекта. На самом деле просто делает
    /// красивый objectName. Чтобы в логах было понятно что происходит.
    /// Вызывается из класса DeviceController. Больше нигде ее использовать
    /// не нужно.
    ///
    void init();

signals:
    void stateChanged(DeviceState _old, DeviceState _new);
    void wrote();

private slots:
    void connect_sibling(DeviceControllerObject* _sibling);
    void on_other_state_changed(DeviceState _old, DeviceState _new);
    void on_device_destroyed();
    void on_need_actual_data();
    void force_read();
private slots:
    void do_write();
    void do_read();
private:
    bool can_set_state(DeviceState _state);
private:
    DeviceState state_;
    DeviceController* controller_;
    Device* device_;
};

#endif // DEVICECONTROLLEROBJECT_H
