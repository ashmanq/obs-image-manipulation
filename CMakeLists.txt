cmake_minimum_required(VERSION 3.10)
# Set the project name and version
project(image-manipulation VERSION 0.1)
set(PLUGIN_AUTHOR "ashmanq")
set(PLUGIN_GIT obs-image-manipulation)
set(CMAKE_CXX_STANDARD 11)


if (WIN32 OR APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
endif()

if(MSVC)
	set(image-manipulation_PLATFORM_DEPS
		w32-pthreads)
endif()

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    add_definitions(-DLINUX=1)
    add_definitions(-DUNIX=1)
endif ()

find_package(Libobs REQUIRED)

# add_executable(image-manipulation image-manipulation.cpp)

configure_file(image-manipulation.hpp.in ${CMAKE_SOURCE_DIR}/image-manipulation.hpp)

set(image-manipulation_SOURCES
    image-manipulation.cpp
    image-manipulation.hpp
)

add_library(image-manipulation MODULE
        ${image-manipulation_SOURCES}
)

target_link_libraries(image-manipulation 
    ${LIBOBS_LIBRARIES}
    ${image-manipulation_PLATFORM_DEPS}
)

include_directories("${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api"
    ${LIBOBS_INCLUDE_DIR}
)
