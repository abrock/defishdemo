cmake_minimum_required(VERSION 3.5)

project(defishdemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Werror -Wall ")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(defishdemo main.cpp)

target_link_libraries(defishdemo
    ${OpenCV_LIBRARIES}
)
