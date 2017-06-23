#ifndef DOCUMENTPRESENTER_H
#define DOCUMENTPRESENTER_H

#include "specificpresenter.h"
#include <core/src/Serialization/Devices/rbsdocument.h>
#include <logging.h>

template<class ViewT>
class DocumentPresenter : public SpecificPresenter<RBSDocument,ViewT>
{
public:
    DocumentPresenter(RBSDocument* _doc, ViewT* _view, bool _auto_read) :
        SpecificPresenter<RBSDocument,ViewT>(_doc,_view, _auto_read)
    {

    }

    ~DocumentPresenter()
    {
    }

    // DeviceController interface
protected:
    bool _do_write() final
    {
        RBSDocument* doc = this->getDocument();
        QString text = this->getView()->getText();
        doc->setText(text);

        qDebug(SUBMIC_LOG)<<"Запись в документ"<<doc
                  <<"из"<<this->getView()->metaObject()->className()
                    <<"("<<text.length()<<")";
        return true;
    }
    bool _do_read() final
    {
        RBSDocument* doc = this->getDocument();
        QString text = doc->getText();
        this->getView()->setText(text);

        qDebug(SUBMIC_LOG)<<"Запись в редактор"<<this->getView()->metaObject()->className()
                    <<"из"<<doc
                   <<"("<<text.length()<<")";
        return true;
    }

    void _device_attached(Device *_device) override
    {
        SpecificPresenter<RBSDocument,ViewT>::_device_attached(_device);
        RBSDocument* doc = qobject_cast<RBSDocument*>(_device);
        if (!doc)
            qCritical()<<"устройство не документ";
        else
        {
            this->connect(doc,
                          SIGNAL(encodingChanged(QString)),
                          SLOT(_activated()),
                          Qt::QueuedConnection);
        }

    }
};

#endif // DOCUMENTPRESENTER_H
