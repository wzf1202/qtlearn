set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

qt_add_executable(qtlearn
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(qtlearn PRIVATE Qt6::Widgets)

set(TS_FILES
    mui/zh_CN/task1.ts
)

foreach(ts_file IN LISTS TS_FILES)
    get_filename_component(ts_dir "${ts_file}" DIRECTORY)
    set_source_files_properties("${ts_file}" PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${ts_dir}")
endforeach()

qt_add_lupdate(qtlearn TS_FILES ${TS_FILES})

qt_add_lrelease(qtlearn TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES)

qt_add_resources(qtlearn "i18n"
    PREFIX "/i18n"
    BASE "${CMAKE_CURRENT_BINARY_DIR}"
    FILES ${QM_FILES}
)