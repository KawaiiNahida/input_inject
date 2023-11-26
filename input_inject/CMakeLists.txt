cmake_minimum_required(VERSION 3.4.1)

project(input_inject)
set(CMAKE_CXX_STANDARD 20)

add_library(
        input_inject SHARED
        src/entry.cpp
        src/hooks.cpp
        ${CMAKE_SOURCE_DIR}/lib/src/hook64/And64InlineHook.cpp)

target_compile_options(input_inject PRIVATE -fno-rtti -fno-exceptions -fdeclspec)
target_include_directories(input_inject PRIVATE ${CMAKE_SOURCE_DIR}/lib/include ${ANDROID_INCLUDE_PATH} ${ANDROID_ARM64_INCLUDE_PATH})
target_link_directories(input_inject PRIVATE ${ANDROID_ARM64_LINK_PATH})

target_link_libraries(input_inject PRIVATE log)
