# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

set(IO_SRCS
    "io/iocp.cpp")
set(SYNCHRONIZATION_SRCS
    "synchronization/critical_section.cpp"
    "synchronization/event.cpp"
    "synchronization/mutex.cpp")
set(HANDLE_SRCS
    "handle/generic.cpp")

add_executable(wdl_tests
    ${IO_SRCS}
    ${SYNCHRONIZATION_SRCS}
    ${UTILITY_SRCS})
    
target_link_libraries(wdl_tests PRIVATE catch_main wdl)

catch_discover_tests(wdl_tests)