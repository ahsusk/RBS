#ifndef ITEMVIEWPROVIDER_H
#define ITEMVIEWPROVIDER_H

#include <QVariantMap>
#include <smwidgets/src/itemview/itemviewsectionprovider.h>
#include <smwidgets/src/sm_widgets_global.h>
///
/// \brief Базовый класс для провайдера данных модели.
/// Он нужен для того, чтобы провайдер был QObject'ом.
/// Так как не бывает шаблонных QObject'ов.
///
class SM_WIDGETSSHARED_EXPORT ItemViewProviderObject : public QObject
{
    Q_OBJECT
public:
    explicit ItemViewProviderObject(QObject* _parent) :
        QObject(_parent)
    {

    }

signals:
    void requestReset();
};

///
///
/// \brief Класс для представления данных модели QAbstractItemModel.
/// \param ItemT тип данных
///
/// Хранит в себе список объектов ItemViewSectionProvider, каждый из
/// которых представляет данные для одной секции (колонки/строки).
/// (Используется в ItemViewModel, данные представляются в колонках)
///
/// В этом классе предоставляются функции для использования в модели
/// паттерна "Мост":
/// 1) columnCount
/// 2) flags
/// 3) setData
/// 4) getData
/// В функциях setData и getData в качестве параметра не индекс, а объект.
/// Поэтому в модели нужно получить из индекса объект.
///
/// Класс наследует ItemViewProviderObject для того, чтобы быть шаблонным
/// QObject'ом и слать сигнал requestReset, который означает, что данные
/// изменились и модель надо обновить.
///
/// \todo В будущем предполагается слать сигнал с объектом или как-то еще,
/// чтобы не обновлять всю модель сразу.
///
template<class ItemT>
class ItemViewProvider : public ItemViewProviderObject
{
    typedef ItemViewSectionProvider<ItemT> SectionProvider;
public:
    explicit ItemViewProvider(QObject* _parent) :
        ItemViewProviderObject(_parent)
    {
    }
    ~ItemViewProvider()
    {
        // если не сделать дисконнект, то в модель будут отправляться
        // сигналы.
        disconnect(this, 0, 0, 0);

        // где-то в модели рбс падает, если не удалить вот так.
        // хотя все дочерние провайдеры дети этого провайдера и должны
        // сами удалиться точно так же.
        // возможно, это не правда и она еще упадет при закрытии.
        foreach (SectionProvider* sp, providers_)
            delete sp;


    }

    ///
    /// \brief Функция получения списка провайдеров для секций.
    ///
    QList<SectionProvider*> getSectionProviders() const
    {
        return providers_;
    }

    ///
    /// \brief getData Функция получения данных для объекта \c _item в
    /// секции \c _section для роли \c _role
    ///
    /// Функция проверяет есть ли провайдер для этой секции, и, если есть,
    /// вызывает у него функцию ItemViewSectionProvider::getData
    ///
    QVariant getData(ItemT _item, int _section, int _role)
    {
        if (canOperate(_section))
            return providers_[_section]->getData(_item,_role);
        return QVariant();
    }
    ///
    /// \brief getData Функция получения данных для всех возможных ролей
    /// для объекта \c _item в секции \c _section
    ///
    /// Функция проверяет есть ли провайдер для этой секции, и, если есть,
    /// вызывает у него функцию ItemViewSectionProvider::getData
    ///
    ///
    QMap<int, QVariant> getData(QObject* _item, int _section)
    {
        QMap<int, QVariant>  result;
        if (canOperate(_section))
            result = providers_[_section]->getData(_item);
        return result;
    }

    ///
    /// \brief getHeaderData Функция получения данных для заголовка
    /// секции \c  _section
    ///
    /// \todo Пока работает только для Qt::DisplayRole
    ///
    QVariant getHeaderData(int _section, int _role) const
    {
        QVariant result;
        if (canOperate(_section) && _role == Qt::DisplayRole)
            result = providers_[_section]->getSectionName();
        return result;
    }

    ///
    /// \brief Функция получения количества секций
    ///
    int sectionCount() const
    {
        return providers_.count();
    }

    ///
    /// \brief canOperate Функция проверки существования секции \c _section
    ///
    bool canOperate(int _section) const
    {
        return (_section >= 0 && _section < sectionCount());
    }

    ///
    /// \brief requestMenu Функция запроса контекстного меню в секции
    /// \c _section в позиции \c _pos
    ///
    /// Функция проверяет есть ли провайдер для этой секции, и, если есть,
    /// вызывает у него функцию ItemViewSectionProvider::requestMenu
    ///
    /// \warning Внимание! Если в провайдерах секций эта функция не перегружена,
    /// то никакого меню не будет.
    void requestMenu(int _section, const QPoint& _pos)
    {
        if (canOperate(_section))
            providers_[_section]->requestMenu(_pos);
    }

    ///
    /// \brief addProvider Функция добавления нового провайдера
    ///
    ///
    void addProvider(ItemViewSectionProvider<ItemT>* _provider)
    {
        providers_.append(_provider);

        connect(_provider,
                SIGNAL(requestReset()),
                SIGNAL(requestReset()));

        _provider->setParent(this);

        connect(_provider,
                &SectionProvider::destroyed,
                [=](QObject* _provider)
        {
            int index = -1;
            for (int i = 0; i < providers_.count(); ++i)
            {
                if (_provider == providers_[i])
                {
                    index = i;
                    break;
                }
            }

            if (index != -1)
            {
                qDebug(SUBMIC_LOG)<<"provider external destroyed"<<index;
                providers_.removeAt(index);
                emit requestReset();
            }
        });

        emit requestReset();
    }

    ///
    /// \brief flags Функция получения флагов модели для объекта \c _item
    /// в секции \c _section
    ///
    /// Функция проверяет есть ли провайдер для этой секции, и, если есть,
    /// вызывает у него функцию ItemViewSectionProvider::flags
    Qt::ItemFlags flags(int _section, ItemT _item) const
    {
        Qt::ItemFlags result;
        if (canOperate(_section))
        {
            result = providers_[_section]->flags(_item);
        }
        return result;
    }

    ///
    /// \brief setData Функция установки данных \c _value для объекта \c _item
    /// в секции \c _section в роли \c _role
    ///
    /// Функция проверяет есть ли провайдер для этой секции, и, если есть,
    /// вызывает у него функцию ItemViewSectionProvider::setData
    ///
    /// \warning В случае удачно измененных данных в модели должен быть
    /// сгенерирован сигнал dataChanged для соответствующих индексов.
    ///
    bool setData(int _section,
                 ItemT _item,
                 const QVariant &_value,
                 int _role) const
    {
        bool result = false;
        if (canOperate(_section))
        {
            result = providers_[_section]->setData(_item,_value, _role);
        }

        return result;
    }

    ///
    /// \brief acceptChanges Функция принятия изменений для объекта \c _item
    ///
    /// Функция для моделей, изменяющих данные и сохраняющих временные копии
    /// данных (например, в диалогах настроек, для того, чтобы можно было всё
    /// отменить)
    ///
    void acceptChanges(QObject* _item) const
    {
        foreach (SectionProvider* p, providers_)
        {
            p->acceptChanges(_item);
        }
    }

    ///
    /// \brief clearChanges Функция сброса изменений для объекта \c _item
    ///
    /// Функция для моделей, изменяющих данные и сохраняющих временные копии
    /// данных (например, в диалогах настроек, для того, чтобы можно было всё
    /// отменить. Собственно, эта функция всё и отменяет)
    ///
    void clearChanges(QObject* _item) const
    {
        foreach (SectionProvider* p, providers_)
        {
            p->clearChanges(_item);
        }
    }
private:
    QList<SectionProvider*> providers_;

};

#endif // ITEMVIEWPROVIDER_H
