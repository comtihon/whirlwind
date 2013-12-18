cmake_minimum_required(VERSION 2.6)
project (whirlwind_test)
 
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
 
set(TEST_SOURCE
${PROJECT_SOURCE_DIR}/main.c
)
 
include_directories("${PROJECT_BINARY_DIR}")
add_executable(${PROJECT_NAME} ${TEST_SOURCE})
include_directories("${PROJECT_INCLUDE_DIR}")
target_link_libraries (whirlwind_test whirlwind)