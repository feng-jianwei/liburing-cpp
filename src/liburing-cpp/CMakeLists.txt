
file(GLOB cpp_sources *.cpp *.c helper/*.cpp helper/*.c)

add_library(liburing-cpp ${cpp_sources})
find_package(liburing REQUIRED)
find_package(magic_enum REQUIRED)
target_include_directories(liburing-cpp 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include/liburing-cpp
    )

target_link_libraries(liburing-cpp 
    PRIVATE 
    liburing::liburing
    magic_enum::magic_enum
    )