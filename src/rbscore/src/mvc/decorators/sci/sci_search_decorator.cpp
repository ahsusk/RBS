#include "sci_search_decorator.h"

ScintillaSearchDecorator::ScintillaSearchDecorator(QWidget *_view) :
    RegExpSearchDecorator(_view)
{

}

void ScintillaSearchDecorator::_select(int _from, int _cnt)
{
    ScintillaView* view = getScintillaView();
    int line_from, index_from, line_to, index_to;
    view->getLineIndexFromPosition(_from, &line_from, &index_from);
    view->getLineIndexFromPosition(_from + _cnt, &line_to, &index_to);

    view->setSelection(line_from, index_from, line_to, index_to);
}


void ScintillaSearchDecorator::_replace(const QString &_rplc)
{
    ScintillaView* view = getScintillaView();
    int line_from, index_from, line_to, index_to;
    view->getSelection(&line_from, &index_from, &line_to, &index_to);
    int length = _rplc.length();

    int begin = view->getPositionFromLineIndex(line_from, index_from);
    view->getLineIndexFromPosition(begin+length, &line_to, &index_to);

    view->replaceSelectedText(_rplc);

    view->setSelection(line_from,index_from, line_to,index_to);
}


QString ScintillaSearchDecorator::_get_text() const
{
    return getScintillaView()->getText();
}

QString ScintillaSearchDecorator::_get_selected_text() const
{
    return getScintillaView()->selectedText();
}

int ScintillaSearchDecorator::_get_current_position(bool _forward, bool _left) const
{
    ScintillaView* view = getScintillaView();

    int line, index;

    if (view->hasSelectedText())
    {
        int line_from, index_from, line_to, index_to;
        view->getSelection(&line_from, &index_from, &line_to, &index_to);

        if (_forward && !_left)
        {
            line = line_to;
            index = index_to;
        }
        else
        {
            line = line_from;
            index = index_from;
        }
    }
    else
    {
        view->getCursorPosition(&line, &index);
    }

    int offset = view->getPositionFromLineIndex(line,index);
    return offset;
}
