cmake_minimum_required(VERSION 3.20)

project(liburing-cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("CMAKE_CXX_STANDARD : ${CMAKE_CXX_STANDARD}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_EXECUTABLE ccache)
if(CCACHE_EXECUTABLE)
    message(STATUS "Found ccache: ${CCACHE_EXECUTABLE}")
    set(CMAKE_C_COMPILER_LAUNCHER   "${CCACHE_EXECUTABLE}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
    # 按需加
    # set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
else()
    message(WARNING "ccache not found, proceeding without it")
endif()

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build using shared libraries" FORCE)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Wall -Wextra)
add_link_options(-Wl,--allow-shlib-undefined)
# ASan
option(ENABLE_ASAN "Build with AddressSanitizer" OFF)
if(ENABLE_ASAN)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        message(STATUS "Enabling AddressSanitizer")
        add_compile_options(
            -fsanitize=address
            -fno-omit-frame-pointer
            -g
            -O1
        )
        add_link_options(-fsanitize=address)
        # 自动设置常用 ASan 环境变量
        set(ENV{ASAN_OPTIONS} "detect_leaks=1:verbosity=1:print_stacktrace=1:abort_on_error=1")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "clang")
    add_compile_options(-Wno-unused-command-line-argument)

    add_link_options(-stdlib=libc++)
    # 禁止自动搜索 libstdc++ 的 include 路径（防止混淆）
    add_compile_options(-nostdinc++)
    include_directories(SYSTEM /usr/include/c++/v1)
endif()

add_subdirectory(src)
add_subdirectory(tests)