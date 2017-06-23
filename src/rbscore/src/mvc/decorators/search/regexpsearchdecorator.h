#ifndef REGEXPSEARCHDECORATOR_H
#define REGEXPSEARCHDECORATOR_H

#include "asearchdecorator.h"
class RegExpSearchDecorator : public ASearchDecorator
{
    Q_OBJECT
public:
    explicit RegExpSearchDecorator(QWidget* _view);

    // ASearchDecorator interface
protected:
    void _set_settings(const QString &_expression, bool _reg_exp, bool _whole_word, bool _case_sensitive);

    bool _check_selection();
    SearchResult _run_search(bool _forward, bool _left) override;

    virtual QString _get_text() const = 0;
    virtual QString _get_selected_text() const = 0;
    virtual int _get_current_position(bool _forward, bool _left) const = 0;

private:
    int search(const QString &_text, int _offset, bool _forward) const;
private:
    QRegExp search_rx_;
};

#endif // REGEXPSEARCHDECORATOR_H
