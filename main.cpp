#include <opencv2/opencv.hpp>
#include <iostream>
#include "geometry.h"
using namespace cv;
int main()
{
	//image

	constexpr int width = 200;
	constexpr int height = 100;

	Mat image(height, width, CV_8UC3, Scalar(50, 50, 50));
	namedWindow("Rendering...", WINDOW_NORMAL);
	resizeWindow("Rendering...", 1440, 900);
	for (int j = height - 1; j >= 0; j--)
		for (int i = 0; i < width; i++)
		{
			vec3 color = { static_cast<double>(j) / (height - 1) , 0.1, static_cast<double>(i) / (width - 1) };
			color = color * 255.999;

			image.at<cv::Vec3b>(height - 1 - j, i)[0] = static_cast<int>(color.e[0]);
			image.at<cv::Vec3b>(height - j - 1, i)[1] = static_cast<int>(color.e[1]);
			image.at<cv::Vec3b>(height - j - 1, i)[2] = static_cast<int>(color.e[2]);

			if (!(j % (height / 100)))
			{
				imshow("Rendering...", image);
				waitKey(1);
			}
		}
	imshow("Rendering...", image);
	waitKey(0);
	destroyAllWindows();
	return 0;
}