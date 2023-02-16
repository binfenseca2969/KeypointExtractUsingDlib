# KeypointExtractUsingDlib
Extract the key point from the face point cloud.

使用Intel Realsense相机提取出包含人脸的点云中人脸的关键点，使用了dlib库对点云中的人脸进行识别。

识别点数为68点，也可使用5点，但需更改部分代码。
## 参考代码
librealsense的dlib示例：https://github.com/IntelRealSense/librealsense/tree/master/wrappers/dlib

利用IntelRealSense D435i 提取一帧pcl：：PointXYZRGB图像(C++)：https://blog.csdn.net/m0_56838271/article/details/121157332
## 代码环境
Windows 10 22H2，PCL-1.12.0，librealsense-2.53.1，dlib-19.24，使用Visual Studio 2019，RelWithDebInfo，CMake-3.20，使用的Realsense相机为D405。 
## 附件下载
shape_predictor_68_face_landmarks.dat下载网址：http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2
## 结果
平板找的Lena图，红色为识别出的关键点，真人也可以（但是不想放自己的照片……）。

![屏幕截图 2023-02-16 211328](https://user-images.githubusercontent.com/94226982/219384604-cff042ec-6d08-41a7-adc9-9e5dff2ef041.png)
