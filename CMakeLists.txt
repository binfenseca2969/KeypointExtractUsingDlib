# CMakeList.txt: KeypointExtractUsingDlibNew 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.12)

project ("KeypointExtractUsingDlib")

find_package(realsense2 REQUIRED)
if(NOT realsense2_FOUND)
	message(FATAL_ERROR "Error: Unable to find the realsense2 folder.")
endif()

find_package(dlib REQUIRED)
if(NOT dlib_FOUND)
	message(FATAL_ERROR "Error: Unable to find the dlib folder.")
endif()

find_package(PCL REQUIRED)
if(NOT PCL_FOUND)
	message(FATAL_ERROR "Error: Unable to find the PCL folder.")
endif()

# 将源代码添加到此项目的可执行文件。
add_executable (KeypointExtractUsingDlib "KeypointExtractUsingDlibNew.cpp" "KeypointExtractUsingDlibNew.h" "rs_frame_image.h" "markup_68.h")

target_link_libraries(KeypointExtractUsingDlib PRIVATE 
	dlib::dlib ${realsense2_LIBRARY} ${PCL_LIBRARIES})
target_include_directories(KeypointExtractUsingDlib PRIVATE 
	${dlib_INCLUDE_DIRS} ${realsense2_INCLUDE_DIR} ${PCL_INCLUDE_DIRS})
# TODO: 如有需要，请添加测试并安装目标。
