#ifndef ASYNTAXPRESENTER_H
#define ASYNTAXPRESENTER_H

#include <QObject>
#include "Core/src/Serialization/Devices/rbsdocument.h"
#include "mvc/base/awidgetdecorator.h"
#include "core/src/settings/settings.h"

#include "Settings/SyntaxSettings/textviewsettings.h"

/**
 * \brief SyntaxPresenter класс презентера для подсветки текста.
 * Нужны отдельные реализации для сцинтиллы и хтмл.
 */

class ASyntaxDecorator : public AWidgetDecorator
{
    Q_OBJECT
public:
    explicit ASyntaxDecorator(QWidget *_view);

    ~ASyntaxDecorator();

public slots:
    virtual void setFont(const QFont& _font)          const = 0;
    virtual void setColor(const QColor& _color)       const = 0;
    virtual void setPaper(const QColor& _paper)       const = 0;
    virtual void setTabWidth(int _tab_width)          const = 0;
    virtual void setIndentation(int _indentation)     const = 0;
    virtual void setUseTabs(bool _use_tabs)           const = 0;
    virtual void setShowLineNumbers(bool _sn)         const = 0;
    virtual void setShowWhiteSpaces(bool _sw)         const = 0;
    virtual void setLineEnding(const QString& _le)    const = 0;
    virtual void setShowEdgeLine(bool _sel)           const = 0;
    virtual void setEdgeColumn(int _column)           const = 0;

};

#endif // ASYNTAXPRESENTER_H
