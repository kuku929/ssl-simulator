set(PROJECT_SOURCES
        main.cpp
        kuruk.cpp
        kuruk.h
        kuruk.ui
)

add_executable(kuruk ${PROJECT_SOURCES})

target_link_libraries(kuruk
    Qt::Widgets
    katha::vyasa
    katha::kshetra
    katha::shunya
    katha::drona
)

include(GNUInstallDirs)
install(TARGETS kuruk
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
