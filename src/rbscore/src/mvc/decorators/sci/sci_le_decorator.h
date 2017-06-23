#ifndef SciLineEndingDecorator_H
#define SciLineEndingDecorator_H

#include <QObject>
#include "mvc/base/awidgetdecorator.h"
#include "mvc/views/scintillaview.h"
#include "Core/src/Serialization/Devices/rbsdocument.h"

Q_DECLARE_METATYPE(QsciScintilla::EolMode)


/**
 * @brief The SciLineEndingDecorator class Класс презентера для управления
 * символами концов строк в документе.
 *
 * Данный класс осуществляет синхронизацию символов последовательности конца
 * строки между документом, представлением документа и ЭУ концами строк.
 *
  */
class SciLineEndingDecorator : public AWidgetDecorator
{
    Q_OBJECT
public:
    SciLineEndingDecorator(ScintillaView *_view);

    CASTED_VIEW_GETTER(ScintillaView)

protected:
    void _do_on_widget_got_focus(Device *) final;
signals:

    /**
     * @brief lineEndingChanged сигнал, генерируемй для представления документа.
     *
     * @param _m - числовой идентификатор конца строки.
     */
    void lineEndingChanged(QsciScintilla::EolMode _m);
public slots:

    /**
     * @brief setLineEndingForDocument метод изменения последовательности
     * конца строки.
     *
     * Генерирует сигналы lineEndingChanged
     */
    void setLineEndingForDocument(const QString& _le);


    QString getLineEnding() const;

private:
    void setLineEnding(const QString &_line_ending);
    QString le_from_content(const QString &_data);

private:
    QString line_ending_;


};



#endif // SciLineEndingDecorator_H
