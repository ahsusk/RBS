#ifndef OBJECTNAMECOLUMN_H
#define OBJECTNAMECOLUMN_H
#include <smwidgets/src/itemview/itemviewsectionprovider.h>
#include <smwidgets/src/sm_widgets_global.h>
class SM_WIDGETSSHARED_EXPORT ObjectNameColumn :
        public ItemViewSectionProvider<QObject*>
{
    Q_OBJECT
public:
    explicit ObjectNameColumn(const QString& _name,
                              QObject* _parent);
};

#endif // OBJECTNAMECOLUMN_H
