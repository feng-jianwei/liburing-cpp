# 获取当前目录下所有的 .cpp 文件
file(GLOB cpp_sources *.cpp *.c)

find_package(benchmark REQUIRED)          # 这会导入 GTest::gtest 等 target

add_executable(benchmark_liburing_cpp ${cpp_sources})

target_include_directories(benchmark_liburing_cpp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(benchmark_liburing_cpp
    PRIVATE
        benchmark::benchmark
        liburing-cpp
        liburing-service
)
