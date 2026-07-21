add_executable(task2 WIN32 main.cpp)

target_link_libraries(task2 PRIVATE user32)

target_compile_definitions(task2 PRIVATE UNICODE _UNICODE)