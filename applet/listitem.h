#ifndef LISTITEM_H
#define LISTITEM_H

#include <QtGui/QWidget>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
        Q_OBJECT

    public:
        explicit ListItem(QWidget *parent = 0);
        ~ListItem();

        void setConnectionName(const QString &);
        void setSignalStrength(int);
        void setActivationState(const QString &);
        void setConnectionIcon(const QString &);
    private:
        Ui::ListItem *ui;
};

#endif // LISTITEM_H
