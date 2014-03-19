set(test1_SOURCES test1.c)

if(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_OSX)
    add_executable(test1 MACOSX_BUNDLE ${test1_SOURCES})
    set_target_properties(test1 PROPERTIES
        MACOSX_BUNDLE_NAME "test1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${TURFS_VERSION_FULL}
        MACOSX_BUNDLE_LONG_VERSION_STRING ${TURFS_VERSION_FULL}
    )
    add_custom_command(TARGET test1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_LIST_DIR}/assets"
                "$<TARGET_FILE_DIR:test1>/../Resources/assets"
    )
else()
    add_executable(test1 ${test1_SOURCES})
    add_custom_command(TARGET test1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_LIST_DIR}/assets"
                "$<TARGET_FILE_DIR:test1>/assets"
    )
endif()
target_link_libraries(test1 turfs)

install(TARGETS test1
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin/test1
)



