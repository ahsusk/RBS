#include "sci_lexer_decorator.h"

#include <Qsci/qsciscintilla.h>
#include "Settings/SyntaxSettings/textviewsettings.h"
#include <Qsci/qscilexer.h>
#include "Core/src/Serialization/Devices/rbsdocument.h"
#include <QDebug>
#include "mvc/managers/lexersmanager.h"
#include "mvc/views/scintillaview.h"
#include "core/src/settings/settings.h"
#include "Settings/SyntaxSettings/extensionssettings.h"
#include "Settings/SyntaxSettings/lexerssettings.h"
#include <logging.h>

// хорошие скобки как в qt в светлой теме
static const QColor matched_back(180,235,180);
static const QColor matched_font(0xff,0,0);

static const QColor unmatched_back(255,0,0);
static const QColor unmatched_font(0,0,0);


SciLexerDecorator::SciLexerDecorator(ScintillaView *_view) :
    ASyntaxDecorator(_view)
{
    TextViewSettings* tvs = theSettings()->findChild<TextViewSettings*>();
    int tab_width = tvs->getTabWidth();
    int indt_width = tvs->getIndentation();
    bool use_tabs = tvs->getUseTabs();
    setTabWidth(tab_width);
    setIndentation(indt_width);
    setUseTabs(use_tabs);

    bool show_line_numbers = tvs->getShowLineNumbers();
    setShowLineNumbers(show_line_numbers);

    QString line_enging = tvs->getLineEnding();
    setLineEnding(line_enging);

    bool show_ws = tvs->getShowWhiteSpaces();
    setShowWhiteSpaces(show_ws);

    bool show_el = tvs->getShowEdgeLine();
    setShowEdgeLine(show_el);

    int edge_col = tvs->getEdgeColumn();
    setEdgeColumn(edge_col);

    setLexer(0);

}/*

void SciLexerDecorator::setActiveLexer(QsciLexer *_lexer)
{
    SciLexerDecorator* active = dynamic_cast<SciLexerDecorator*>(ASyntaxPresenter::getActive());
    if (active != NULL)
        active->setLexer(_lexer);
}

QsciLexer *SciLexerDecorator::getActiveLexer()
{
    QsciLexer* result = 0;
    SciLexerDecorator* active = dynamic_cast<SciLexerDecorator*>(ASyntaxPresenter::getActive());
    if (active != NULL)
        result = active->getLexer();
    return result;
}*/

void SciLexerDecorator::setFont(const QFont &_font) const
{
    getScintillaView()->setFont(_font);
}

void SciLexerDecorator::setColor(const QColor &_color) const
{
    getScintillaView()->setColor(_color);
    if (getLexer() ==0)
        set_caret_color(_color);
}

void SciLexerDecorator::setPaper(const QColor &_paper) const
{
    getScintillaView()->setPaper(_paper);
}

void SciLexerDecorator::setTabWidth(int _tab_width) const
{
    getScintillaView()->setTabWidth(_tab_width);
}

void SciLexerDecorator::setIndentation(int _indentation) const
{
    getScintillaView()->setIndentationWidth(_indentation);
}

void SciLexerDecorator::setUseTabs(bool _use_tabs) const
{
    getScintillaView()->setIndentationsUseTabs(_use_tabs);
}

void SciLexerDecorator::setShowLineNumbers(bool _show_numbers) const
{
    getScintillaView()->setMarginLineNumbers(ScintillaView::RBS_NUMBERS_MARGIN,
                                             _show_numbers);
    if (_show_numbers)
    {
        getScintillaView()->setMarginWidth(ScintillaView::RBS_NUMBERS_MARGIN,35);
    }
    else
    {
        getScintillaView()->setMarginWidth(ScintillaView::RBS_NUMBERS_MARGIN,0);
    }
}

void SciLexerDecorator::setShowWhiteSpaces(bool _show_whitespace) const
{
    QsciScintilla::WhitespaceVisibility visibility = _show_whitespace?
                QsciScintilla::WsVisible:QsciScintilla::WsInvisible;

    getScintillaView()->setWhitespaceVisibility(visibility);
    getScintillaView()->setEolVisibility(_show_whitespace);
}

void SciLexerDecorator::setLineEnding(const QString &_line_ending) const
{
    QsciScintilla::EolMode mode =
            _line_ending=="LF"?QsciScintilla::EolUnix:QsciScintilla::EolWindows;
    getScintillaView()->setEolMode(mode);
}

void SciLexerDecorator::setShowEdgeLine(bool _sel) const
{
    getScintillaView()->SendScintilla(
                QsciScintilla::SCI_SETEDGEMODE,
                _sel ? QsciScintilla::EdgeLine : QsciScintilla::EdgeNone);
}

void SciLexerDecorator::setEdgeColumn(int _column) const
{
    getScintillaView()->SendScintilla(QsciScintilla::SCI_SETEDGECOLUMN, _column);
}

void SciLexerDecorator::_set_device_info(Device *_device)
{
    ExtensionsSettings* es = theSettings()->findChild<ExtensionsSettings*>();
    if (!es)
    {
        qWarning()<<"не найдены настройки расширений";
        return;
    }

    LexersSettings* ls = theSettings()->findChild<LexersSettings*>();
    if (!ls)
    {
        qWarning()<<"не найдены настройки лексеров";
        return;
    }

    QFileInfo finfo = _device->getFileInfo();
    QString suffix = finfo.suffix();
    QString lang = es->getLanguage(suffix);

    QsciLexer* lexer = ls->getLexer(lang);
    qDebug(SUBMIC_LOG)<<"Установка лексера"<<lang<<"для"<<finfo.absoluteFilePath();
    setLexer(lexer);
}

void SciLexerDecorator::set_caret_color(const QColor &_color, int _style) const
{
    if (_style == 0)
    {
        getScintillaView()->setCaretForegroundColor(_color);
    }
}

void SciLexerDecorator::setLexer(QsciLexer *_lexer)
{
    ScintillaView* view = getScintillaView();
    QsciLexer* lexer = getLexer();

    if (lexer != NULL)
    {
        disconnect(lexer,NULL,this,NULL);
    }



    view->setLexer(_lexer);

    if (_lexer == NULL)
        set_default_settings();
    else
    {
        QColor caret_color = _lexer->color(0);
        set_caret_color(caret_color);
        connect(_lexer,
                SIGNAL(colorChanged(QColor,int)),
                SLOT(set_caret_color(QColor,int)));
    }

    // при замене лексера, меняются эти цвета. хотя
    // своих стилей у них нет. поэтому каждый раз
    // устанавливаем их снова.
    view->setMatchedBraceBackgroundColor(matched_back);
    view->setMatchedBraceForegroundColor(matched_font);
    view->setUnmatchedBraceBackgroundColor(unmatched_back);
    view->setUnmatchedBraceForegroundColor(unmatched_font);

    view->setBraceMatching(QsciScintilla::SloppyBraceMatch);
}

QsciLexer *SciLexerDecorator::getLexer() const
{
    QsciScintilla* sci = getScintillaView();
    QsciLexer* lexer = sci->lexer();
    return lexer;
}

//void SciLexerDecorator::_deactivated()
//{
//    ScintillaView* view = getScintillaView();
//    if (view != NULL) // представление будет нулевое, если _deactivated
//                        // вызван после destroyed
//        view->setCaretWidth(0);
//}

//void SciLexerDecorator::_activated()
//{
//    getScintillaView()->setCaretWidth(2);
////    qDebug()<<"change_current_lexer();"<<(lex?lex->language():"default");

//}

void SciLexerDecorator::set_default_settings()
{
    TextViewSettings* tvs = theSettings()->findChild<TextViewSettings*>();
    QFont font = tvs->getFont();
    QColor color = tvs->getColor();
    QColor paper = tvs->getPaper();

    setFont(font);
    setColor(color);
    setPaper(paper);

    getScintillaView()->setCaretForegroundColor(color);
}
