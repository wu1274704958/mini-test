cmake_minimum_required(VERSION 3.0.0)
project(hello VERSION 0.1.0)

include(CTest)


include_directories(./include)
if(MSVC)
    add_definitions(/std:c++17 /permissive)
else()
    add_definitions(-std=c++17)
endif()

add_executable(hello hello.cpp src/tool.cpp)
add_subdirectory(test)
enable_testing()
add_test(NAME my_test COMMAND Test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
