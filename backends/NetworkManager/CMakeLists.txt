include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/internals
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/internals/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/service
    ${CMAKE_CURRENT_SOURCE_DIR}/settings
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(knm_nm_LIB_HEADERS

   nmdbusactiveconnectionmonitor.h

   #org.freedesktop.NetworkManager.Settings.Connection.Active
   nm-active-connectioninterface.h

   #org.freedesktop.NetworkManager.Settings.VPN.Connection
   nm-vpn-connectioninterface.h

   nmdbussettingsconnectionprovider.h
   nmdbussecretagent.h

   remoteconnection.h
   nm-settingsinterface.h
   nm-settings-connectioninterface.h
)

set(knm_nm_LIB_SOURCES

   connectiondbus.cpp

   settingdbus.cpp

   pbkdf2.cpp
   wephash.cpp

   settings/802-11-wireless-securitydbus.cpp

   settings/802-11-wirelessdbus.cpp

   settings/802-3-ethernetdbus.cpp

   settings/802-1xdbus.cpp

   settings/gsmdbus.cpp

   settings/cdmadbus.cpp

   settings/bluetoothdbus.cpp

   settings/ipv4dbus.cpp

   settings/ipv6dbus.cpp

   settings/pppdbus.cpp

   settings/pppoedbus.cpp

   settings/serialdbus.cpp

   settings/vpndbus.cpp

   nmdbusactiveconnectionmonitor.cpp

   #org.freedesktop.NetworkManager.Settings.Connection.Active
   nm-active-connectioninterface.cpp

   #org.freedesktop.NetworkManager.Settings.VPN.Connection
   nm-vpn-connectioninterface.cpp

   nmdbussettingsconnectionprovider.cpp
   nmdbussecretagent.cpp

   remoteconnection.cpp
   nm-settingsinterface.cpp
   nm-settings-connectioninterface.cpp

   ../../libs/service/events.cpp
)

link_directories( 
    ${MODEMMANAGERQT_LIBRARY_DIRS} 
    ${QTNETWORK_LIBRARY_DIRS} 
    ${NETWORKMANAGERQT_LIBRARY_DIRS} 
    ${CMAKE_LIBRARY_PATH} 
    ${CMAKE_CURRENT_BINARY_DIR}/../../libs/internals
    ${CMAKE_CURRENT_BINARY_DIR}/../../libs/client
    ${CMAKE_CURRENT_BINARY_DIR}/../../libs/service
)

qt4_wrap_cpp(MOC
    ${knm_nm_LIB_HEADERS}
)

add_library(
    knm_nm SHARED ${MOC} ${knm_nm_LIB_SOURCES} 
)
target_link_libraries(knm_nm ${QT_QTGUI_LIBRARY} ModemManagerQt NetworkManagerQt knminternals knmservice ${QT_QTNETWORK_LIBRARY})

set_target_properties(knm_nm PROPERTIES
   VERSION ${GENERIC_LIB_VERSION}
   SOVERSION ${GENERIC_LIB_SOVERSION}
   )
install(TARGETS knm_nm DESTINATION lib${LIB_SUFFIX})

