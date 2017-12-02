cmake_minimum_required(VERSION 2.8.11)
project(Utility)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

GET_FILENAME_COMPONENT( SOURCE_ROOT . ABSOLUTE )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin/${BIN_PATH_POSTFIX})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin/${BIN_PATH_POSTFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOURCE_ROOT}/../bin/${BIN_PATH_POSTFIX})

add_compile_options(--std=c++14)
add_compile_options(-Wall)
add_compile_options(-Werror)

include_directories(${SOURCE_ROOT})

set(Boost_USE_STATIC_LIBS   ON)
FIND_PACKAGE(Boost 1.54.0 REQUIRED COMPONENTS system filesystem program_options)

find_package(Threads REQUIRED)   # We require thread support
include(ExternalProject)         # Enable ExternalProject module
ExternalProject_Add(
    gmock
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gmock
    INSTALL_COMMAND "" # Disable install step
)
ExternalProject_Get_Property(gmock source_dir binary_dir)
SET(GMOCK_SOURCE_DIR ${source_dir})
SET(GMOCK_BINARY_DIR ${binary_dir})

add_library(libgtest IMPORTED STATIC GLOBAL)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${GMOCK_BINARY_DIR}/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

add_library(libgmock IMPORTED STATIC GLOBAL)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${GMOCK_BINARY_DIR}/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")

SET(GMOCK_INCLUDE_DIRECTORIES
   "${GMOCK_SOURCE_DIR}/googlemock/include"
   "${GMOCK_SOURCE_DIR}/googletest/include")

SET(GMOCK_ALL_LIBRARIES 
   "${GMOCK_BINARY_DIR}/googlemock/gtest/libgtest.a" 
   "${GMOCK_BINARY_DIR}/googlemock/gtest/libgtest_main.a" 
   "${GMOCK_BINARY_DIR}/googlemock/libgmock.a" 
   "${GMOCK_BINARY_DIR}/googlemock/libgmock_main.a"
   pthread)

add_subdirectory(Lib)
add_subdirectory(App)
add_subdirectory(Test)
