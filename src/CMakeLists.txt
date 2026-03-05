add_subdirectory(liburing-cpp)
file(GLOB cpp_sources *.cpp *.c helper/*.cpp helper/*.c)

add_library(liburing-service ${cpp_sources})
target_include_directories(liburing-service 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(liburing-service 
    PRIVATE
    liburing-cpp
    )