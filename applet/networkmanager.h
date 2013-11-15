#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QWidget>

namespace Ui
{
    class NetworkManagerApplet;
}

class NetworkManagerApplet : public QWidget
{
        Q_OBJECT

    public:
        explicit NetworkManagerApplet(QWidget *parent = 0);
        ~NetworkManagerApplet();

    private:
        Ui::NetworkManagerApplet *ui;
};

#endif // NETWORKMANAGER_H
