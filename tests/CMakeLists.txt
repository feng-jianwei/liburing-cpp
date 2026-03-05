enable_testing()

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

add_subdirectory(benchmark_uring)
add_subdirectory(unitest)