# CMakeLists.txt: wdl

cmake_minimum_required(VERSION 3.15)

project(wdl CXX)

option(WDL_ENABLE_TESTING "Enable testing" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(
    ${PROJECT_NAME} 
    INTERFACE 
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if(WDL_ENABLE_TESTING)
    enable_testing()
    message("Building tests...")
    add_subdirectory(test)
    add_subdirectory(deps/catch2)
endif()