 
cmake_minimum_required(VERSION 2.8) 

project (whirlwind_java_bunlde)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

set (CMAKE_JAVA_COMPILE_FLAGS "-source" "1.7" "-target" "1.7")
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (PROJECT_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

#only for test if java file is not corrupted (is compiling)
add_jar(CipherBundle ${PROJECT_SOURCE_DIR}/com/whirlwind/bundle/CipherBundle.java)

get_target_property(_jarFile ${PROJECT_NAME} JAR_FILE)
get_target_property(_classDir ${PROJECT_NAME} CLASSDIR)        

add_custom_command(
    OUTPUT CipherBundle.h
    COMMAND ${Java_JAVAH_EXECUTABLE} -jni 
    								 -classpath .:${PROJECT_SOURCE_DIR}
        							 -d ${PROJECT_HEADER_DIR}
        							 -jni com.whirlwind.bundle.CipherBundle
    )
set(CMAKE_C_FLAGS "-std=c99 -fPIC")

include_directories(/usr/include)
include_directories(${JNI_INCLUDE_DIRS})
include_directories(${JAVA_INCLUDE_PATH} ${PROJECT_SOURCE_DIR} ${PROJECT_HEADER_DIR})
include_directories(${CMAKE_SOURCE_DIR}/whirlwind/include) 

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE_DIR}/cipherBundle.c CipherBundle.h)

target_link_libraries(${PROJECT_NAME} ${JNI_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/build/libwhirlwind_static.a)