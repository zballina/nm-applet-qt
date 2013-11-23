include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/..
   ${CMAKE_CURRENT_SOURCE_DIR}/../internals
   ${CMAKE_CURRENT_BINARY_DIR}/../internals
   ${CMAKE_CURRENT_SOURCE_DIR}/../backends/NetworkManager
)

set(knmservice_HEADERS

   # list of all connections, maintains connectionhandlers
    connectionlist.h

   #defines interface for objects storing secrets
    secretsprovider.h

   # list of things that may be connected: connections and unconfigured networks
   activatablelist.h

   # maintains NIC activatable providers
   networkinterfacemonitor.h

   # relates connections to network interfaces
    networkinterfaceactivatableprovider.h

   # subclass of the above, uses ethernet carrier status to select activatables
    wirednetworkinterfaceactivatableprovider.h

   # complex class for managing connections and unconfigured networks for wireless interfaces
    wirelessnetworkinterfaceactivatableprovider.h

   # relates gsm connections to network interfaces
    gsmnetworkinterfaceactivatableprovider.h

   # configures unconfigured devices and new wireless networks on demand and connects them
    configurationlauncher.h

   # updates connection state as they are used
    connectionusagemonitor.h

   # exports activatables on the session bus to Plasmoids etc
    sessionabstractedservice.h

   # creates vpn activatables
    vpninterfaceconnectionprovider.h

   # sort logic for activatables
    sortedactivatablelist.h

   # dialog to ask for PIN code
   pindialog.h
)
   
set(knmservice_SOURCES
   # defines interface for objects observing connections
    connectionhandler.cpp

   # list of all connections, maintains connectionhandlers
    connectionlist.cpp

   # handles storage of secrets
    #secretstorage.cpp

   #defines interface for objects storing secrets
    secretsprovider.cpp

   # list of things that may be connected: connections and unconfigured networks
   activatablelist.cpp

   # maintains NIC activatable providers
   networkinterfacemonitor.cpp

   # relates connections to network interfaces
    networkinterfaceactivatableprovider.cpp

   # subclass of the above, uses ethernet carrier status to select activatables
    wirednetworkinterfaceactivatableprovider.cpp

   # complex class for managing connections and unconfigured networks for wireless interfaces
    wirelessnetworkinterfaceactivatableprovider.cpp

   # relates gsm connections to network interfaces
    gsmnetworkinterfaceactivatableprovider.cpp

   # interface for tracking activatable events
   activatableobserver.cpp

   # simple debug output observer
    #activatabledebug.cpp

   # configures unconfigured devices and new wireless networks on demand and connects them
    configurationlauncher.cpp

   # updates connection state as they are used
    connectionusagemonitor.cpp

   # exports activatables on the session bus to Plasmoids etc
    sessionabstractedservice.cpp

   # creates vpn activatables
    vpninterfaceconnectionprovider.cpp

   # performs KNotifications on events
    #notificationmanager.cpp

   # static global definitions of event string ids
   events.cpp

   # sort logic for activatables
    sortedactivatablelist.cpp

   # dialog to ask for PIN code
   pindialog.cpp
)

set(knmservice_UI_SOURCES
    forms/pinwidget.ui
)

qt4_add_dbus_adaptor(
    knmservice_SRCS
    interfaces/org.kde.networkmanagement.activatable.xml
    activatable.h
    Knm::Activatable
    activatableadaptor
    ActivatableAdaptor
)

qt4_add_dbus_adaptor(
    knmservice_SRCS
    interfaces/org.kde.networkmanagement.interfaceconnection.xml
    interfaceconnection.h
    Knm::InterfaceConnection
    interfaceconnectionadaptor
    InterfaceConnectionAdaptor
)

qt4_add_dbus_adaptor(
    knmservice_SRCS
    interfaces/org.kde.networkmanagement.wirelessinterfaceconnection.xml
    wirelessinterfaceconnection.h
    Knm::WirelessInterfaceConnection
    wirelessinterfaceconnectionadaptor
    WirelessInterfaceConnectionAdaptor
)

qt4_add_dbus_adaptor(
    knmservice_SRCS
    interfaces/org.kde.networkmanagement.wirelessnetwork.xml
    wirelessnetwork.h
    Knm::WirelessNetwork
    wirelessnetworkadaptor
    WirelessNetworkAdaptor
)

qt4_add_dbus_adaptor(
    knmservice_SRCS
    interfaces/org.kde.networkmanagement.gsminterfaceconnection.xml
    gsminterfaceconnection.h
    Knm::GsmInterfaceConnection
    gsminterfaceconnectionadaptor
    GsmInterfaceConnectionAdaptor
)

qt4_add_dbus_adaptor(knmservice_SRCS
   interfaces/org.kde.networkmanagement.xml
   sessionabstractedservice.h
   SessionAbstractedService
   sessionabstractedserviceadaptor
   SessionAbstractedServiceAdaptor
)

include( ${QT_USE_FILE} )


qt4_wrap_ui(UI_FILES ${knmservice_UI_SOURCES})
qt4_wrap_cpp( HEADERS_MOC ${knmservice_HEADERS})

link_directories( 
    ${MODEMMANAGERQT_LIBRARY_DIRS} 
    ${QTNETWORK_LIBRARY_DIRS} 
    ${NETWORKMANAGERQT_LIBRARY_DIRS} 
    ${CMAKE_LIBRARY_PATH} 
    ${CMAKE_CURRENT_BINARY_DIR}/../libs/internals
)

add_library(knmservice 
    SHARED 
    ${knmservice_SRCS} 
    ${UI_FILES}
    ${HEADERS_MOC} 
    ${knmservice_SOURCES}
)

target_link_libraries(knmservice 
    knminternals 
    ModemManagerQt 
    NetworkManagerQt 
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTDBUS_LIBRARY} 
    ${QT_QTGUI_LIBRARY}
)

set_target_properties(knmservice PROPERTIES
   VERSION ${GENERIC_LIB_VERSION}
   SOVERSION ${GENERIC_LIB_SOVERSION}
)

install(TARGETS knmservice DESTINATION lib${LIB_SUFFIX})
