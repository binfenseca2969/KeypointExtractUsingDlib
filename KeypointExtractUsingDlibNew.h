// KeypointExtractUsingDlibNew.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
// TODO: 在此处引用程序需要的其他标头。
//librealsense2
#include <librealsense2/rs.hpp>
//pcl
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/ply_io.h>
//dlib
#include <dlib/image_processing/frontal_face_detector.h>
// #include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
//others
#include <vector>
#include "rs_frame_image.h"
#include "markup_68.h"