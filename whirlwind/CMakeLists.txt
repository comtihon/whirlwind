 
cmake_minimum_required(VERSION 2.8) 

project (whirlwind)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
            
set(CMAKE_C_FLAGS "-std=c99") 

set (LIB_SOURCE  
	${PROJECT_SOURCE_DIR}/cipherConf.c
	${PROJECT_SOURCE_DIR}/cipherCore.c
	${PROJECT_SOURCE_DIR}/cipherAddon.c
	${PROJECT_SOURCE_DIR}/cipherSupport.c)
                  
set (BUNDLE_SOURCE
	${PROJECT_SOURCE_DIR}/cipherBundle.c)
              
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories (/usr/include)
 
add_library(${PROJECT_NAME} SHARED ${LIB_SOURCE})
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE})

include_directories("${JAVA_HOME}/include")
add_library(whirlwind_java_bundle SHARED ${BUNDLE_SOURCE})
target_link_libraries(whirlwind_java_bundle ${PROJECT_NAME})