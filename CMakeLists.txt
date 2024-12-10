cmake_minimum_required(VERSION 3.14)
project(TgBot)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW) # find_package() uses <PackageName>_ROOT variables
endif()

# options
option(ENABLE_TESTS "Set to ON to enable building of tests" OFF)
option(BUILD_SHARED_LIBS "Build tgbot-cpp shared/static library." OFF)
option(BUILD_DOCUMENTATION "Build doxygen API documentation." OFF)

set(SRC_LIST
    src/Api.cpp
    src/Bot.cpp
    src/EventHandler.cpp
    src/TgTypeParser.cpp
    src/net/BoostSslClient.cpp
    src/net/CurlHttpClient.cpp
    src/net/HttpParser.cpp
    src/net/TgLongPoll.cpp
    src/net/Url.cpp
    src/tools/StringTools.cpp)

# building project
add_library(${PROJECT_NAME} ${SRC_LIST})

# sources
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0601)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TGBOT_LP64)
endif()

# libs
## threads
find_package(Threads REQUIRED)

## zlib
find_package(ZLIB REQUIRED)

## openssl
find_package(OpenSSL REQUIRED)

## curl
find_package(CURL 7.58.0)
if (CURL_FOUND)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DHAVE_CURL)
endif()

## jsoncpp
if (NOT TARGET JsonCpp::JsonCpp)
    find_package(jsoncpp)
    if (NOT TARGET JsonCpp::JsonCpp)
        message(STATUS "Using alternative findjsoncpp")
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
        find_package(JsonCppCustom)
        if (NOT TARGET JsonCpp::JsonCpp)
            message(FATAL_ERROR "Jsoncpp is not available")
        endif()
    endif()
endif()
install(FILES CMake/FindJsonCppCustom.cmake
    DESTINATION lib/cmake/tgbot-cpp
)

## boost
set(Boost_USE_MULTITHREADED ON)
if (ENABLE_TESTS)
    find_package(Boost 1.70 CONFIG COMPONENTS system unit_test_framework REQUIRED)
else()
    find_package(Boost 1.70 CONFIG COMPONENTS system REQUIRED)
endif()
if(NOT Boost_USE_STATIC_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_ALL_DYN_LINK)
endif()
target_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_BIND_GLOBAL_PLACEHOLDERS)

set(LIB_LIST
    Threads::Threads
    ZLIB::ZLIB
    OpenSSL::SSL
    Boost::system
    JsonCpp::JsonCpp
)

if (CURL_FOUND)
    list(APPEND LIB_LIST CURL::libcurl)
endif()

if (WIN32)
    list(APPEND LIB_LIST Ws2_32)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC 
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_include_directories(${PROJECT_NAME} PUBLIC ${jsoncpp_INCLUDE_DIRS} ${JSONCPP_PKG_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        FILE ${PROJECT_NAME}Config.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

# ABI version
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION 1)

# tests
if (ENABLE_TESTS)
    message(STATUS "Building of tests is enabled")
    enable_testing()
    add_subdirectory(test)
endif()

# Documentation
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
        install(DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/ TYPE DOC)
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TGBOT_DLL)
endif()
