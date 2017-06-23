#ifndef SCI_LEXER_DECORATOR_H
#define SCI_LEXER_DECORATOR_H

#include "mvc/decorators/asyntaxdecorator.h"
#include "mvc/views/scintillaview.h"
class QsciLexer;
class SciLexerDecorator : public ASyntaxDecorator
{
    Q_OBJECT
    CASTED_VIEW_GETTER(ScintillaView)
//public:
//    static void setActiveLexer(QsciLexer* _lexer);
//    static QsciLexer* getActiveLexer();
public:
    SciLexerDecorator(ScintillaView* _view);
    ~SciLexerDecorator(){}
    void initView();
signals:
    void currentLexerChanged(const QString& _lexer);

public slots:
    void setLexer(QsciLexer* _lexer);
    QsciLexer* getLexer() const;

    void setFont(const QFont& _font)          const override;
    void setColor(const QColor& _color)       const override;
    void setPaper(const QColor& _paper)       const override;
    void setTabWidth(int _tab_width)          const override;
    void setIndentation(int _indentation)     const override;
    void setUseTabs(bool _use_tabs)           const override;
    void setShowLineNumbers(bool _sn)         const override;
    void setShowWhiteSpaces(bool _sw)         const override;
    void setLineEnding(const QString& _le)    const override;
    void setShowEdgeLine(bool _sel)           const override;
    void setEdgeColumn(int _column)           const override;

    // AWidgetDecorator interface
protected:
    void _set_device_info(Device *_device);
private slots:

    void set_caret_color(const QColor& _color, int _style = 0) const;
//protected:
//    void _deactivated() override;
//    void _activated() override;

private:
    void change_current_lexer(QsciLexer *_lex);

    void set_default_settings();


};

#endif // SCI_LEXER_DECORATOR_H
