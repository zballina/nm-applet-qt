project(nm-applet-qt)
cmake_minimum_required(VERSION 2.8)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)
set(SO_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(-DSO_VERSION=\"${SO_VERSION}\")
message(STATUS "")
message(STATUS "NetworkManager Applet-Qt version: ${SO_VERSION}")

set(MAJOR_VERSION 4)
set(MINOR_VERSION 10)
set(PATCH_VERSION 0)
set(GENERIC_LIB_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
add_definitions(-DGENERIC_LIB_VERSION=\"${GENERIC_LIB_VERSION}\")
message(STATUS "Generic Library version: ${GENERIC_LIB_VERSION}")
message(STATUS "")

set(GENERIC_LIB_SOVERSION ${MAJOR_VERSION})
add_definitions(-DGENERIC_LIB_SOVERSION=\"${GENERIC_LIB_SOVERSION}\")

set (MINIMUM_NM_VERSION_REQUIRED "0.9.8")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(FindPkgConfig)

find_package( Qt4 REQUIRED QtCore QtGui QtDBus QtXml )

pkg_check_modules(MODEMMANAGERQT REQUIRED ModemManagerQt)
if (NOT MODEMMANAGERQT_FOUND)
    message(FATAL_ERROR "ERROR: ModemManagerQt not found. http://projects.kde.org/search?q=modemmanager&scope=all&all_words=1&projects=1&submit=Submit")
endif (NOT MODEMMANAGERQT_FOUND)

pkg_check_modules(NETWORKMANAGERQT REQUIRED NetworkManagerQt)
if (NOT NETWORKMANAGERQT_FOUND)
    message(FATAL_ERROR "ERROR: NetworkManagerQt not found. http://projects.kde.org/search?q=networkmanager&scope=all&all_words=1&projects=1&submit=Submit")
endif (NOT NETWORKMANAGERQT_FOUND)

add_definitions(${QT_DEFINITIONS} -DWITH_MODEMMANAGERQT=1)
include_directories(${MODEMMANAGERQT_INCLUDE_DIRS} ${NETWORKMANAGERQT_INCLUDE_DIRS} ${CMAKE_INCLUDE_PATH})
link_directories(${MODEMMANAGERQT_LIBRARY_DIRS} ${NETWORKMANAGERQT_LIBRARY_DIRS} ${CMAKE_LIBRARY_PATH})

find_package(NetworkManager ${MINIMUM_NM_VERSION_REQUIRED})

if (NETWORKMANAGER_FOUND)
    PKG_SEARCH_MODULE( NM-UTIL libnm-util )
    if (NOT NM-UTIL_FOUND)
        message(FATAL_ERROR "Could NOT find libnm-util, check FindPkgConfig output above!")
    endif (NOT NM-UTIL_FOUND)

    configure_file(backends/NetworkManager/config-nm09backend.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/backends/NetworkManager/config-nm09backend.h )

    if (${NETWORKMANAGER_VERSION} VERSION_GREATER ${MINIMUM_NM_VERSION_REQUIRED} OR
        ${NETWORKMANAGER_VERSION} VERSION_EQUAL ${MINIMUM_NM_VERSION_REQUIRED})
        set(NM_0_9 TRUE)
        add_definitions(-DMINIMUM_NM_VERSION_REQUIRED=\"${MINIMUM_NM_VERSION_REQUIRED}\")
        include_directories(
            ${NETWORKMANAGER_INCLUDE_DIRS}
            ${NM-UTIL_INCLUDE_DIRS}
            ${CMAKE_CURRENT_BUILD_DIR}
            ${QT_INCLUDES}
        )

        pkg_check_modules( QTNETWORK REQUIRED QtNetwork )
        include(${QT_USE_FILE})

        add_subdirectory(applet)
        add_subdirectory(backends)
        add_subdirectory(libs)
        add_subdirectory(kded)

    else()
        message(FATAL_ERROR "ERROR: NetworkManager version '${NETWORKMANAGER_VERSION}' does not match minimum required (${MINIMUM_NM_VERSION_REQUIRED})")
    endif()
else(NETWORKMANAGER_FOUND)
    message(FATAL_ERROR "ERROR: NetworkManager headers >= ${MINIMUM_NM_VERSION_REQUIRED} not installed. http://projects.gnome.org/NetworkManager/")
endif(NETWORKMANAGER_FOUND)
