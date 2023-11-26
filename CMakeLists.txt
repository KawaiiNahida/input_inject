cmake_minimum_required(VERSION 3.4.1)

#[[
Add following Options in cmake setting
-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
-DANDROID_NDK_HOME=C:/Users/<username>/AppData/Local/Android/Sdk/ndk/25.0.8775105
-DANDROID_ABI=armeabi-v7a/arm64-v8a
]]

include(Options.cmake)

set(VCPKG_CMAKE_SYSTEM_NAME Android)
if (ANDROID_ABI STREQUAL "arm64-v8a")
    set(VCPKG_TARGET_TRIPLET arm64-android)
else ()
    set(VCPKG_TARGET_TRIPLET arm-android)
endif ()
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake)

set(ANDROID_INCLUDE_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include)
set(ANDROID_ARM64_INCLUDE_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/aarch64-linux-android)
set(ANDROID_ARM64_LINK_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/aarch64-linux-android)
set(ANDROID_ARM_INCLUDE_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/arm-linux-androideabi)
set(ANDROID_ARM_LINK_PATH ${ANDROID_NDK_HOME}/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/arm-linux-androideabi)

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)

set(ANDROID_PLATFORM 31)

project(parent)

message(STATUS "Current ANDROID_ABI=${ANDROID_ABI}")
if (ANDROID_ABI STREQUAL "arm64-v8a")
    add_compile_options(-fno-exceptions -fno-rtti)
    add_subdirectory(input_inject)
endif ()