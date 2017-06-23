#include "sci_line_col_decorator.h"
#include <Qsci/qsciscintilla.h>
#include <QVBoxLayout>
#include <QDebug>
SciLineColumnDecorator::SciLineColumnDecorator(QWidget *_view) :
    AWidgetDecorator(_view),
    row_label_(0),
    col_label_(0)
{

    QWidget* bottom_wgt = new QWidget(getView());
    QHBoxLayout* hb = new QHBoxLayout(bottom_wgt);
    bottom_wgt->setLayout(hb);
    hb->addSpacerItem(new QSpacerItem(1,10,
                                      QSizePolicy::Expanding,
                                      QSizePolicy::Preferred));
    hb->setMargin(0);
    hb->setSpacing(1);

    QLabel* row_text = new QLabel(tr("Строка: "), bottom_wgt);
    row_label_ = new QLabel("-1", bottom_wgt);
    row_label_->setAlignment(Qt::AlignLeft);
    QLabel* col_text = new QLabel(tr("Столбец: "), bottom_wgt);
    col_label_ = new QLabel("-1", bottom_wgt);
    col_label_->setAlignment(Qt::AlignLeft);
    hb->addWidget(row_text);
    hb->addWidget(row_label_);
    hb->addWidget(col_text);
    hb->addWidget(col_label_);

    QWidget* parent_wgt = getView()->parentWidget();
    QLayout* layout = parent_wgt->layout();
    QVBoxLayout* vbl = qobject_cast<QVBoxLayout*>(layout);
    if (!vbl)
    {
        qWarning()<<"чтобы добавить виджеты строки и столбца нужен"
                    "вертикальный лейаут";
        return;
    }
    int index = vbl->indexOf(getView());
    vbl->insertWidget(index+1,bottom_wgt);

    QFont font_for_labels("Courier new",8);
    row_text->setFont(font_for_labels);
    row_label_->setFont(font_for_labels);
    col_text->setFont(font_for_labels);
    col_label_->setFont(font_for_labels);

    QFontMetrics metric(font_for_labels);
    // находим ширину 4 цифр
    int width = metric.width("0000");

    row_label_->setMinimumWidth(width);
    col_label_->setMinimumWidth(width);

    connect(getView(),
            SIGNAL(cursorPositionChanged(int,int)),
            SLOT(change_values(int,int)));
}

void SciLineColumnDecorator::change_values(int _row, int _column)
{
    change_value(row_label_,_row+1);
    change_value(col_label_,_column);
}

void SciLineColumnDecorator::change_value(QLabel *_label, int _value)
{
    _label->setText(QString::number(_value));
}
