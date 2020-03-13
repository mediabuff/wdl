# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

add_executable(wdl_tests)
    
target_link_libraries(wdl_tests PRIVATE catch_main wdl)

catch_discover_tests(wdl_tests)