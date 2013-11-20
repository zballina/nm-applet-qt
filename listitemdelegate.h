#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include <QItemDelegate>

class ListItemDelegate : public QItemDelegate
{
        Q_OBJECT
    public:
        explicit ListItemDelegate(QObject *parent = 0);
        void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const;
    signals:

    public slots:

};

#endif // LISTITEMDELEGATE_H
