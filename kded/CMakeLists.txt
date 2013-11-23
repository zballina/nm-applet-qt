include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/internals
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/service
   # this will become optional
   ${CMAKE_CURRENT_SOURCE_DIR}/../backends/NetworkManager

   # for ImportNm08Connections
   ${CMAKE_CURRENT_BINARY_DIR}/../libs/internals
)
   ## next target: test service

set(kded_networkmanagement_SOURCES
    service.cpp
)

set(kded_networkmanagement_HEADERS
   service.h
)

qt4_wrap_cpp(MOC
    ${kded_networkmanagement_HEADERS}
)

add_library(kded_networkmanagement SHARED ${MOC} ${kded_networkmanagement_SOURCES})

target_link_libraries(kded_networkmanagement
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTDBUS_LIBRARY} 
    ${QT_QTNETWORK_LIBRARY}
    knminternals
    knmservice
    knm_nm
    NetworkManagerQt
)

set_target_properties(kded_networkmanagement PROPERTIES
   VERSION ${GENERIC_LIB_VERSION}
   SOVERSION ${GENERIC_LIB_SOVERSION}
)
   
install(TARGETS kded_networkmanagement DESTINATION lib${LIB_SUFFIX})

