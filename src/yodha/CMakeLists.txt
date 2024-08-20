# must include kshetra.h also so that auto moc compiler works
add_library(yodha src/yodha.cpp include/yodha/yodha.h)

target_link_libraries(yodha
    Qt5::Core
    Qt5::Widgets
    katha::shunya
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(yodha
    INTERFACE include
    PRIVATE  include/yodha
)
add_library(katha::yodha ALIAS yodha)
