set(read_test_SOURCES read_test.c)

if(TURFS_TARGET_PLATFORM STREQUAL TURFS_TARGET_PLATFORM_OSX)
    add_executable(read_test MACOSX_BUNDLE ${read_test_SOURCES})
    set_target_properties(read_test PROPERTIES
        MACOSX_BUNDLE_NAME "read_test"
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${TURFS_VERSION_FULL}
        MACOSX_BUNDLE_LONG_VERSION_STRING ${TURFS_VERSION_FULL}
    )
    add_custom_command(TARGET read_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_LIST_DIR}/assets"
                "$<TARGET_FILE_DIR:read_test>/../Resources/assets"
    )
else()
    add_executable(read_test ${read_test_SOURCES})
    add_custom_command(TARGET read_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_LIST_DIR}/assets"
                "$<TARGET_FILE_DIR:read_test>/assets"
    )
endif()
target_link_libraries(read_test turfs)

install(TARGETS read_test
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin/read_test
)



