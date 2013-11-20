#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include <QtGui/QItemDelegate>

class ListItemDelegate : public QItemDelegate
{
        Q_OBJECT
    public:
        explicit ListItemDelegate(QObject *parent = 0);
        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void updateEditorGeometry(QWidget *editor,
            const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LISTITEMDELEGATE_H
