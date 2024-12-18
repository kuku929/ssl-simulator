add_library(varma src/visualize.cpp include/varma/visualize.h)

target_link_libraries(varma
    Qt5::Core
    Qt5::Widgets
)

# this will allow the linker to find .h files in other packages which are linked
target_include_directories(varma
    INTERFACE include
    PRIVATE  include/varma
)
add_library(katha::varma ALIAS varma)
