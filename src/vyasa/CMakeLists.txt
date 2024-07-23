# must include drona.h also so that auto moc compiler works
add_library(vyasa src/vyasa.cpp include/vyasa/vyasa.h)

target_link_libraries(vyasa
    Qt5::Network
    Qt5::Core
    Qt5::Widgets
    shared::core
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(vyasa
    INTERFACE include
    PRIVATE  include/vyasa
)
add_library(katha::vyasa ALIAS vyasa)

