CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(test_task)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")
include(cmake/build_prefix.cmake)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD 20)

SET(APP_NAME ${PROJECT_NAME})

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

SET(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(src)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} PRIVATE lamp server client)