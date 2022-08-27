#include <opencv2/opencv.hpp>
// #include <iostream>
#include "camera.h"
#include "geometry.h"
#include "hittable_list.h"
#include "material.h"
#include "Sphere.h"

using namespace cv;
vec3 light_dir = normalize(vec3(-1, 1, 1));
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int width = 1600;
constexpr int height = static_cast<int>(width / aspect_ratio);
constexpr int samples_perpixel = 100;
constexpr int max_depth = 50;

vec3 ray_color(const ray& r, hittable_list world, int depth)
{
	hit_record rec;
	if (depth <= 0)return vec3(0, 0, 0);
	if(world.hit(r, rec, Infinity, 0.000001))
	{
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return vec3(0, 0, 0);
	}
	const vec3 unit_dir = normalize(r.get_dir());
	double t = 0.5 * (unit_dir.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	//camera
	camera cam;
	constexpr auto aspect_ratio = 16.0 / 9.0;
	//image
	int windowHeight;
	int windowWidth;
	if(width>height)
	{
		windowHeight = 1080 - 200;
		windowWidth = static_cast<int>(static_cast<double>(windowHeight) * aspect_ratio);
	}
	else
	{
		windowWidth = 1920 - 200;
		windowHeight = static_cast<int>(static_cast<double>(windowWidth) * aspect_ratio);
	}
	//world
	hittable_list world;
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	Mat image(height, width, CV_8UC3, Scalar(50, 50, 50));
	namedWindow("Rendering...", WINDOW_NORMAL);
	resizeWindow("Rendering...", windowWidth, windowHeight);

	
	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			color pixel_color(0, 0, 0);
			for (int x = 0; x < samples_perpixel; ++x)
			{
				float U = static_cast<float>(i + random_double()) / static_cast<float>(width);
				float V = static_cast<float>(j + random_double()) / static_cast<float>(height);

				ray r = cam.get_ray(U, V);
				pixel_color += ray_color(r, world, max_depth);
			}
			pixel_color /= samples_perpixel;

			//gamma½ÃÕý
			pixel_color.e[0] = clamp(0.999,0.0,sqrt(pixel_color.e[0]));
			pixel_color.e[1] = clamp(0.999, 0.0, sqrt(pixel_color.e[1]));
			pixel_color.e[2] = clamp(0.999, 0.0, sqrt(pixel_color.e[2]));
			//[0,1]Ó³Éäµ½[0,255]
			pixel_color *= 255.999;

			image.at<cv::Vec3b>(height - 1 - j, i)[0] = static_cast<int>(pixel_color.e[2]);
			image.at<cv::Vec3b>(height - j - 1, i)[1] = static_cast<int>(pixel_color.e[1]);
			image.at<cv::Vec3b>(height - j - 1, i)[2] = static_cast<int>(pixel_color.e[0]);

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