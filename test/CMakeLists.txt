cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)

include(CTest)

add_definitions(-std=c++17)
include_directories(../include
                    /C/my_files/boost_1_68_0/boost_1_68_0
                    )
link_directories(
    /C/my_files/boost_1_68_0/boost_1_68_0/stage/lib
)
link_libraries( 
    libboost_unit_test_framework.a
)
add_executable(${PROJECT_NAME} test.cpp ../src/tool.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)