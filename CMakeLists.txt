 
cmake_minimum_required(VERSION 2.8)

project (whirlwind_root)
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
 
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
 
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

include(CTest)
enable_testing()

add_subdirectory(whirlwind)
add_subdirectory(whirlwind_test)
add_subdirectory(java_bundle)

add_test( RandomTest ${CMAKE_BINARY_DIR}/RandomTest )
add_test( CryptMemoryTest-Mem ${CMAKE_BINARY_DIR}/CryptMemoryTest Mem)
add_test( CryptMemoryTest-File ${CMAKE_BINARY_DIR}/CryptMemoryTest File)