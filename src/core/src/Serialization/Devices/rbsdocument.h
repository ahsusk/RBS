#ifndef RBSDOCUMENT_H
#define RBSDOCUMENT_H

#include "core/src/core_global.h"
#include "device.h"

///
/// Класс документа RBS
///
/// Документ - это устройство, у которого есть кодировка.
/// Данные документа можно получить и установить в виде строки QString.
/// При получении данные конвертируются в установленную кодировку из массива
/// данных устройства.
/// При установке данных строка \c _text конвертируется в массив данных с
/// помощью установленной кодировки.
///
/// Функция getText не является константной, т.к. меняет состояние устройства
/// на "чтение".
///
/// Изменение кодировки не влечет за собой модификацию документа, так как
/// "сырые" данные при этом остаются те же, просто меняется их отображение.
///
/// При конвертировании в другую кодировку необходимо сохранить копию данных
/// в виде строки QString, установить новую кодировку с помощью \c setEncoding,
/// и установить сохраненные данные с помощью \c setText.
///
/// \sa EncodingManager, EncodingToolBar
class CORESHARED_EXPORT RBSDocument : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString encoding
               READ getEncoding
               WRITE setEncoding
               NOTIFY encodingChanged)

    Q_PROPERTY(QString text
               READ getText
               WRITE setText)
public:
    explicit RBSDocument(ADeviceImplementor *_impl,
                         const QString& _encoding);
    ~RBSDocument();

public slots:
    QString getText();
    void setText(const QString& _text);
    void setEncoding(const QString& _encoding);
    QString getEncoding() const;


signals:
    void encodingChanged(const QString& _encoding);

private:
    QString encoding_;

};

#endif // RBSDOCUMENT_H
