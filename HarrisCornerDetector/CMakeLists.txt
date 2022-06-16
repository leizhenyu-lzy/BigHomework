# cmake_minimum_required(VERSION 3.0)
# project(OpenCV4)

# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

# # set(SRC_LIST testOpenCV.cpp)
# set(CMAKE_CXX_STANDARD 11)

# # find_package( OpenCV REQUIRED )
# # include_directories(${OpenCV_INCLUDE_DIRS})

# message(STATUS "BINARY DIR" ${PROJECT_BINARY_DIR})
# message(STATUS "SOURCE DIR" ${PROJECT_SOURCE_DIR})

# add_executable(test testOpenCV.cpp)
# target_link_libraries(test opencv_core opencv_imgcodecs opencv_highgui)
project(opencv-test CXX)
find_package(OpenCV CONFIG REQUIRED)

# OpenCV不导出目标，只导出变量，可以用interface library简单地包装一下
add_library(OpenCV INTERFACE)
target_include_directories(OpenCV INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCV INTERFACE ${OpenCV_LIBS})

add_executable(zhihuHarris zhihu.cpp)
target_link_libraries(zhihuHarris OpenCV)