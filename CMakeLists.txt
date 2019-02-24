cmake_minimum_required(VERSION 3.13.0)
project(CurlPP)

add_subdirectory(deps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(foo main.cpp)
target_include_directories(foo PRIVATE include)
target_link_libraries(foo libcurl nlohmann_json)