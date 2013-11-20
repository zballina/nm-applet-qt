#include <QtCore/QModelIndex>

#include "listitemdelegate.h"
#include "listitem.h"
#include "connectionslistmodel.h"

ListItemDelegate::ListItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void ListItemDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString value = index.model()->data(index, ConnectionsListModel::ConnectionNameRole).toString();
    qDebug() << value;
    ListItem *item = static_cast<ListItem*>(editor);
    item->setConnectionName(value);
}

void ListItemDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
