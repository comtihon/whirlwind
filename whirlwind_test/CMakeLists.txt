cmake_minimum_required(VERSION 2.6)
project (whirlwind_test)
 
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_C_FLAGS "-std=c99") 
 
add_executable(RandomTest ${PROJECT_SOURCE_DIR}/randomTest.c)
 
include_directories("${PROJECT_BINARY_DIR}")
add_executable(CryptMemoryTest ${PROJECT_SOURCE_DIR}/cryptMemoryTest.c)
include_directories("${PROJECT_INCLUDE_DIR}")
target_link_libraries (CryptMemoryTest whirlwind)