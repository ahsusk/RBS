#include "sci_markers_decorator.h"
#include "Core/src/Serialization/Devices/rbsdocument.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include "utils/src/filesystem/filesystem.h"
#include "src/mvc/managers/markersmanager.h"
#include <logging.h>

SciMarkersDecorator::SciMarkersDecorator(ScintillaView* _view):
    AWidgetDecorator(_view)
{
    _view->markerDefine(QsciScintilla::Background,
                        MarkersManager::MARKER_LINE_BACKGROUD);

    _view->SendScintilla(QsciScintillaBase::SCI_MARKERSETBACK,
                         MarkersManager::MARKER_LINE_BACKGROUD,
                         0xffaa00);

    _view->SendScintilla(QsciScintillaBase::SCI_MARKERENABLEHIGHLIGHT,true);

    connect(_view,
            SIGNAL(allTextChanged()),
            SLOT(_set_device_info()));

    connect(_view,
            SIGNAL(lexerChanged(QsciLexer*)),
            SLOT(lexer_changed(QsciLexer*)));

    connect(_view,
            SIGNAL(cursorPositionChanged(int,int)),
            SLOT(position_changed(int,int)));

    lexer_changed(_view->lexer());
}

void SciMarkersDecorator::position_changed(int _l, int)
{
    ScintillaView* sci = getScintillaView();
    sci->markerDeleteAll(MarkersManager::MARKER_LINE_BACKGROUD);
    sci->markerAdd(_l, MarkersManager::MARKER_LINE_BACKGROUD);
}

void SciMarkersDecorator::lexer_changed(QsciLexer *_lexer)
{
    load_paper();

    if (lexer_connection_)
        disconnect(lexer_connection_);

    if (_lexer)
        lexer_connection_ = connect(_lexer,
                                SIGNAL(paperChanged(QColor,int)),
                                SLOT(load_paper()));
    else
        lexer_connection_ = connect(getView(),
                                    SIGNAL(paperChanged(QColor)),
                                    SLOT(load_paper()));
}

void SciMarkersDecorator::load_paper()
{
    ScintillaView* view = getScintillaView();
    QsciLexer* lexer = view->lexer();
    QColor paper;
    // получаем цвет фона редактора
    if (lexer == 0)
        paper = view->paper();
    else
        paper = lexer->paper(0);

    // получаем компоненты
    int r = paper.red();
    int g = paper.green();
    int b = paper.blue();

    // находим среднее значение
    int mid = (r + g + b)/3;
    int k;

    if (mid > 0x7f)
    {
        // цвет считается светлым, надо сделать чуть темнее
        k = -0x10;
    }
    else
    {
        // цвет тёмный, надо светлее
        k = 0x10;
    }

    r += k; g += k; b += k;

    // если вдруг переборщили, то делаем максимум 0xff
    if (r > 0xff) r = 0xff;
    if (g > 0xff) g = 0xff;
    if (b > 0xff) b = 0xff;

    // цвета в сцинтиле представляются наоборот оО (только в этой команде),
    // поэтому

    int color = (b << 16) | (g << 8) | r;

    view->SendScintilla(QsciScintillaBase::SCI_MARKERSETBACK,
                        MarkersManager::MARKER_LINE_BACKGROUD,
                        color);
}

void SciMarkersDecorator::addMarker(uint _marker_number,
                                  int _line,
                                  bool _force_show)
{
    ScintillaView* view = getScintillaView();
    if (placed_markers_.contains({_line,_marker_number}))
    {
        qDebug(SUBMIC_LOG)<<"маркер с номером"<<_marker_number<<"уже добавлен"
                  "в строку"<<_line<<view;
        return;
    }

    int result = view->markerAdd(_line,_marker_number);
    if (result != -1)
        placed_markers_.insert({_line,_marker_number}, result);

    qDebug(SUBMIC_LOG)<<"Добавлен маркер с номером"<<_marker_number<<"в строку"<<_line
           <<"Идентификатор:"<<result<<getView();

    if (_force_show)
    {
        view->ensureLineVisible(_line);
        view->setFocus();
    }
}

void SciMarkersDecorator::registerMarker(int _bit, const QPixmap &_pixmap)
{
    ScintillaView* view = getScintillaView();
    int mn = view->markerDefine(_pixmap,_bit);
    int mask = view->marginMarkerMask(0);

    mask |= 1<<mn;
    view->setMarginMarkerMask(0,mask);

    qDebug(SUBMIC_LOG)<<"регистрация маркера"<<_bit<<"в редакторе"<<view
              <<"номер:"<<mn
           <<"маска:"<<QString::number(mask,2);

}

void SciMarkersDecorator::removeMarker(uint _marker_number, int _line)
{
    ScintillaView* view = getScintillaView();
    qDebug(SUBMIC_LOG)<<"Запрос на удаление маркера"<<_marker_number
                     <<"линия"<<_line<<"редактор"<<getView();
    if (_line != -1)
    {
        QPair<int,uint> marker_key(_line,_marker_number);
        if (placed_markers_.contains(marker_key))
        {
            int handler = placed_markers_.value(marker_key);
            qDebug(SUBMIC_LOG)<<"Удаляется маркер в строке"<<_line
                   <<"с номером"<<_marker_number<<"идентификатор:"<<handler;
            view->markerDeleteHandle(handler);
            placed_markers_.remove(marker_key);
        }
    }
    else
    {
        QList<QPair<int, uint> > keys = placed_markers_.keys();
        for (int i = keys.count() - 1; i >= 0; --i)
        {
            QPair<int,uint> key = keys[i];
            if (key.second != _marker_number)
            {
                removeMarker(_marker_number,key.first);
            }
        }
    }
}

void SciMarkersDecorator::_set_device_info(Device *_device)
{
    if (_device == 0)
    {
        _device = DevicePresenter::getDeviceByView(getView());
    }

    RBSDocument* doc = qobject_cast<RBSDocument*>(_device);

    QMap<int,uint> markers = theMarkersManager()->getMarkersForDocument(doc);

    QMapIterator<int,uint> iter_markers(markers);
    while(iter_markers.hasNext())
    {
        iter_markers.next();
        int line = iter_markers.key();
        uint mask = iter_markers.value();
        for (uint i = 0; i < 32; ++i)
        {
            uint bit = 1<<i;
            if ((mask & bit) != 0)
                addMarker(i,line,false);
        }
    }
}


