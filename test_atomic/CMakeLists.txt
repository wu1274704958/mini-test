file(GLOB SOURCE_FILES "*.cpp")
set(BUILD_NAME "test_atomic")
include_directories("../include")
add_executable(${BUILD_NAME} ${SOURCE_FILES})