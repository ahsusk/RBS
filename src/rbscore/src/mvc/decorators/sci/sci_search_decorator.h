#ifndef SCINTILLASEARCHDECORATOR_H
#define SCINTILLASEARCHDECORATOR_H

#include "src/mvc/decorators/search/regexpsearchdecorator.h"
#include "src/mvc/views/scintillaview.h"
class ScintillaSearchDecorator : public RegExpSearchDecorator
{
    Q_OBJECT

    CASTED_VIEW_GETTER(ScintillaView)
public:
    Q_INVOKABLE explicit ScintillaSearchDecorator(QWidget* _view);

    // ASearchDecorator interface
protected:
    void _select(int _from, int _cnt) override;

    void _replace(const QString &_rplc) override;
    // RegExpSearchDecorator interface
protected:
    QString _get_text() const override;
    QString _get_selected_text() const override;
    int _get_current_position(bool _forward, bool _left) const override;
};

#endif // SCINTILLASEARCHDECORATOR_H
