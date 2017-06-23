#ifndef SCINTILLATEXTPRESENTER_H
#define SCINTILLATEXTPRESENTER_H

#include <QObject>
#include "mvc/decorators/atextdecorator.h"
#include "mvc/views/scintillaview.h"

class SciTextDecorator : public ATextDecorator
{
    Q_OBJECT
    CASTED_VIEW_GETTER(ScintillaView)
public:
    Q_INVOKABLE explicit SciTextDecorator(QWidget* _view);
    ~SciTextDecorator(){}
public slots:
    QString     getSelectedText()                          const override;
    void        replaceSelectedText(const QString& _text)  const override;
    QList<int>  getSelection()                             const override;
    void        setSelection(const QList<int> &_selection) const override;
    QPoint      getCursorPosition()                        const override;
    void        setCursorPosition(int _line, int _index)   const override;
    QString     getLine(int _line)                         const override;
    void        cut()                                      const override;
    void        copy()                                     const override;
    void        paste()                                    const override;
    void        undo()                                     const override;
    void        redo()                                     const override;
    void        ensureLineVisible(int _line)               const override;

};

#endif // SCINTILLATEXTPRESENTER_H
