#ifndef ITEMVIEWSECTIONPROVIDER_H
#define ITEMVIEWSECTIONPROVIDER_H

#include <QVariant>
#include <smwidgets/src/sm_widgets_global.h>
#include <functional>
#include <logging.h>


class SM_WIDGETSSHARED_EXPORT ItemViewSectionProviderObjectBase : public QObject
{
    Q_OBJECT

public:
    explicit ItemViewSectionProviderObjectBase(QObject* _parent) :
        QObject(_parent)
    {

    }

signals:
    void requestReset();
};

///
/// \brief Класс провайдера секции модели QAbstractItemModel
///
/// (Используется в ItemViewProvider)
///
/// Наследует ItemViewSectionProviderObjectBase для того, чтобы быть
/// шаблонным QObject'ом.
///
template<class ItemT>
class ItemViewSectionProvider : public ItemViewSectionProviderObjectBase
{
protected:
    typedef std::function<QVariant (ItemT) > DataFunction;
    typedef ItemViewSectionProvider<ItemT> SuperClass;
public:
    ItemViewSectionProvider(const QString& _name,
                            QObject* _parent) :
        ItemViewSectionProviderObjectBase(_parent),
        section_name_(_name)
    {
        functions_[Qt::UserRole] = [=] (ItemT _item)
        {
            return _get_user_data(_item);
        };
    }

    ~ItemViewSectionProvider()
    {
        qDebug(SUBMIC_LOG)<<"provider for section"<<section_name_<<"destroyed";
    }

    ///
    /// \brief Функция получения названия колонки
    ///
    QString getSectionName() const
    {
        return section_name_;
    }

    ///
    /// \brief Функция установки названия колонки
    ///
    void setSectionName(const QString& _name)
    {
        if (section_name_ != _name)
        {
            section_name_ = _name;
            emit requestReset();
        }
    }

    ///
    /// \brief Функция получения ролей, которые поддерживает данный объект.
    /// Это просто список ключей карты \c functions_
    ///
    QList<int> getSupportedRoles() const
    {
        return functions_.keys();
    }

    ///
    /// \brief Функция получения данных всех поддерживаемых ролей для
    /// объекта \c _item
    ///
    QMap<int, QVariant> getData(ItemT _item) const
    {
        QMap<int,QVariant> result;
        QList<int> roles = getSupportedRoles();
        foreach(int role, roles)
        {
            QVariant data = getData(_item,role);
            result.insert(role,data);
        }
        return result;
    }

    ///
    /// \brief Функция получения данных для объекта \c _item для роли \c _role
    ///
    QVariant getData(ItemT _item, int _role) const
    {
        if (functions_.contains(_role))
        {
            return functions_[_role](_item);
        }
        return QVariant();
    }

    ///
    /// \brief Функция запроса меню в точке. Никак не обрабатывается в этом классе.
    ///
    virtual void requestMenu(const QPoint& ) {}

    ///
    /// \brief Функция получения флагов для объекта \c _item
    ///
    virtual Qt::ItemFlags flags(ItemT _item) const
    {
        Q_UNUSED(_item);
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    ///
    /// \brief Функция изменения данных объекта \c _item роли \c _role на
    /// новые данные \c _data
    /// \return true - в случае успешного изменения
    ///
    virtual bool setData(ItemT _item, const QVariant& _data, int _role)
    {
        Q_UNUSED(_item)
        Q_UNUSED(_data)
        Q_UNUSED(_role)


        return false;
    }

    ///
    /// \brief Функция применения изменений, сделанных в таблице, к объекту
    ///
    virtual void acceptChanges(ItemT) const {}
    ///
    /// \brief Функция отмены изменений, сделанных в таблице, к объекту
    ///
    virtual void clearChanges(ItemT) const {}

protected:
    ///
    /// \brief Функция получения пользовательских данных для объекта.
    /// \return По умолчанию возвращает QVariant()
    ///
    virtual QVariant _get_user_data(ItemT) const
    {
        return QVariant();
    }

    void addFunction(int _role, DataFunction _function)
    {
        functions_.insert(_role,_function);
    }

private:
    QString section_name_;
    QMap<int,DataFunction> functions_;
};
#endif // ITEMVIEWSECTIONPROVIDER_H
