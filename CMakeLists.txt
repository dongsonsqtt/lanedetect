cmake_minimum_required(VERSION 2.8)
project( C++ )
find_package( OpenCV REQUIRED )
add_executable( run video.cpp )
target_link_libraries( run ${OpenCV_LIBS} )
