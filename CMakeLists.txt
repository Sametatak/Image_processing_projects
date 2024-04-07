cmake_minimum_required(VERSION 3.5.0)
project(opencvtest VERSION 0.1.0)

include(CTest)
enable_testing()

set(OpenCV_DIR "C:/Users/samet/Desktop/opencv/build")
find_package(OpenCV REQUIRED)

# Use the variable set by find_package to include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main ./task1/main.cpp)
add_executable(main2 ./task2/task2.cpp)


target_link_libraries(main ${OpenCV_LIBS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
