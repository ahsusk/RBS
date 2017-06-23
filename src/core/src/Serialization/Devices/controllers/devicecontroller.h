#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <core/src/core_global.h>
#include <QCoreApplication>
#include "devicecontrollerobject.h"

#define DEVICE_CONTROLLER_SIGNALS \
    void detached(Device* _d) override;\
    void attached(Device* _d) override;

class Device;
///
/// Класс контроллера устройства
///
/// Для отображения данных устройства в каком-либо виде с возможностью
/// синхронизации необходимы контроллеры устройств.
///
/// Контроллер устройства следит за изменениями в данных и грамотно синхронизирует
/// несколько контроллеров между собой.
///
/// В классе присутствует список всех созданных контроллеров для всех устройств
/// (\c all_controllers_)
///
/// Для каждого контроллера создается объект контроллера (DeviceControllerObject)
///
/// Эти классы знают друг про друга и сильно друг от друга зависят.
/// Такой прием был необходим, т.к. ромбовидное наследование от QObject запрещено.
/// А все контроллеры рано или поздно QObject'ы.
///
class CORESHARED_EXPORT DeviceController
{
    Q_DECLARE_TR_FUNCTIONS(DeviceController)
    friend class DeviceControllerObject;
public:
    ///
    /// \brief Конструктор
    /// \param _device Устройство
    /// \param _auto_read Необходимость читать из устройства сразу.
    ///
    /// Текстовые редакторы читают сразу, а элементы дерева проектов - нет.
    DeviceController(Device* _device, bool _auto_read = true);
    virtual ~DeviceController();

    ///
    /// \brief Функция подключения устройства к контроллеру.
    ///
    /// Необходима при замене устройства в контроллере.
    /// То есть ровно в двух случаях:
    /// 1) Сохранить как
    /// 2) Создание проекта через мастер проектов (по шаблону, чтобы
    /// скопировать шаблон)
    /// \param _device Устройство
    /// \param _auto_read Необходимость читать из устройства сразу
    ///
    void attach(Device* _device, bool _auto_read);

    ///
    /// Функция получения устройства
    ///
    Device* getDevice() const;

    ///
    /// \brief setState
    /// \param _state
    /// \return
    ///
    bool setState(DeviceState _state);

    bool hasOtherControllers() const;
    virtual void connectModification() = 0;
    virtual void disconnectModification() = 0;

    // сделать сигналом
    virtual void detached(Device* _device) = 0;
    virtual void attached(Device* _device) = 0;
protected:
    virtual bool _do_write() = 0;
    virtual bool _do_read() = 0;

    virtual void _device_attached(Device* _device) = 0;
    virtual void _detach(Device*) {}
    virtual bool _need_force_read() const { return false; }
    virtual bool _need_force_write() const { return false; }
    void _init();

private:

    void __clear();
private:
    Device* device_;
    DeviceControllerObject* obj_;

    static QList<DeviceController*> all_controllers_;

public:
    static QList<DeviceController*> getControllers(Device *_w);
};

#endif // DEVICECONTROLLER_H
