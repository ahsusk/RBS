#ifndef SPECIFICPRESENTER_H
#define SPECIFICPRESENTER_H

#include "devicepresenter.h"

template <class DocT, class ViewT>
class SpecificPresenter : public DevicePresenter
{
public:
    SpecificPresenter(DocT* _doc, ViewT *_view, bool _auto_read) :
        DevicePresenter(_doc,_view,_auto_read)
    {

    }

    ViewT* getView() const
    {
        return getCastedView<ViewT*>();
    }

    DocT* getDocument() const
    {
        return getCastedDevice<DocT*>();
    }

};

#endif // SPECIFICPRESENTER_H
