# test/CMakeLists.txt

# pull in catch_discover_tests()
include(${CMAKE_SOURCE_DIR}/deps/catch2/contrib/Catch.cmake)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

set(HANDLE_SRCS
    "handle/generic.cpp")
set(IO_SRCS
    "io/iocp.cpp")
set(SYNCHRONIZATION_SRCS
    "synchronization/barrier.cpp"
    "synchronization/critical_section.cpp"
    "synchronization/event.cpp"
    "synchronization/mutex.cpp"
    "synchronization/slim_lock.cpp")
set(THREAD_SRCS
    "thread/thread.cpp")
set(THREADPOOL_SRCS
    "threadpool/pool.cpp")

add_executable(wdl_test_suite
    ${HANDLE_SRCS}
    ${IO_SRCS}
    ${SYNCHRONIZATION_SRCS}
    ${THREAD_SRCS}
    ${THREADPOOL_SRCS})
    
target_link_libraries(wdl_test_suite PRIVATE catch_main wdl)

catch_discover_tests(wdl_test_suite)