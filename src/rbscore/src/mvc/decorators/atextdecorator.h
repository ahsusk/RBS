#ifndef A_TEXT_DECORATOR_H
#define A_TEXT_DECORATOR_H

#include <QObject>
#include <rbscore/src/rbscore_global.h>
#include <rbscore/src/mvc/base/awidgetdecorator.h>

class RBSCORESHARED_EXPORT ATextDecorator : public AWidgetDecorator
{
    Q_OBJECT
public:
    explicit ATextDecorator(QWidget* _view) :
        AWidgetDecorator(_view)
    {

    }

    ~ATextDecorator(){}
signals:
    void cursorPositionChanged(int _line, int _index);

public slots:
    virtual QString    getSelectedText()                             const = 0;
    virtual void       replaceSelectedText(const QString& _text)  const = 0;
    virtual QList<int> getSelection()                             const = 0;
    virtual void       setSelection(const QList<int> &_selection) const = 0;
    virtual QPoint     getCursorPosition()                        const = 0;
    virtual void       setCursorPosition(int _line, int _index)   const = 0;

    virtual QString    getLine(int _line)                         const = 0;

    virtual void       cut()                                      const = 0;
    virtual void       copy()                                     const = 0;
    virtual void       paste()                                    const = 0;
    virtual void       undo()                                     const = 0;
    virtual void       redo()                                     const = 0;

    virtual void       ensureLineVisible(int _line)               const = 0;

};

#endif // A_TEXT_DECORATOR_H
