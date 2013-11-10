project(nmapplet-qt)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(FindPkgConfig)

find_package( Qt4 REQUIRED QtCore QtGui QtDBus )
find_package( NetworkManager ${MINIMUM_NM_VERSION_REQUIRED} )
pkg_check_modules( NETWORKMANAGERQT REQUIRED NetworkManagerQt )

if ( NOT NETWORKMANAGERQT_FOUND )
    message(FATAL_ERROR "ERROR: NetworkManagerQt not found. http://projects.kde.org/search?q=networkmanager&scope=all&all_words=1&projects=1&submit=Submit")
endif ( NOT NETWORKMANAGERQT_FOUND )

pkg_check_modules( QTNETWORK REQUIRED QtNetwork )
pkg_check_modules( NETWORKMANAGER REQUIRED NetworkManager )

include(${QT_USE_FILE})
add_subdirectory( src )
