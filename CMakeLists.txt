cmake_minimum_required(VERSION 3.22.0)
project(kuruk VERSION 1.0)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# flags for Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# flags for g++
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED YES)
SET(CMAKE_CXX_EXTENSIONS ON)

# for downloading the required libs
add_custom_target(download)

# to use sanitize_env() idk what that does
include(EnvHelper)

find_package(OpenGL REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
sanitize_env()
find_package(Protobuf 3.3.0)
restore_env()

# to compile Qt meta objects automatically, so peaceee
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Widgets Network OpenGL REQUIRED)

# place to download stuff
set(DEPENDENCY_DOWNLOADS "${CMAKE_BINARY_DIR}/dependencies")

include(BuildBullet)
include(BuildEigen)
include(BuildGoogleTest)
include(GetGameController)

# executables are created here
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# compiling go brrr
add_subdirectory(src)
