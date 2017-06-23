#ifndef DEVICEMAP_H
#define DEVICEMAP_H
#include <QObject>
#include <QMap>
#include <utils/src/MyersSingleton.h>
#include <core/src/core_global.h>
#include <QFileSystemWatcher>
class Device;
class RealDevice;
class DeviceWrapperFactory;
///
/// Класс с картой устройств
///
/// Здесь хранятся все устройства, созданные в RBS
///
/// Для получения устройства
///
///
class CORESHARED_EXPORT DeviceMap : public QObject,
        public MyersSingleton<DeviceMap>
{
    Q_OBJECT
    DeviceMap();
    friend class MyersSingleton<DeviceMap>;
    friend class Device;
public:

    template <class T>
    T getCastedDevice(const QString& _path)
    {
        return qobject_cast<T>(getDevice(_path));
    }

    Device* getDevice(const QString& _path);
    QList<Device*> getDevices() const;
    void remove(Device* _device);
    void remove(const QString& _path);
    bool contains(const QString& _path) const;

signals:
    void deviceMapped(Device* _device);
    void removeDevice(Device* _device);
    void deviceRemoved(const QString& _path);
private slots:
    void device_detroyed(QObject* _device);

    void __push_me(Device* _device);

private:
    QString get_clean_path(const QString& _path) const;
    Device* value(const QString& _path) const;
private:
    QMap<QString,Device*> map_;


};

CORESHARED_EXPORT SM_MYERS_SINGLETON_DECL(DeviceMap)

#endif // DEVICEMAP_H
