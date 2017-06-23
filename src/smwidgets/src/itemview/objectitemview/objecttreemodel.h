#ifndef OBJECTTREEMODEL_H
#define OBJECTTREEMODEL_H

#include <QAbstractItemModel>
#include <smwidgets/src/sm_widgets_global.h>
#include <smwidgets/src/itemview/itemviewmodel.h>
class ObjectTreeNode;
typedef ItemViewProvider<QObject*> ObjectTreeProvider;
class SM_WIDGETSSHARED_EXPORT ObjectTreeModel : public ItemViewModel<QObject*>
{
    Q_OBJECT
public:
    explicit ObjectTreeModel(Provider* _provider,
                             QObject *parent = 0);
    ~ObjectTreeModel();

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &_index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    ///
    /// \brief setRoot Функция установки корневого объекта.
    ///
    void setRoot(QObject* _object);

    ///
    /// \brief acceptChanges Функция подтверждения изменений.
    ///
    void acceptChanges();

    ///
    /// \brief clearChanges Функция отмены изменений.
    ///
    void clearChanges();

    ///
    /// \brief indexFromItem Функция получения индекса по элементу дерева объектов.
    /// \param _item
    /// \param _from - индекс, с которого необходимо начать поиск.
    ///
    QModelIndex indexFromItem(QObject* _item,
                              const QModelIndex &_from = QModelIndex()) const;

    ///
    /// \brief itemFromIndex Функция получения объекта по индексу.
    ///
    QObject* getItemFromIndex(const QModelIndex& _index) const override;

    ///
    /// \brief Установка метатипа элементов дерева модели. Вызывать в
    /// конструкторе дочерних моделей или сразу после конструктора. но ДО
    /// установки корневого объекта.
    ///
    template<class NodeT>
    void setNodeType()
    {
        int type_id = qMetaTypeId<NodeT*>();
        node_meta_object_ = QMetaType::metaObjectForType(type_id);
    }

    ObjectTreeNode* getNodeFromIndex(const QModelIndex& _index) const;
private:
    QModelIndex index_from_node(ObjectTreeNode* _node) const;
    void connect_node(ObjectTreeNode* _node) const;
    ObjectTreeNode* create_node(QObject *_item, ObjectTreeNode *_parent);
    void accept_changes(ObjectTreeNode* _node) const;
    void clear_changes(ObjectTreeNode* _node) const;

private slots:
    void on_need_insert_rows(int _from, int _to);
    void on_need_remove_rows(int _from, int _to);
    void on_data_changed();
    void end_insert_rows();
    void end_remove_rows();


private:
    const QMetaObject* node_meta_object_;
    ObjectTreeNode* root_;
};

#endif // OBJECTTREEMODEL_H
