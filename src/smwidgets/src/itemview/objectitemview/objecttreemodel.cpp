#include "objecttreemodel.h"
#include "objecttreenode.h"
#include <QDebug>
ObjectTreeModel::ObjectTreeModel(Provider *_provider, QObject *parent)
    : SuperClass(_provider, parent),
      root_(NULL)
{
    node_meta_object_ = &ObjectTreeNode::staticMetaObject;
    root_ = new ObjectTreeNode(0,0);
}

ObjectTreeModel::~ObjectTreeModel()
{
    delete root_;
}


QModelIndex ObjectTreeModel::index(int row,
                                   int column,
                                   const QModelIndex &parent) const
{
    QModelIndex result;

   if (root_ != NULL)
   {
       ObjectTreeNode* parent_node = getNodeFromIndex(parent);
       ObjectTreeNode* child_node = parent_node->getChild(row);
       if (child_node != NULL)
       {
           result = createIndex(row,column,child_node);
           connect_node(child_node);
       }
   }
   return result;
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &_index) const
{
    ObjectTreeNode* child_node = getNodeFromIndex(_index);
    if (child_node == NULL)
        return QModelIndex();

    ObjectTreeNode* node = child_node->getParent();
    if (node == NULL)
        return QModelIndex();

    ObjectTreeNode* parent_node = node->getParent();
    if (parent_node == NULL)
        return QModelIndex();

    int row = parent_node->indexOf(node);
    return createIndex(row,0,node);
}

int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    ObjectTreeNode* node = getNodeFromIndex(parent);
    return node->getCount();
}

void ObjectTreeModel::setRoot(QObject *_object)
{
    beginResetModel();
    root_->clear();
    create_node(_object,root_);
    endResetModel();
}

void ObjectTreeModel::acceptChanges()
{
    accept_changes(root_);
}

void ObjectTreeModel::clearChanges()
{
    clear_changes(root_);
}


QModelIndex ObjectTreeModel::index_from_node(
        ObjectTreeNode *_node) const
{
    return indexFromItem(_node->getItem());
}

QModelIndex ObjectTreeModel::indexFromItem(QObject *_item, const QModelIndex &_from) const
{
    QModelIndex result;
    QModelIndex r = _from;
    int rc = rowCount(r);

    for (int i = 0; i < rc; ++i)
    {
        result = index(i,0,r);
        ObjectTreeNode* node = getNodeFromIndex(result);
        if (node->getItem() == _item)
        {
            break;
        }
        else
        {
            result = indexFromItem(_item,result);
            if (result.isValid())
                break;
        }
    }

    return result;
}

QObject *ObjectTreeModel::getItemFromIndex(const QModelIndex &_index) const
{
    ObjectTreeNode* node = getNodeFromIndex(_index);
    if (node)
    {
        return node->getItem();
    }
    return 0;
}

ObjectTreeNode *ObjectTreeModel::getNodeFromIndex(const QModelIndex &_index) const
{
    if (_index.isValid())
        return reinterpret_cast<ObjectTreeNode*>(_index.internalPointer());
    return root_;
}

void ObjectTreeModel::connect_node(ObjectTreeNode *_node) const
{
    connect(_node,
            SIGNAL(needInsertRows(int,int)),
            SLOT(on_need_insert_rows(int,int)),
            Qt::UniqueConnection);
    connect(_node,
            SIGNAL(endInsertRows()),
            SLOT(end_insert_rows()),
            Qt::UniqueConnection);
    connect(_node,
            SIGNAL(endRemoveRows()),
            SLOT(end_remove_rows()),
            Qt::UniqueConnection);
    connect(_node,
            SIGNAL(needRemoveRows(int,int)),
            SLOT(on_need_remove_rows(int,int)),
            Qt::UniqueConnection);
    connect(_node,
            SIGNAL(dataChanged()),
            SLOT(on_data_changed()),
            Qt::UniqueConnection);
}


ObjectTreeNode *ObjectTreeModel::create_node(QObject* _item, ObjectTreeNode*_parent)
{
    ObjectTreeNode* node =0 ;
    if (node_meta_object_)
    {
        QObject* result = node_meta_object_->newInstance(
                    Q_ARG(QObject*,_item),
                    Q_ARG(ObjectTreeNode*,_parent));
        node = qobject_cast<ObjectTreeNode*>(result);
        if (!node)
            qCritical()<<"Плохой конструктор элементов дерева.";

    }
    else
        qCritical()<<"Плохой метатип элементов дерева.";
    return node;
}

void ObjectTreeModel::accept_changes(ObjectTreeNode *_node) const
{
    int cnt = _node->getCount();
    for (int i = 0; i < cnt; ++i)
    {
        ObjectTreeNode* c = _node->getChild(i);
        accept_changes(c);
    }

    QObject* item = _node->getItem();
    _accept_changes(item);
}

void ObjectTreeModel::clear_changes(ObjectTreeNode *_node) const
{
    int cnt = _node->getCount();
    for (int i = 0; i < cnt; ++i)
    {
        ObjectTreeNode* c = _node->getChild(i);
        clear_changes(c);
    }

    QObject* item = _node->getItem();
    _clear_changes(item);
}

void ObjectTreeModel::on_need_insert_rows(int _from, int _to)
{
    QObject* sender_obj = sender();
    ObjectTreeNode* node = qobject_cast<ObjectTreeNode*>(sender_obj);

    QModelIndex index = index_from_node(node);
    beginInsertRows(index,_from,_to);
}

void ObjectTreeModel::on_need_remove_rows(int _from, int _to)
{
    QObject* sender_obj = sender();
    ObjectTreeNode* node = qobject_cast<ObjectTreeNode*>(sender_obj);
    QModelIndex index = index_from_node(node);
    beginRemoveRows(index,_from,_to);
}

void ObjectTreeModel::on_data_changed()
{

    QObject* sender_obj = sender();

    ObjectTreeNode* node = qobject_cast<ObjectTreeNode*>(sender_obj);
    QModelIndex node_index = index_from_node(node);
    QModelIndex parent_index = node_index.parent();
    int row = node_index.row();

    QModelIndex from = index(row,0,parent_index);
    QModelIndex to = index(row,columnCount(parent_index)-1,parent_index);

    qDebug()<<"on data changed"<<sender_obj;
    qDebug()<<"обновим индексы в строке"<<row<<"с"<<from<<"по"<<to;
//    qDebug()<<"emit data changed"<<sender_obj<<from<<to;
    emit dataChanged(from,to);
}

void ObjectTreeModel::end_insert_rows()
{
    endInsertRows();
}

void ObjectTreeModel::end_remove_rows()
{
    endRemoveRows();
}
