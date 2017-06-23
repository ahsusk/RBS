#include "objecttreenode.h"

ObjectTreeNode::ObjectTreeNode(QObject *_item,
                               ObjectTreeNode *_parent,
                               int _type_of_child) :
    item_(_item),
    parent_(_parent),
    type_of_child_(_type_of_child)
{
    if (parent_ != NULL)
        parent_->add_child(this);

    if (_item != NULL)
    {
        const QMetaObject* m_o = _item->metaObject();
        QByteArray data = QMetaObject::normalizedSignature("childAdded(QObject*)");
        int index = m_o->indexOfMethod(data);
        if (index != -1)
        {
            connect(_item,
                    SIGNAL(childAdded(QObject*)),
                    SLOT(_on_child_added(QObject*)),
                    Qt::QueuedConnection);
        }
        data = QMetaObject::normalizedSignature("childRemoved(QObject*)");
        index = m_o->indexOfMethod(data);
        if (index != -1)
        {
            connect(_item,
                    SIGNAL(childRemoved(QObject*)),
                    SLOT(_on_child_removed(QObject*)));
        }
        connect(_item,
                SIGNAL(objectNameChanged(QString)),
                SIGNAL(dataChanged()));

        if (type_of_child_ != -1)
        {
            QList<QObject*> child_list = _item->children();
            foreach (QObject* c, child_list)
            {
                if (_check_child(c))
                {
                    new ObjectTreeNode(c,this,_type_of_child);
                }
            }
        }
    }
}

ObjectTreeNode::~ObjectTreeNode()
{
    clear();
}

int ObjectTreeNode::getCount() const
{
    return children_.count();
}

ObjectTreeNode *ObjectTreeNode::getParent() const
{
    return parent_;
}

int ObjectTreeNode::indexOf(ObjectTreeNode *_child) const
{
    return children_.indexOf(_child);
}

ObjectTreeNode *ObjectTreeNode::getChild(int _index) const
{
    ObjectTreeNode* result = 0;
    if (_index >= 0 && _index < getCount())
        result = children_[_index];
    return result;
}

QObject *ObjectTreeNode::getItem() const
{
    return item_;
}

void ObjectTreeNode::clear()
{
    qDeleteAll(children_);
    children_.clear();
}

void ObjectTreeNode::setParentNode(ObjectTreeNode *_node)
{
    parent_ = _node;
}

void ObjectTreeNode::add_child(ObjectTreeNode *_child)
{
    int result = children_.count();
    emit needInsertRows(result,result);
    children_.append(_child);
    emit endInsertRows();
}

void ObjectTreeNode::remove_child(ObjectTreeNode *_child)
{
    int index = children_.indexOf(_child);
    emit needRemoveRows(index,index);
    children_.removeAt(index);
    delete _child;
    emit endRemoveRows();
}

bool ObjectTreeNode::_check_child(QObject *_child) const
{
    bool result = true;
    if (type_of_child_ > 0)
    {
        bool type_matches = false;
        const QMetaObject* mo = _child->metaObject();
        while (!type_matches && mo)
        {
            QString classname = mo->className();
            classname.push_back('*');
            int type = QMetaType::type(classname.toUtf8().constData());
            type_matches = type == type_of_child_;

            mo = mo->superClass();
        }
        result = type_matches;
    }

    result = result && !_child->objectName().startsWith('@');
    return result;
}

void ObjectTreeNode::_remove_child(QObject *_item)
{
    foreach (ObjectTreeNode* node, children_)
    {
        if (node->getItem() == _item)
        {
            remove_child(node);
            break;
        }
    }
}

void ObjectTreeNode::child_added(QObject *_child)
{
    if (_check_child(_child))
        _on_child_added(_child);

}

void ObjectTreeNode::child_removed(QObject *_child)
{
    if (_check_child(_child))
        _on_child_removed(_child);
}

void ObjectTreeNode::_on_child_added(QObject *_c)
{
    new ObjectTreeNode(_c,this,type_of_child_);
}

void ObjectTreeNode::_on_child_removed(QObject *_c)
{
    _remove_child(_c);
}

