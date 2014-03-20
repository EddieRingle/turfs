set(android_read_test_SOURCES android_read_test.c android_native_app_glue.c android_native_app_glue.h)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -landroid -llog" CACHE STRING "linker flags" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -landroid -llog" CACHE STRING "linker flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -landroid -llog" CACHE STRING "linker flags" FORCE)

add_library(android_read_test SHARED ${android_read_test_SOURCES})
add_custom_command(TARGET android_read_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/assets"
            "$<TARGET_FILE_DIR:android_read_test>/assets"
)
target_link_libraries(android_read_test turfs)
message(STATUS "Linked against ${CMAKE_SHARED_LINKER_FLAGS}")

install(TARGETS android_read_test
    BUNDLE DESTINATION bin
    LIBRARY DESTINATION bin/android_read_test
)



