#ifndef LISTCONNECTIONS_H
#define LISTCONNECTIONS_H

#include <QtGui/QDialog>

namespace Ui {
class ListConnections;
}

class ListConnections : public QDialog
{
    Q_OBJECT

public:
    explicit ListConnections(QWidget *parent = 0);
    ~ListConnections();

private:
    Ui::ListConnections *ui;
};

#endif // LISTCONNECTIONS_H
