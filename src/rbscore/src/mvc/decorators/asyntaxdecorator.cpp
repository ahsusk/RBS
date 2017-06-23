#include "asyntaxdecorator.h"

ASyntaxDecorator::ASyntaxDecorator(QWidget *_view) :
    AWidgetDecorator(_view)
{
    TextViewSettings* tvs = theSettings()->findChild<TextViewSettings*>();

    connect(tvs,
            SIGNAL(fontChanged(QFont)),
            SLOT(setFont(QFont)));

    connect(tvs,
            SIGNAL(colorChanged(QColor)),
            SLOT(setColor(QColor)));

    connect(tvs,
            SIGNAL(paperChanged(QColor)),
            SLOT(setPaper(QColor)));

    connect(tvs,
            SIGNAL(indentationChanged(int)),
            SLOT(setIndentation(int)));

    connect(tvs,
            SIGNAL(tabWidthChanged(int)),
            SLOT(setTabWidth(int)));

    connect(tvs,
            SIGNAL(useTabsChanged(bool)),
            SLOT(setUseTabs(bool)));

    connect(tvs,
            SIGNAL(showLineNumbersChanged(bool)),
            SLOT(setShowLineNumbers(bool)));

    connect(tvs,
            SIGNAL(showWhiteSpacesChanged(bool)),
            SLOT(setShowWhiteSpaces(bool)));

    connect(tvs,
            SIGNAL(showEdgeLineChanged(bool)),
            SLOT(setShowEdgeLine(bool)));
    connect(tvs,
            SIGNAL(edgeColumnChanged(int)),
            SLOT(setEdgeColumn(int)));
}

ASyntaxDecorator::~ASyntaxDecorator()
{

}

