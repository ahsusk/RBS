#ifndef ITEMVIEWMODEL_H
#define ITEMVIEWMODEL_H

#include <QAbstractItemModel>
#include "itemviewprovider.h"

///
/// \brief Класс модели QAbstractItemModel, использующий в качестве представления
/// данных колонок (!) ItemViewProvider.
///
/// \param ItemT тип данных в ячейках представления
///
/// Количество колонок, данные, данные заголовков определяются из провайдера.
/// Функции index, parent, rowCount необходимо перегрузить самостоятельно.
/// Таким образом, этот класс пригоден и для таблиц, и для деревьев.
/// Для списков тоже, если будет одна колонка.
///
template<class ItemT>
class ItemViewModel : public QAbstractItemModel
{
public:
    enum ExtendedRoles
    {
        // роль для получения объекта
        ItemRole = Qt::UserRole + 0x100
    };

protected:
    typedef ItemViewModel<ItemT> SuperClass;
    typedef ItemViewProvider<ItemT> Provider;
public:
    explicit ItemViewModel(ItemViewProvider<ItemT>* _provider,
                           QObject* _parent) :
        QAbstractItemModel(_parent),
        provider_(_provider)
    {

        connect(provider_,
                &Provider::requestReset,
                [=]()
        {
            beginResetModel();
            endResetModel();
        });

        provider_->setParent(this);
    }

    ~ItemViewModel()
    {
    }

    ///
    /// \brief data Перегруженная функция
    ///
    /// Для всех ролей, кроме ItemRole, использует данные провайдера.
    /// Для ItemRole возвращает объект в индексе \c _index
    ///
    QVariant data(const QModelIndex &_index, int _role) const override
    {
        QVariant result;
        ItemT item = getItemFromIndex(_index);
        if (item != 0)
        {
            if (_role == ItemRole)
                result = qVariantFromValue(item);
            else
            {
                int col = _index.column();
                result = provider_->getData(item,col,_role);
            }
        }

        return result;
    }

    int columnCount(const QModelIndex &) const override
    {
        return provider_->sectionCount();
    }

    QVariant headerData(int _section, Qt::Orientation, int _role) const override
    {
        return provider_->getHeaderData(_section, _role);
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        ItemT item = getItemFromIndex(index);
        int col = index.column();
        Qt::ItemFlags flags = provider_->flags(col,item);
        return flags;
    }
    bool setData(const QModelIndex &index, const QVariant &value, int role) override
    {
        ItemT item = getItemFromIndex(index);
        int col = index.column();
        bool changed = provider_->setData(col,item,value,role);
        if (changed)
            emit dataChanged(index,index);
        return changed;
    }

    ///
    /// \brief getItemFromIndex Функция получения объекта по индексу.
    /// Должна быть определена в дочерних классах
    ///
    virtual ItemT getItemFromIndex(const QModelIndex& _index) const = 0;

    ///
    /// \brief requestMenu Функция запроса контекстного меню заголовка секции
    /// \c _section
    ///
    void requestMenu(int _section,
                     const QPoint& _point)
    {
        provider_->requestMenu(_section, _point);
    }

protected:
    void _accept_changes(ItemT _item) const
    {
        provider_->acceptChanges(_item);
    }

    void _clear_changes(ItemT _item) const
    {
        provider_->clearChanges(_item);
    }

private:
    ItemViewProvider<ItemT>* provider_;
};

#endif // ITEMVIEWMODEL_H
