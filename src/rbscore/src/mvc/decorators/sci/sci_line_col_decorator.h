#ifndef SCIBOTTOMWIDGETPRESENTER_H
#define SCIBOTTOMWIDGETPRESENTER_H

#include <QObject>
#include "mvc/base/awidgetdecorator.h"
#include <QLabel>

class QsciScintilla;

/**
 * @brief The SciBottomWidgetPresenter class класс презентера для нижней
 * панельки у Сцинтиллы.
 */
class SciLineColumnDecorator : public AWidgetDecorator
{
    Q_OBJECT
public:
    explicit SciLineColumnDecorator(QWidget* _view);
    // AWidgetDecorator interface
private slots:
    /**
     * @brief change_values слот изменения номеров столбца и строки.
     * @param _row
     * @param _column
     */
    void change_values(int _row, int _column);

private:
    void change_value(QLabel* _label, int _value);

private:
    QLabel* row_label_;
    QLabel* col_label_;
};

#endif // SCIBOTTOMWIDGETPRESENTER_H
