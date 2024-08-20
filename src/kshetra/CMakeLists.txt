# must include kshetra.h also so that auto moc compiler works
add_library(kshetra src/kshetra.cpp include/kshetra/kshetra.h)

target_link_libraries(kshetra
    Qt5::Network
    Qt5::Core
    Qt5::Widgets
    shared::protobuf
    shared::core
    katha::shunya
    katha::yodha
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(kshetra
    INTERFACE include
    PRIVATE  include/kshetra
)
add_library(katha::kshetra ALIAS kshetra)

