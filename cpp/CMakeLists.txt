cmake_minimum_required(VERSION 3.15.3)
set(PROJECT_NAME balltracking)
project(${PROJECT_NAME})
find_package( OpenCV REQUIRED )

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        main.cpp
        )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
