cmake_policy(SET CMP0072 NEW)

# Set include directories
include_directories(${CMAKE_SOURCE_DIR}/source)

# Set source files
file(GLOB_RECURSE SOURCES "source/*.cpp" "source/*.h")

add_library(NVoronoi ${SOURCES})

# Find and link libraries
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# Link against the OpenGL, GLEW, GLUT, and GLU libraries
target_link_libraries(NVoronoi
    OpenGL::GL
    OpenGL::GLU
    GLEW::GLEW
    GLUT::GLUT
    Qt5::Network
    Qt5::Core
    Qt5::Widgets
)

target_include_directories(NVoronoi
    INTERFACE source
    PRIVATE source
)

add_library(katha::voronoi ALIAS NVoronoi)
