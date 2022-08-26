#include <opencv2/opencv.hpp>
// #include <iostream>
#include "geometry.h"
#include "ray.h"

using namespace cv;
vec3 light_dir = normalize(vec3(1, 1, 1));
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int width = 600;
constexpr int height = static_cast<int>(width / aspect_ratio);

double hit_sphere(const ray& r, const vec3& center, double radius)
{
	vec3 a_minus_c = r.get_origin() - center;
	const double a = dot(r.get_dir(), r.get_dir());
	const double b = 2.0 * dot(r.get_dir(), a_minus_c);
	const double c = dot(a_minus_c, a_minus_c) - radius * radius;
	const double discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1.0;
	else
	{
		double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t1 > 0) 
			return t1;
		double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t2 > 0) 
			return t2;
		return -1.0;
	}
}

vec3 get_color(const ray& r)
{
	double t = hit_sphere(r, vec3(0, 0, -1), 0.6);
	if(t > 0)
	{
		vec3 N = r.at(t) - vec3(0, 0, -1);
		N = normalize(N);
		double color = dot(N, light_dir);
		if (color < 0) color = 0;
		return vec3(color,color,color);
	}
	const vec3 unit_dir = normalize(r.get_dir());
	t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	// image

	vec3 LowerLeftCorner(-2.0, -1.0, -1.0);
	vec3 Horizontal(4.0, 0.0, 0.0);
	vec3 Vertical(0.0, 2.0, 0.0);
	vec3 Origin(0.0, 0.0, 0.0);

	Mat image(height, width, CV_8UC3, Scalar(50, 50, 50));
	namedWindow("Rendering...", WINDOW_NORMAL);
	resizeWindow("Rendering...", 1440, 750);
	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			float U = static_cast<float>(i) / static_cast<float>(width);
			float V = static_cast<float>(j) / static_cast<float>(height);

			ray r(Origin, LowerLeftCorner + U * Horizontal + V * Vertical);
			vec3 color = get_color(r);

			color *= 255.999;

			image.at<cv::Vec3b>(height - 1 - j, i)[0] = static_cast<int>(color.e[2]);
			image.at<cv::Vec3b>(height - j - 1, i)[1] = static_cast<int>(color.e[1]);
			image.at<cv::Vec3b>(height - j - 1, i)[2] = static_cast<int>(color.e[0]);


		}
		if (!(j % (height / 100)))
		{
			imshow("Rendering...", image);
			waitKey(1);
		}
	}
	imwrite("image/diffuse.png", image);
	imshow("Rendering...", image);
	waitKey(0);
	destroyAllWindows();
	return 0;
}