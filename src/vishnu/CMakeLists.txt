# must include drona.h also so that auto moc compiler works
add_library(vishnu src/vishnu.cpp include/vishnu/vishnu.h)

target_link_libraries(vishnu
    Qt5::Network
    Qt5::Core
    Qt5::Widgets
    shared::core
    katha::shunya
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(vishnu
    INTERFACE include
    PRIVATE  include/vishnu
)
add_library(katha::vishnu ALIAS vishnu)
