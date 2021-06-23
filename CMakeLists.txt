cmake_minimum_required(VERSION 3.13)

project(fsmonitor)

#################################
### Project Compilation Flags
#################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#################################
### PTHREADS
#################################
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#################################
### Add sources
#################################
set(SOURCES
    src/main.cpp
)

#################################
### Set Target
#################################
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

#################################
### Set Install
#################################
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
