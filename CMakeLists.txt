cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(image-manipulation VERSION 0.1)

find_package(Libobs REQUIRED)

add_executable(image-manipulation image-manipulation.cpp)

configure_file(image-manipulation.h.in image-manipulation.h)
target_include_directories(image-manipulation PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
include_directories("${LIBOBS_INCLUDE_DIR}/../UI/obs-frontend-api"
    ${LIBOBS_INCLUDE_DIR}
)
