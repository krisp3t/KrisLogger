cmake_minimum_required(VERSION 3.10...3.21)

set(KRISLOGGER_VERSION "0.0.1")
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
        @ONLY
)

project(KrisLogger LANGUAGES CXX)
message(STATUS "Building KrisLogger version: ${KRISLOGGER_VERSION}")

# TODO: debug, release builds

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

file(GLOB SRCS "src/*.cpp")
file(GLOB HDRS "include/*.h")

# TODO: static or shared library
add_library(${PROJECT_NAME} STATIC ${SRCS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            src
)

if(WIN32)
    target_compile_definitions(KrisLogger PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if (ANDROID)
    find_library(LOG_LIB log)
    if(LOG_LIB)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${LOG_LIB})
    else()
        message(FATAL_ERROR "log library (for Android) not found")
    endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(DIRECTORY include/
        DESTINATION include)




