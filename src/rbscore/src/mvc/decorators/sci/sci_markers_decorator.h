#ifndef MarkersPresenter_H
#define MarkersPresenter_H

#include <QObject>
#include <rbscore/src/mvc/base/awidgetdecorator.h>
#include <rbscore/src/mvc/views/scintillaview.h>
#include <rbscore/src/rbscore_global.h>
#include <core/src/Serialization/Devices/rbsdocument.h>

#include <QMap>
class RBSCORESHARED_EXPORT SciMarkersDecorator : public AWidgetDecorator
{
    Q_OBJECT
    CASTED_VIEW_GETTER(ScintillaView)

public:
    explicit SciMarkersDecorator(ScintillaView *_view);
public slots:
    void addMarker(uint _marker_number,
                   int _line,
                   bool _force_show = false);

    void registerMarker(int _bit,
                        const QPixmap& _pixmap);

    void removeMarker(uint _marker_number,
                      int _line);

protected slots:
    void _set_device_info(Device *_device = 0);
private slots:
    void position_changed(int _l, int);
    void lexer_changed(QsciLexer* _lexer);
    void load_paper();

private:
    QMetaObject::Connection lexer_connection_;
    ///
    /// \brief Маркеры, которые стоят в документе.
    /// В формате:
    /// <номер строки, номер маркера> - хендлер
    ///
    QMap<QPair<int,uint>, int> placed_markers_;
};

#endif // MarkersPresenter_H
