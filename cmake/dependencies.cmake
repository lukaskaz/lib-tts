cmake_minimum_required(VERSION 3.10)

find_package(Boost 1.74.0 COMPONENTS filesystem)
find_package(CURL REQUIRED)
find_package(google_cloud_cpp_texttospeech REQUIRED)

include(ExternalProject)

set(source_dir "${CMAKE_BINARY_DIR}/libshellcmd-src")
set(build_dir "${CMAKE_BINARY_DIR}/libshellcmd-build")

EXTERNALPROJECT_ADD(
  libshellcmd
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-shellcmd.git
  GIT_TAG           main
  PATCH_COMMAND     ${patching_cmd}
  PREFIX            libshellcmd-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/inc)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_BINARY_DIR}/libnlohmann-src")
set(build_dir "${CMAKE_BINARY_DIR}/libnlohmann-build")

EXTERNALPROJECT_ADD(
  libnlohmann
  GIT_REPOSITORY    https://github.com/nlohmann/json.git
  GIT_TAG           v3.11.3
  PATCH_COMMAND     ${patching_cmd}
  PREFIX            libnlohmann-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

include_directories(${source_dir}/include)
link_directories(${build_dir}/build)

IF(NOT Boost_FOUND)
set(source_dir "${CMAKE_BINARY_DIR}/libboost-src")
set(build_dir "${CMAKE_BINARY_DIR}/libboost-build")
set(include_dir "${build_dir}/include")

EXTERNALPROJECT_ADD(
  libboost
  URL               "https://archives.boost.io/release/1.74.0/source/boost_1_74_0.tar.gz"
  PATCH_COMMAND     ""
  PREFIX            libboost-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   mkdir ${include_dir} &&
    cp -a ${source_dir}/boost ${include_dir}
  TEST_COMMAND      ""
)

include_directories(${include_dir})
ENDIF()
