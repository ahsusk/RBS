#include "objectnamecolumn.h"


ObjectNameColumn::ObjectNameColumn(const QString &_name,
                                   QObject *_parent) :
    SuperClass(_name,_parent)
{
    addFunction(Qt::DisplayRole, [=](QObject* _object)
    {
        QVariant result;
        if (_object)
            result = _object->objectName();
        return result;
    });
}
