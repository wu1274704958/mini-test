cmake_minimum_required(VERSION 3.0.0)
project(hello VERSION 0.1.0)

include(CTest)


include_directories(./include)
if(MSVC)
    add_definitions(/std:c++17 /permissive-)
else()
	add_definitions(-std=c++17)
	SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
	SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
    
endif()

add_executable(hello hello.cpp src/tool.cpp)
add_executable(t2 src/t2.cpp)
add_executable(t3 src/t3.cpp)
add_executable(t4 src/t4.cpp)
add_executable(t5 src/t5.cpp)
add_executable(t_matrix src/matrix.cpp)
add_subdirectory(test_atomic)
add_executable(test_rgba src/test_rgb.cpp)
add_executable(test_comm src/test_comm.cpp)
add_executable(t_matrix2 src/test_matrix.cpp)

#add_subdirectory(test)
#enable_testing()
#add_test(NAME my_test COMMAND Test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
