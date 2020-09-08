cmake_minimum_required(VERSION 3.5)

project(webqa LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS system date_time REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(3rdparty/drogon)

add_executable(webqa main.cpp)

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_CURR}
        drogon
        )
