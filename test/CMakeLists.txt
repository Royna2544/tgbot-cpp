set(TEST_SRC_LIST
    main.cpp
    tgbot/net/Url.cpp
    tgbot/net/HttpParser.cpp
    tgbot/tools/StringTools.cpp
)

include_directories("${PROJECT_SOURCE_DIR}/test")
add_executable(${PROJECT_NAME}_test ${TEST_SRC_LIST})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} Boost::unit_test_framework)
add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)

set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_STANDARD_REQUIRED 17)
set_target_properties(${PROJECT_NAME}_test PROPERTIES CXX_EXTENSIONS OFF)
