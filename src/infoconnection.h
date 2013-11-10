#ifndef INFOCONNECTION_H
#define INFOCONNECTION_H

#include <QtGui/QDialog>

namespace Ui {
class InfoConnection;
}

class InfoConnection : public QDialog
{
    Q_OBJECT

public:
    explicit InfoConnection(QWidget *parent = 0);
    ~InfoConnection();

private:
    Ui::InfoConnection *ui;
};

#endif // INFOCONNECTION_H
