cmake_minimum_required(VERSION 3.10...3.21)

project(KrisLogger VERSION ${KRISLOGGER_VERSION} LANGUAGES CXX)
message(STATUS "Building KrisLogger version: ${KRISLOGGER_VERSION}")

# TODO: debug, release builds

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(SRCS "src/*.cpp")
set(HDRS "include/*.h")

# TODO: static or shared library
add_library(${PROJECT_NAME}} STATIC ${SRCS})

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
    find_library(log-lib log)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${log-lib})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${KRISLOGGER_VERSION}
        SOVERSION ${KRISLOGGER_VERSION}
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




