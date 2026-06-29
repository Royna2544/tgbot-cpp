cmake_minimum_required(VERSION 3.14)
project(TgBot)

if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW) # find_package() uses <PackageName>_ROOT variables
endif()

# options
option(ENABLE_TESTS "Set to ON to enable building of tests" OFF)
option(BUILD_SHARED_LIBS "Build tgbot-cpp shared/static library." OFF)
option(BUILD_DOCUMENTATION "Build doxygen API documentation." OFF)

# libs
## threads
find_package(Threads REQUIRED)

## zlib
find_package(ZLIB REQUIRED)

## openssl
find_package(OpenSSL REQUIRED)

## nlohmann_json
find_package(nlohmann_json 3.2.0 REQUIRED)

file(GLOB SRC_LIST CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/net/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/types/*.cpp"
)

# building project
add_library(${PROJECT_NAME} ${SRC_LIST})

# sources
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX)
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TGBOT_LP64)
endif()

# Tests use Boost.Test; the library itself no longer depends on Boost.
if (ENABLE_TESTS)
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost 1.70 CONFIG COMPONENTS unit_test_framework REQUIRED)
endif()

set(LIB_LIST
    Threads::Threads
    ZLIB::ZLIB
    OpenSSL::SSL
    OpenSSL::Crypto
    nlohmann_json::nlohmann_json
)

if (WIN32)
    list(APPEND LIB_LIST Ws2_32)
endif()

# On macOS the vendored cpp-httplib loads root certificates from the system
# keychain (via the Security framework), so consumers must link these.
if (APPLE)
    list(APPEND LIB_LIST "-framework CoreFoundation" "-framework Security")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
# cpp-httplib is vendored (single header) and fully hidden behind the library
# (pimpl), so it is only needed when building the library itself.
target_include_directories(${PROJECT_NAME} PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_LIST})
include(GNUInstallDirs)
include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
                       BASE_NAME TGBOT
                       EXPORT_MACRO_NAME TGBOT_API
                       NO_EXPORT_MACRO_NAME TGBOT_NO_EXPORT
                       DEPRECATED_MACRO_NAME TGBOT_DEPRECATED
                       STATIC_DEFINE TGBOT_STATIC_DEFINE
                       INCLUDE_GUARD_NAME TGBOT_EXPORT_H
                       EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/tgbot/export.h)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TGBOT_STATIC_DEFINE)
endif()

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/tgbot/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tgbot)
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
