#include "sci_text_decorator.h"

SciTextDecorator::SciTextDecorator(QWidget *_view) :
    ATextDecorator(_view)
{
    connect(_view,
            SIGNAL(cursorPositionChanged(int,int)),
            SIGNAL(cursorPositionChanged(int,int)));
}

QString SciTextDecorator::getSelectedText() const
{
    return getScintillaView()->selectedText();
}

void SciTextDecorator::replaceSelectedText(const QString &_text) const
{
    ScintillaView* view = getScintillaView();
    int l_f,l_t,i_f,i_t;
    view->getSelection(&l_f, &i_f, &l_t, &i_t);
    int pp = view->getPositionFromLineIndex(l_f, i_f);
    QString text = _text;
    view->replaceSelectedText(text);
    int pl = pp+_text.toUtf8().length();
    view->getLineIndexFromPosition(pl, &l_t, &i_t);
    view->setSelection(l_f, i_f, l_t, i_t);
}

QList<int> SciTextDecorator::getSelection() const
{
    int line_from, line_to, index_from, index_to;
    getScintillaView()->getSelection(&line_from,
                                  &index_from,
                                  &line_to,
                                  &index_to);

    QList<int> result;
    result<<line_from<<index_from<<line_to<<index_to;

    return result;
}

void SciTextDecorator::setSelection(const QList<int> &_selection) const
{
    if (_selection.count() == 4)
    {
        int line_from, line_to, index_from, index_to;
        line_from = _selection[0];
        index_from = _selection[1];
        line_to = _selection[2];
        index_to = _selection[3];

        getScintillaView()->setSelection(line_from,index_from,line_to,index_to);
    }
}

QPoint SciTextDecorator::getCursorPosition() const
{
    int line,index;
    getScintillaView()->getCursorPosition(&line,&index);
    return QPoint(line,index);
}

void SciTextDecorator::setCursorPosition(int _line, int _index) const
{
    ScintillaView* view = getScintillaView();
    view->setCursorPosition(_line,_index);
    view->ensureLineVisible(_line);
}

QString SciTextDecorator::getLine(int _line) const
{
    QString result = getScintillaView()->text(_line);
    return result;
}

void SciTextDecorator::cut() const
{
    ScintillaView* sv = getScintillaView();
    qDebug()<<"cut in scintilla"<<sv;
    sv->cut();
}

void SciTextDecorator::copy() const
{
    getScintillaView()->copy();
}

void SciTextDecorator::paste() const
{
    getScintillaView()->paste();
}

void SciTextDecorator::undo() const
{
    getScintillaView()->undo();
}

void SciTextDecorator::redo() const
{
    ScintillaView* sv = getScintillaView();
    qDebug()<<"redo in scintilla"<<sv;
    sv->redo();
}

void SciTextDecorator::ensureLineVisible(int _line) const
{
    getScintillaView()->ensureLineVisible(_line);
}
