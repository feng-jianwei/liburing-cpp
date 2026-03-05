# 获取当前目录下所有的 .cpp 文件
file(GLOB cpp_sources *.cpp *.c)

# 导入 Conan 生成的文件
find_package(GTest REQUIRED)          # 这会导入 GTest::gtest 等 target

add_executable(unitest_liburing_cpp ${cpp_sources})

target_include_directories(unitest_liburing_cpp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(unitest_liburing_cpp
    PRIVATE
        GTest::gtest_main
        GTest::gtest
        liburing-cpp
        liburing-service
)

# 推荐：自动发现所有 TEST() 宏（最方便）
include(GoogleTest)
gtest_discover_tests(unitest_liburing_cpp)
