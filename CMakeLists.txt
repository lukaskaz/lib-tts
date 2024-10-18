cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
project(tts)

option(VCPKG_MANIFEST_MODE "Install vcpkg dependencies locally" ON)
option(RUN_TESTS "Creates and runs unit tests for the project" OFF)

include($ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake)
include(cmake/flags.cmake)
include(cmake/dependencies.cmake)


if(RUN_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(examples)

include_directories(inc)
file(GLOB SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} libshellcmd)
add_dependencies(${PROJECT_NAME} libnlohmann)
IF(NOT Boost_FOUND)
    add_dependencies(${PROJECT_NAME} libboost)
ENDIF()
target_link_libraries(${PROJECT_NAME} shellcmd CURL::libcurl google-cloud-cpp::texttospeech)
