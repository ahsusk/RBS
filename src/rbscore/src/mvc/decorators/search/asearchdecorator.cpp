#include "asearchdecorator.h"

ASearchDecorator::ASearchDecorator(QWidget *_view) :
    AWidgetDecorator(_view)
{

}


bool ASearchDecorator::find(const QString &_expression,
                            bool _reg_exp,
                            bool _whole_word,
                            bool _case_sensitive,
                            bool _forward,
                            bool _left)
{
//    qDebug()<<"set settings"<<_expression << _reg_exp<<_whole_word<<_case_sensitive;

    // Эта функция установит настройки поиска примерно так
    _set_settings(_expression, _reg_exp, _whole_word, _case_sensitive);
    // Затем запустит поиск по тексту, который вернет Selection,
    // который необходимо выделить
    SearchResult result = _run_search(_forward, _left);

//    qDebug()<<"result of search"<<result;
    if (result.first != -1)
    {
        _select(result.first, result.second);
        return true;
    }

    return false;

}

bool ASearchDecorator::replace(const QString &_expr,
                               const QString &_rplc,
                               bool _rx,
                               bool _ww,
                               bool _cs)
{
    _set_settings(_expr,_rx,_ww,_cs);
    if (_check_selection())
    {
        _replace(_rplc);

        return true;
    }

    return false;
}

bool ASearchDecorator::replaceAndFind(const QString &_expr,
                                      const QString &_rplc,
                                      bool _rx,
                                      bool _ww,
                                      bool _cs)
{
    if (replace(_expr,_rplc,_rx,_ww,_cs))
        return find(_expr,_rx,_ww,_cs, true);

    return false;
}

bool ASearchDecorator::replaceAll(const QString &_expr,
                                  const QString &_rplc,
                                  bool _rx,
                                  bool _ww,
                                  bool _cs)
{
    if (find(_expr,_rx,_ww,_cs,true))
        while (replaceAndFind(_expr,_rplc, _rx,_ww,_cs));

    return true;
}
