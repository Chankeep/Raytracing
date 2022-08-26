#include <opencv2/opencv.hpp>
// #include <iostream>
#include "geometry.h"
#include "hittable_list.h"
#include "ray.h"
#include "Sphere.h"

using namespace cv;
vec3 light_dir = normalize(vec3(1, 1, 1));
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int width = 600;
constexpr int height = static_cast<int>(width / aspect_ratio);

vec3 ray_color(const ray& r, hittable_list world)
{
	hit_record rec;
	if(world.hit(r, rec, Infinity, 0))
	{
		color N = (rec.normal + 1) * 0.5;
		// std::cout << N.length()<< std::endl;
		// N = normalize(N);

		// std::cout << N.length() << std::endl;
		double NdotL = std::max(0.0,dot(N, light_dir));
		color c = vec3(NdotL,NdotL,NdotL);
		return N;
	}
	const vec3 unit_dir = normalize(r.get_dir());
	double t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	// image

	vec3 LowerLeftCorner(-2.0, -1.0, -1.0);
	vec3 Horizontal(4.0, 0.0, 0.0);
	vec3 Vertical(0.0, 2.0, 0.0);
	vec3 Origin(0.0, 0.0, 0.0);

	//world
	hittable_list world;
	world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

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
			vec3 color = ray_color(r, world);

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
	imwrite("image/normal.png", image);
	imshow("Rendering...", image);
	waitKey(0);
	destroyAllWindows();
	return 0;
}