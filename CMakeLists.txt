cmake_minimum_required(VERSION 3.25.0-rc4)

set(OpenCV_DIR C:/opencv/minGW)

project(CameraApp)
file(GLOB project_SRCS src/*.cpp)
add_executable(CameraApp ${project_SRCS})

find_package(wxWidgets REQUIRED COMPONENTS core base)
target_link_libraries(CameraApp ${wxWidgets_LIBRARIES})
target_include_directories(CameraApp PRIVATE ${wxWidgets_INCLUDE_DIRS})

find_package(OpenCV 4.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

target_link_libraries(CameraApp ${OpenCV_LIBS})
target_include_directories(CameraApp PRIVATE ${OpenCV_INCLUDE_DIRS})
