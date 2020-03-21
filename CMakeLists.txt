# CMakeLists.txt: wdl

cmake_minimum_required(VERSION 3.15)

project(wdl CXX)

option(WDL_ENABLE_TESTING "Enable testing" OFF)
option(WDL_HEADER_ONLY "Do not link against static library" ON)

# building with tests requires static library
# if(WDL_ENABLE_TESTING)
#     set(WDL_HEADER_ONLY OFF)
# endif()

# set(WDL_STATIC_LIB_SRCS
#     "src/synchronization/wait.cpp")

if (WDL_HEADER_ONLY)
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(
        ${PROJECT_NAME} 
        INTERFACE 
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>)
    target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
else()
    add_library(${PROJECT_NAME} ${WDL_STATIC_LIB_SRCS})
    target_include_directories(
        ${PROJECT_NAME} 
        PUBLIC 
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>)
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
endif()

if(WDL_ENABLE_TESTING)
    enable_testing()
    message("Building tests...")
    add_subdirectory(test)
    add_subdirectory(deps/catch2)
endif()