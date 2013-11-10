set( SOURCES_FILES
    main.cpp
    applet.cpp
    networkmanagermenu.cpp
    listconnections.cpp
    about.cpp
    infoconnection.cpp
    widgetinfoconnection.cpp
)

set( HEADERS_FILES
    applet.h
    networkmanagermenu.h
    listconnections.h
    about.h
    infoconnection.h
    widgetinfoconnection.h
)

set( RESOURCES_FILES
    ../resources.qrc
)

set( UIS_FILES
    listconnections.ui
    about.ui
    infoconnection.ui
)

include( ${QT_USE_FILE} )

qt4_wrap_ui(UIS ${UIS_FILES} )
qt4_wrap_cpp( HEADERS_MOC ${HEADERS_FILES} )
qt4_add_resources( RESOURCES_QRC ${RESOURCES_FILES} )

add_definitions( -Wall ${QT_DEFINITIONS} )
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QTNETWORK_INCLUDE_DIRS}
    ${NETWORKMANAGERQT_INCLUDE_DIRS}
    ${NETWORKMANAGER_INCLUDE_DIRS}
    qtxdg )

link_directories( ${QTNETWORK_LIBRARY_DIRS} ${NETWORKMANAGERQT_LIBRARY_DIRS} )

add_executable( ${PROJECT_NAME}
    ${HEADERS_FILES}
    ${SOURCES_FILES}
    ${UIS}
    ${RESOURCES_QRC}
    ${HEADERS_MOC}
)

target_link_libraries( ${PROJECT_NAME} NetworkManagerQt qtxdg ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTDBUS_LIBRARY} ${QT_QTNETWORK_LIBRARY} )
install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )
