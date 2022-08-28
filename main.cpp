#include <opencv2/opencv.hpp>
#include <thread>
#include <mutex>
// #include <iostream>
#include "camera.h"
#include "geometry.h"
#include "hittable_list.h"
#include "material.h"
#include "Sphere.h"

using namespace std;
using namespace cv;
vec3 light_dir = normalize(vec3(-1, 1, 1));
constexpr double aspect_ratio = 16.0 / 9.0;
constexpr int width = 1280;
constexpr int height = static_cast<int>(width / aspect_ratio);
constexpr int samples_perpixel = 200;
constexpr int max_depth = 50;
hittable_list world;
//互斥锁
mutex mut;
//线程数量
int nthread = 18;


vec3 ray_color(const ray& r, hittable_list world, int depth)
{
	hit_record rec;
	if (depth <= 0)return vec3(0, 0, 0);
	if(world.hit(r, rec, Infinity, 0.00000001))
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

void random_scene()
{
	//地面材质
	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	//地面实例
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++)
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if((center - point3(4,0.2,0)).length() > 0.9)
			{
				shared_ptr<material> sphere_material;
				if (choose_mat < 0.8)
				{
					//diffuse
					auto albedo = random() * random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}

				else if (choose_mat < 0.95)
				{
					//metal
					auto albedo = random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else
				{
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));
	
}

void multithread(Mat& image, int thread_index, const camera& cam)
{
	const int perThread_height = height / nthread;
	for (int j = thread_index * perThread_height; j < perThread_height * (thread_index + 1); j++)
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
			lock_guard<mutex>locker(mut);
			//gamma矫正
			pixel_color.e[0] = clamp(0.999, 0.0, sqrt(pixel_color.e[0]));
			pixel_color.e[1] = clamp(0.999, 0.0, sqrt(pixel_color.e[1]));
			pixel_color.e[2] = clamp(0.999, 0.0, sqrt(pixel_color.e[2]));
			//[0,1]映射到[0,255]
			pixel_color *= 255.999;

			image.at<cv::Vec3b>(height - 1 - j, i)[0] = static_cast<int>(pixel_color.e[2]);
			image.at<cv::Vec3b>(height - j - 1, i)[1] = static_cast<int>(pixel_color.e[1]);
			image.at<cv::Vec3b>(height - j - 1, i)[2] = static_cast<int>(pixel_color.e[0]);
		}
	}
}

int main()
{
	
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
	//camera
	point3 lookfrom(12, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto focal_length = 10.0;
	auto aperture = 0.1;
	camera cam(lookfrom, lookat, 20, aspect_ratio, aperture, focal_length);
	
	//world
	random_scene();
	//这里R等于cos45°是因为相机是看不到球的正上方的。渲染出来的图像也不是球的最上方
	// auto R = sin(PI / 4);
	// auto material_left = make_shared<lambertian>(color(0, 0, 1));
	// auto material_right = make_shared<lambertian>(color(1, 0, 0));
	//
	// world.add(make_shared<Sphere>(point3(-R, 0, -1), R, material_left));
	// world.add(make_shared<Sphere>(point3(R, 0, -1), R, material_right));


	cv::Mat image(height, width, CV_8UC3, Scalar(50, 50, 50));
	namedWindow("Rendering...", WINDOW_NORMAL);
	resizeWindow("Rendering...", windowWidth, windowHeight);

	
	// for (int j = 0; j < height; j++)
	// {
	// 	for (int i = 0; i < width; i++)
	// 	{
	// 		color pixel_color(0, 0, 0);
	// 		for (int x = 0; x < samples_perpixel; ++x)
	// 		{
	// 			float U = static_cast<float>(i + random_double()) / static_cast<float>(width);
	// 			float V = static_cast<float>(j + random_double()) / static_cast<float>(height);
	//
	// 			ray r = cam.get_ray(U, V);
	// 			pixel_color += ray_color(r, world, max_depth);
	// 		}
	// 		pixel_color /= samples_perpixel;
	// 		write_color(image, pixel_color, i, j);
	// 	}
	// 	if (!(j % (height / 100)))
	// 	{
	// 		imshow("Rendering...", image);
	// 		waitKey(1);
	// 	}
	// }

	//创建多线程加速
	thread ths[18];
	for (int i = 0; i < nthread; i++)
	{
		//实例化线程，给予线程任务
		ths[i] = thread(multithread, std::ref(image), i, std::ref(cam));
	}
	for (auto& th : ths)
		//主线程main等待各个子进程
		th.join();

	cv::imwrite("image/final render.png", image);
	cv::imshow("Rendering...", image);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
