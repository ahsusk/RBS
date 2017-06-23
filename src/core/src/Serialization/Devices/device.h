#ifndef DEVICE_H
#define DEVICE_H

#include <core/src/core_global.h>
#include <QIODevice>
#include <QDir>
#include <QUrl>

class ADeviceImplementor;
///
/// \brief Абстрактный класс устройства.
///
/// Работа с документами, логами, файловыми элементами дерева проектов
/// происходит через этот класс.
///
/// Устройство наследует класс QIODevice, но его основные функции не
/// реализует. Так, можно сделать устройство с буфером
/// (BufferedDeviceImplementor), или устройство, напрямую работающее с файлом
/// (DirectlyDeviceImplementor)
///
/// Объект класса хранит указатель на реальное устройство(файл) \c real_device_,
/// но всё управление записью и чтением должны выполнять дочерние классы.
///
///
class CORESHARED_EXPORT Device : public QIODevice
{
    Q_OBJECT
    Q_PROPERTY(QUrl      url          READ getUrl)
    Q_PROPERTY(QString   fileName     READ getFileName)
    Q_PROPERTY(QString   absolutePath READ getAbsolutePath USER true)
    Q_PROPERTY(QDir      directory    READ getDirectory)
    Q_PROPERTY(QString   absoluteDir  READ getAbsoluteDir)
    Q_PROPERTY(QFileInfo fileInfo     READ getFileInfo)
    Q_PROPERTY(bool      modified     READ isModified
               NOTIFY modificationChanged)

protected:
    enum State
    {
        Default,
        Saving,
        Loading
    };

public:
    ~Device();
    ///
    /// \brief open Перегруженная функция открытия устройства. Если устройство
    /// открывается на чтение, шлет сигнал \ref needActualData
    ///
    bool open(OpenMode _open_mode) override;
    void close();
    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
protected:
    explicit Device(ADeviceImplementor* _impl);
    State _get_state() const;
public slots:
    QUrl getUrl() const;
    QString getFileName() const;
    QDir getDirectory() const ;
    QString getAbsoluteDir() const ;
    QFileInfo getFileInfo() const ;
    QString getAbsolutePath() const ;
    bool isModified() const;

public slots:
    ///
    /// \brief save Функция сохранения устройства.
    /// Если устройство не модифицировано, ничего не делает.
    ///
    /// Если модифицировано, вызывает _do_save
    ///
    bool save();

    ///
    /// \brief load Функция загрузки устройства. Вызывает _do_load
    ///
    bool load();

    ///
    /// \brief modify Функция установки флага модификации. Именно установки.
    /// Снять его можно только в приватной функции set_modified, которая
    /// вызывается при загрузке/сохранении данных.
    /// И НИКОМУ БОЛЬШЕ НЕЛЬЗЯ ЕЕ ВЫЗЫВАТЬ!!!
    ///
    void modify();

    ///
    /// \brief getData Функция получения данных устройства.
    /// Открывает себя, читает все данные, закрывает себя, возвращает данные.
    ///
    QByteArray getData();


public slots:
    ///
    /// \brief needAskForSaving Функция проверки, надо ли спрашивать о сохранении
    /// устройства при закрытии RBS или просто при удалении контроллеров устройства.
    /// Для временных файлов (\ref TempDevice) ничего не спрашивается.
    ///
    bool needAskForSaving();

    void setAlwaysSave(bool _as);

    bool fileIsOpen(OpenMode _mode) const;
signals:
    ///
    /// \brief modificationChanged Сигнал об изменении флага модификации.
    /// Шлется только в том случае, если флаг реально изменился.
    ///
    void modificationChanged(bool _modified);

    void rbsOpenFile();
    void rbsWriteToFile();
    void rbsCloseFile();
    void dataChanged();

    ///
    /// \brief aboutToBeSaved Сигнал о том, что данные из буфера будут записаны на диск.
    ///
    void aboutToBeSaved();
    ///
    /// \brief aboutToBeLoaded Сигнал о том, что данные будут загружены с диска.
    ///
    void aboutToBeLoaded();
    ///
    /// \brief saved Сигнал о том, что данные были сохранены.
    ///
    void saved();
    ///
    /// \brief loaded Сигнал о том, что данные были загружены.
    ///
    void loaded();
    ///
    /// \brief needActualData Сигнал о том, что нужны актуальные данные.
    ///
    void needActualData();

private:
    void set_modified(bool _modified);

private:
    ADeviceImplementor* impl_;
    bool modified_;
    State state_;

    bool always_save_;


};


#include <QDebug>
CORESHARED_EXPORT QByteArray toUtf8(const Device* _dw);
CORESHARED_EXPORT QDebug operator<<(QDebug dbg, const Device* _dw);

#endif // DEVICE_H
