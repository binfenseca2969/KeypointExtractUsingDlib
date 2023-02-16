// KeypointExtractUsingDlibNew.cpp: 定义应用程序的入口点。
//

#include "KeypointExtractUsingDlibNew.h"

std::tuple<uint8_t, uint8_t, uint8_t> get_texcolor(rs2::video_frame texture, rs2::texture_coordinate texcoords)
{
	const int w = texture.get_width(), h = texture.get_height();
	int x = std::min(std::max(int(texcoords.u * w + .5f), 0), w - 1);
	int y = std::min(std::max(int(texcoords.v * h + .5f), 0), h - 1);
	int idx = x * texture.get_bytes_per_pixel() + y * texture.get_stride_in_bytes();
	const auto texture_data = reinterpret_cast<const uint8_t*>(texture.get_data());
	return std::tuple<uint8_t, uint8_t, uint8_t>(
		texture_data[idx], texture_data[idx + 1], texture_data[idx + 2]);
}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr points_to_pcl(const rs2::points& points, const rs2::video_frame& color)
{
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);

	auto sp = points.get_profile().as<rs2::video_stream_profile>();
	cloud->width = sp.width();
	cloud->height = sp.height();
	cloud->is_dense = false;
	cloud->points.resize(points.size());

	auto tex_coords = points.get_texture_coordinates();
	auto vertices = points.get_vertices();

	for (int i = 0; i < points.size(); ++i)
	{
		cloud->points[i].x = vertices[i].x;
		cloud->points[i].y = vertices[i].y;
		cloud->points[i].z = vertices[i].z;

		std::tuple<uint8_t, uint8_t, uint8_t> current_color;
		current_color = get_texcolor(color, tex_coords[i]);

		cloud->points[i].r = std::get<0>(current_color);
		cloud->points[i].g = std::get<1>(current_color);
		cloud->points[i].b = std::get<2>(current_color);
		if (i < 500)
			std::cout << int(cloud->points[i].r) << "\t" << int(cloud->points[i].g) << "\t" << int(cloud->points[i].b) << "\t" << std::endl;
	}

	return cloud;
}

int main(int argc, char* argv[])
{
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	rs2::pointcloud pc;
	rs2::points points;
	rs2::pipeline pipe;
	pipe.start();
	pcl::PLYWriter writer;
	for (int i = 1; i < 20; i++)
		pipe.wait_for_frames();
	auto frames = pipe.wait_for_frames();
	auto depth = frames.get_depth_frame();
	auto color = frames.get_color_frame();

	dlib::frontal_face_detector face_bbox_detector = dlib::get_frontal_face_detector();//创建dlib检测器用于检测人脸
	dlib::shape_predictor face_landmark_annotator;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> face_landmark_annotator;
	/*
		 shape_predictor_68_face_landmarks.dat下载网址：
		 http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
	*/
	rs_frame_image<dlib::rgb_pixel, RS2_FORMAT_RGB8>image(color);//Create a dlib image for face detection
	std::vector<dlib::rectangle> face_bboxex = face_bbox_detector(image);
	std::vector<dlib::full_object_detection> faces;
	for (auto const& bbox : face_bboxex)
		faces.push_back(face_landmark_annotator(image, bbox));

	pc.map_to(color);
	points = pc.calculate(depth);//这里顺序不能反，否则rgb信息不对

	auto pcl_points = points_to_pcl(points, color);

	for (auto const& face : faces)
	{
		for (int i = RIGHT_EAR; i <= N_POINTS; ++i)
		{
			auto pt = face.part(i);
			std::cout << "number: " << i << " x: " << pt.x() << " y: " << pt.y() << std::endl;
			pcl_points->points[pt.y() * depth.get_width() + pt.x()].r = 255;//将识别到的人脸关键点标红
			pcl_points->points[pt.y() * depth.get_width() + pt.x()].g = 0;
			pcl_points->points[pt.y() * depth.get_width() + pt.x()].b = 0;
			std::cout << "x: " << pcl_points->points[pt.y() * depth.get_width() + pt.x()].x <<
				" y: " << pcl_points->points[pt.y() * depth.get_width() + pt.x()].y << " z: " <<
				pcl_points->points[pt.y() * depth.get_width() + pt.x()].z << std::endl;
		}
	}

	writer.write("colorful.ply", *pcl_points);
	viewer->removeAllPointClouds();
	viewer->addPointCloud<pcl::PointXYZRGB>(pcl_points);

	while (!viewer->wasStopped()) {
		viewer->spinOnce(100);
		// boost::this_thread::sleep(boost::posix_time::microseconds(1));
	}
	return 0;
}