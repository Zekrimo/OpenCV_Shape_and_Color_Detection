cmake_minimum_required(VERSION 3.5)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( OpenCV_Shapes_and_Colors
	Main.cpp
	ShapeAndColorDetection.cpp
)

target_link_libraries( OpenCV_Shapes_and_Colors ${OpenCV_LIBS} )
