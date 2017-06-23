#ifndef ASEARCHDECORATOR_H
#define ASEARCHDECORATOR_H

#include <QObject>
#include <rbscore/src/rbscore_global.h>
#include <rbscore/src/mvc/base/awidgetdecorator.h>

class ASearchDecorator : public AWidgetDecorator
{
    Q_OBJECT

protected:

    typedef QPair<int, int> SearchResult;

public:
    explicit ASearchDecorator(QWidget* _view);

    bool find(const QString& _expression,
              bool _reg_exp,
              bool _whole_word,
              bool _case_sensitive,
              bool _forward,
              bool _left = false);

    bool replace(const QString& _expr,
                 const QString& _rplc,
                 bool _rx,
                 bool _ww,
                 bool _cs);

    bool replaceAndFind(const QString& _expr,
                        const QString& _rplc,
                        bool _rx,
                        bool _ww,
                        bool _cs);

    bool replaceAll(const QString& _expr,
                    const QString& _rplc,
                    bool _rx,
                    bool _ww,
                    bool _cs);

protected:
    virtual void _select(int _from, int _cnt) = 0;
    virtual void _set_settings(const QString& _expression,
                               bool _reg_exp,
                               bool _whole_word,
                               bool _case_sensitive) = 0;

    virtual bool _check_selection() = 0;
    virtual void _replace(const QString& _rplc) = 0;
    virtual SearchResult _run_search(bool _forward, bool _left) = 0;
protected:
};

#endif // ASEARCHDECORATOR_H

