/*
Copyright 2009 Paul Marchouk <pmarchouk@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) version 3, or any
later version accepted by the membership of KDE e.V. (or its
successor approved by the membership of KDE e.V.), which shall
act as a proxy defined in Section 6 of version 3 of the license.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KNM_EXTERNALS_WIRELESSINTERFACECONNECTIONHELPERS_P_H
#define KNM_EXTERNALS_WIRELESSINTERFACECONNECTIONHELPERS_P_H

#include <QString>

#include "interfaceconnectionhelpers_p.h"
#include <NetworkManagerQt/wirelessdevice.h>

class QObject;

namespace Knm
{

    class Connection;
    class WirelessInterfaceConnection;

    class WirelessInterfaceConnectionBuilder : public InterfaceConnectionBuilder
    {
        public:
            WirelessInterfaceConnectionBuilder(const NetworkManager::WirelessDevice::Ptr &interface,
                                               Knm::Connection *connection,
                                               const QString &deviceUni,
                                               QObject *parent);
            virtual ~WirelessInterfaceConnectionBuilder();

            Knm::WirelessInterfaceConnection *build();

        protected:
            void init(WirelessInterfaceConnection *ic);

        private:
            Q_DISABLE_COPY(WirelessInterfaceConnectionBuilder)

        protected:
            NetworkManager::WirelessDevice::Ptr m_interface;
    };

    class HiddenWirelessInterfaceConnectionBuilder : public WirelessInterfaceConnectionBuilder
    {
        public:
            HiddenWirelessInterfaceConnectionBuilder(const NetworkManager::WirelessDevice::Ptr &interface,
                    Knm::Connection *connection,
                    const QString &deviceUni,
                    QObject *parent);
            virtual ~HiddenWirelessInterfaceConnectionBuilder();

            Knm::HiddenWirelessInterfaceConnection *build();

        private:
            Q_DISABLE_COPY(HiddenWirelessInterfaceConnectionBuilder)

        protected:
            NetworkManager::WirelessDevice *m_interface;
    };

    class WirelessInterfaceConnectionSync : public InterfaceConnectionSync
    {
        public:
            WirelessInterfaceConnectionSync();
            virtual ~WirelessInterfaceConnectionSync();

            void sync(Knm::WirelessInterfaceConnection *interfaceConnection,
                      Knm::Connection *connection);

        private:
            Q_DISABLE_COPY(WirelessInterfaceConnectionSync)
    };

} //Knm

#endif // KNM_EXTERNALS_WIRELESSINTERFACECONNECTIONHELPERS_P_H
