#include "regexpsearchdecorator.h"

RegExpSearchDecorator::RegExpSearchDecorator(QWidget *_view) :
    ASearchDecorator(_view)
{

}

void RegExpSearchDecorator::_set_settings(const QString &_expression,
                                          bool _reg_exp,
                                          bool _whole_word,
                                          bool _case_sensitive)
{
    QRegExp::PatternSyntax ps = QRegExp::RegExp2;
    QString pattern = _expression;
    if (!_reg_exp && !_whole_word)
        ps = QRegExp::FixedString;
    else if (!_reg_exp)
        pattern = QRegExp::escape(pattern);

    Qt::CaseSensitivity cs = _case_sensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if (_whole_word)
    {
        pattern.prepend("\\b");
        pattern.append("\\b");
    }
    search_rx_ = QRegExp(pattern, cs, ps);
}

bool RegExpSearchDecorator::_check_selection()
{
    QString selected = _get_selected_text();
    return search_rx_.exactMatch(selected);
}

ASearchDecorator::SearchResult RegExpSearchDecorator::_run_search(bool _forward, bool _left)
{
    QString text = _get_text();
    int offset = _get_current_position(_forward, _left);
    if (offset == -1)
        offset = 0;

    int index = search(text,offset,_forward);


    SearchResult result (-1,-1);
    if (index != -1)
    {
        int length = search_rx_.matchedLength();
        result = { index, length };
    }
    qDebug()<<"search"<<search_rx_.pattern()<<"from"<<offset<<result;
    return result;
}

int RegExpSearchDecorator::search(const QString& _text,
                                  int _offset,
                                  bool _forward) const
{
    int index = -1;
    int offset = _offset;
    if (offset == -1)
    {
        if (_forward)
            offset = 0;
        else
            offset = _text.length();
    }

    if (_forward)
        index = search_rx_.indexIn(_text,offset);
    else
        index = search_rx_.lastIndexIn(_text,offset);

    if (index == -1 && _offset != -1)
    {
        index = search(_text,-1,_forward);
    }

    return index;

}


