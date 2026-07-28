find_package(Threads REQUIRED)

add_executable(thread_testparallelcal testparallelcal.cpp)
target_link_libraries(thread_testparallelcal PRIVATE Threads::Threads)