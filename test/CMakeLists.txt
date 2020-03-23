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
    "synchronization/slim_lock.cpp"
    "synchronization/wait.cpp"
    "synchronization/waitable_address.cpp")
set(THREAD_SRCS
    "thread/thread.cpp")
set(THREADPOOL_SRCS
    "threadpool/pool.cpp")
set(TIMING_SRCS
    "timing/filetime.cpp")

add_executable(handle_tests ${HANDLE_SRCS})
add_executable(io_tests ${IO_SRCS})
add_executable(synchronization_tests ${SYNCHRONIZATION_SRCS})
add_executable(thread_tests ${THREAD_SRCS})
add_executable(threadpool_tests ${THREADPOOL_SRCS})
add_executable(timing_tests ${TIMING_SRCS})
    
target_link_libraries(handle_tests PRIVATE catch_main wdl)
target_link_libraries(io_tests PRIVATE catch_main wdl)
target_link_libraries(synchronization_tests PRIVATE catch_main wdl)
target_link_libraries(thread_tests PRIVATE catch_main wdl)
target_link_libraries(threadpool_tests PRIVATE catch_main wdl)
target_link_libraries(timing_tests PRIVATE catch_main wdl)

catch_discover_tests(handle_tests)
catch_discover_tests(io_tests)
catch_discover_tests(synchronization_tests)
catch_discover_tests(thread_tests)
catch_discover_tests(threadpool_tests)
catch_discover_tests(timing_tests)