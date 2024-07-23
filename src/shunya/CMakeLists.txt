# must include shunya.h also so that auto moc compiler works
add_library(shunya src/shunya.cpp include/shunya/shunya.h)

target_link_libraries(shunya
    Qt5::Network
    Qt5::Core
    Qt5::Widgets
    shared::protobuf
    shared::core
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(shunya
    INTERFACE include
    PRIVATE  include/shunya
)
add_library(katha::shunya ALIAS shunya)
