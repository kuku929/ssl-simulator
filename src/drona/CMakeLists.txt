add_library(drona src/drona.cpp include/drona/drona.h
    include/drona/dhanush.h src/dhanush.cpp)

target_link_libraries(drona
    Qt5::Network
    Qt5::Core
    Qt5::Widgets
    shared::protobuf
    shared::core
    katha::yodha
    katha::voronoi
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(drona
    INTERFACE include
    PRIVATE  include/drona
    PRIVATE  include/dhanush
)
add_library(katha::drona ALIAS drona)
