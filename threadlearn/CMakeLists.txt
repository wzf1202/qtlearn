find_package(Threads REQUIRED)

option(THREADLEARN_BUILD_PARALLEL "Build testparallelcal" OFF)
option(THREADLEARN_BUILD_PRODUCER "Build producer_consumer" ON)

if(THREADLEARN_BUILD_PARALLEL)
    add_executable(thread_testparallelcal testparallelcal.cpp)
    target_link_libraries(thread_testparallelcal PRIVATE Threads::Threads)
endif()

if(THREADLEARN_BUILD_PRODUCER)
    add_executable(thread_producer_consumer producer_consumer.cpp)
    target_link_libraries(thread_producer_consumer PRIVATE Threads::Threads)
endif()