include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs/client
    ${CMAKE_CURRENT_SOURCE_DIR}/../libs/internals
    ${CMAKE_CURRENT_SOURCE_DIR}/../backends/NetworkManager
    ${CMAKE_CURRENT_BINARY_DIR}/../libs/client
    ${CMAKE_CURRENT_BINARY_DIR}/../libs/internals
    ${CMAKE_CURRENT_BINARY_DIR}/../backends/NetworkManager
    ${QTNETWORK_INCLUDE_DIRS}
    ${NETWORKMANAGERQT_INCLUDE_DIRS}
    ${NETWORKMANAGER_INCLUDE_DIRS}
    ${MODEMMANAGERQT_INCLUDE_DIRS}
)

set( SOURCES_FILES
    main.cpp
    about.cpp
    networkmanager.cpp
    connectionslistmodel.cpp
    connectionitem.cpp
    declarativeinterfaceitem.cpp
    interfaceslistmodel.cpp
    nmdialog.cpp
    ../libs/service/events.cpp
)

set( HEADERS_FILES
    about.h
    networkmanager.h
    connectionslistmodel.h
    connectionitem.h
    declarativeinterfaceitem.h
    interfaceslistmodel.h
    nmdialog.h
)

set( RESOURCES_FILES
    resources.qrc
)

set( UIS_FILES
    ui/about.ui
    ui/nmdialog.ui
)

include( ${QT_USE_FILE} )

qt4_wrap_ui(UIS ${UIS_FILES} )
qt4_wrap_cpp( HEADERS_MOC ${HEADERS_FILES} )
qt4_add_resources( RESOURCES_QRC ${RESOURCES_FILES} )

add_definitions( -Wall ${QT_DEFINITIONS} -DWITH_MODEMMANAGERQT=1)
link_directories( 
    ${MODEMMANAGERQT_LIBRARY_DIRS} 
    ${QTNETWORK_LIBRARY_DIRS} 
    ${NETWORKMANAGERQT_LIBRARY_DIRS} 
    ${CMAKE_LIBRARY_PATH} 
    ${CMAKE_CURRENT_BINARY_DIR}/../libs/internals
    ${CMAKE_CURRENT_BINARY_DIR}/../libs/client
)
add_executable( ${PROJECT_NAME}
    ${HEADERS_FILES}
    ${SOURCES_FILES}
    ${UIS}
    ${RESOURCES_QRC}
    ${HEADERS_MOC}
)

target_link_libraries( ${PROJECT_NAME} NetworkManagerQt knmclient knm_nm knminternals ModemManagerQt ${QT_LIBRARIES} ${QT_QTDECLARATIVE_LIBRARY} )
install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )
