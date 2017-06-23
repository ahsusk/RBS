#ifndef OBJECTTREENODE_H
#define OBJECTTREENODE_H

#include <QObject>
#include <QMap>
#include <smwidgets/src/sm_widgets_global.h>



///
/// \brief Класс элемента дерева объектов.
///
/// Соответствует 1 к 1 с объектами QObject. То есть является оберткой вокруг
/// одного элемента дерева объектов.
/// Считает количество дочерних элементов опеределенного типа или вообще всех
/// (см. конструктор). Генерит сигналы о том, что добавлен/удален дочерний
/// объект.
///
class SM_WIDGETSSHARED_EXPORT ObjectTreeNode: public QObject
{
    Q_OBJECT

public:
    ///
    /// \brief ObjectTreeNode Конструктор
    ///
    /// \param _item объект, соответствующий этому элементу дерева объектов.
    /// Если указатель на него не нулевой, то будет произведена проверка,
    /// есть ли в классе этого объекта сигналы childAdded и childRemoved.
    /// Если они есть, то они будут подключены к слотам _on_child_added и
    /// _on_child_removed соответственно. Если таких сигналов нет, это не ошибка.
    ///
    /// Сигнал objectNameChanged коннектится к сигналу dataChanged();
    ///
    /// Если нужны еще какие-то случаи генерации этого сигнала, необходимо
    /// коннектить самостоятельно в дочерних классах.
    ///
    /// \param _parent - указатель на родительский элемент дерева элементов.
    ///
    /// \param _type_of_child -1 - не добавлять детей
    ///                       0 - добавить всех детей
    ///                       тип - добавить детей только этого типа.
    Q_INVOKABLE ObjectTreeNode(QObject* _item,
                               ObjectTreeNode* _parent,
                               int _type_of_child = 0);

public:
    ~ObjectTreeNode();

    ///
    /// \brief getCount Функция получения количества дочерних элементов.
    /// \return Возвращает количество дочерних элементов типа _type_of_child,
    /// переданного в конструкторе.
    ///
    int getCount() const;
    ///
    /// \brief getParent Получение родительского элемента
    ///
    ObjectTreeNode* getParent() const;
    ///
    /// \brief indexOf Получение индекса дочернего элемента
    /// \return индекс или -1
    ///
    int indexOf(ObjectTreeNode* _child) const;

    ///
    /// \brief getChild Получение дочернего элемента по индексу \c _index
    ///
    ObjectTreeNode* getChild(int _index) const;

    ///
    /// \brief getItem Получение объекта, хранящегося в элементе
    ///
    QObject* getItem() const;

    ///
    /// \brief clear Очистка элемента. УДАЛЯЮТСЯ все дочерние.
    ///
    void clear();

    void setParentNode(ObjectTreeNode* _node);
signals:
    ///
    /// \brief needInsertRows Сигнал о том, что добавляется дочерний элемент.
    /// Вызывается по childAdded объекта.
    /// \c _from и \c _to всегда одинаковые и равны количеству детей ДО добавления.
    ///
    void needInsertRows(int _from, int _to);
    ///
    /// \brief endInsertRows Сигнал о том, что дочерний элемент закончил добавляться.
    ///
    void endInsertRows();
    ///
    /// \brief needRemoveRows Сигнал о том, что удаляется дочерний элемент.
    /// Вызывается по childRemoved объекта. УДАЛЯЕТ дочерний элемент (не объект).
    /// \c _from и \c _to всегда одинаковые и равны индексу, по которому хранился
    /// элемент.
    ///
    void needRemoveRows(int _from, int _to);
    ///
    /// \brief endRemoveRows Сигнал о том, что дочерний элемент закончил удаление.
    ///
    void endRemoveRows();

    ///
    /// \brief dataChanged Сигнал о том, что данные в объекте изменились.
    ///
    void dataChanged();

private:
    void add_child(ObjectTreeNode* _child);
    void remove_child(ObjectTreeNode* _child);

protected:
    void _remove_child(QObject *_item);
    ///
    /// \brief check_child Функция проверки, является ли дочерний объект
    /// \c _child объектом, за которым надо следить.
    bool _check_child(QObject* _child) const;

private slots:
    void child_added(QObject* _child);
    void child_removed(QObject* _child);

protected slots:
    virtual void _on_child_added(QObject*_c);
    virtual void _on_child_removed(QObject*_c);
private:
    QObject* item_;
    QList<ObjectTreeNode*> children_;
    ObjectTreeNode* parent_;
    int type_of_child_;
};

#endif // OBJECTTREENODE_H
