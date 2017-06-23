#include "sci_le_decorator.h"

#include "mvc/bind/presenterfactory.h"
#include "mvc/managers/lineendingmanager.h"

#include "Core/src/Serialization/Devices/rbsdocument.h"
#include "core/src/settings/settings.h"
#include "Settings/SyntaxSettings/textviewsettings.h"

DECLARE_SPECIFIC_DECORATOR_FACTORY(RBSDocument,ScintillaView,SciLineEndingDecorator)

SciLineEndingDecorator::SciLineEndingDecorator(ScintillaView* _view)
    : AWidgetDecorator(_view)
{
}


void SciLineEndingDecorator::_do_on_widget_got_focus(Device *)
{
    // если пустой, значит еще не настроили
    if (line_ending_.isEmpty())
    {
        QString text = getScintillaView()->getText();
        line_ending_ = le_from_content(text);
        // если пустой, значит в тексте нет концов строк.
        // берем по умолчанию
        if (line_ending_.isEmpty())
        {
            TextViewSettings* tvs = theSettings()->findChild<TextViewSettings*>();
            line_ending_ = tvs->getLineEnding();
        }
        setLineEnding(line_ending_);
    }
}

void SciLineEndingDecorator::setLineEndingForDocument(const QString &_le)
{
    ScintillaView* scintilla = getScintillaView();
    QString text = scintilla->getText();

    if (_le == "CRLF")
    {
        text.replace("\r\n", "\n");
        text.replace("\n", "\r\n");

    }
    else if (_le == "LF")
    {
        text.replace("\r\n", "\n");

    }
    scintilla->setText(text);

    setLineEnding(_le);
}

QString SciLineEndingDecorator::getLineEnding() const
{
    return line_ending_;
}

void SciLineEndingDecorator::setLineEnding(const QString &_line_ending)
{
    QsciScintilla::EolMode mode = _line_ending == "CRLF"?
                QsciScintilla::EolWindows : QsciScintilla::EolUnix;
    line_ending_ = _line_ending;
    getScintillaView()->setEolMode(mode);
}


QString SciLineEndingDecorator::le_from_content(
        const QString &_data)
{
    if (_data.contains("\r\n"))
        return "CRLF";
    else if (_data.contains("\n"))
        return "LF";
    return line_ending_;
}
