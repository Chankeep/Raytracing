
#include "func.h"

using namespace std;
using namespace cv;
vec3 light_dir = normalize(vec3(-1, 1, 1));
int width = 1000;
int height = 1000;
int samples_perpixel = 10;
int max_depth = 50;
hittable_list world;
color background(0, 0, 0);


//互斥锁
mutex mut;
//线程数量
int nthread = 16;

vec3 ray_color(const ray& r, const color& background, BVH_node& BVH, int depth)
{
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	// If the ray hits nothing, return the background color.
	if (!BVH.hit(r, rec, Infinity, 0.00001))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.pos, rec.u, rec.v);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, BVH, depth - 1);
}

void checker_scene()
{
	//地面材质
	auto checker_mat = make_shared<checker_texture>(color(1.0, 1.0, 1.0), color(0, 0, 0));
	world.add(make_shared<Sphere>(point3(0, -100.5f, -1), 100, make_shared<lambertian>(checker_mat)));
	// auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	//地面实例
	// world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -10; a < 10; a++)
		for (int b = -10; b < 10; b++)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
	//
			if((center - point3(4,0.2,0)).length() > 0.9)
			{
				shared_ptr<material> sphere_material;
	// 			if (choose_mat < 0.8)
	// 			{
	// 				//diffuse
	// 				auto albedo = random() * random();
	// 				sphere_material = make_shared<lambertian>(albedo);
	// 				point3 center1(a + random_double(), 0.2, b + random_double() * 1.5);
	// 				auto center2 = center1 + vec3(0, random_double(0, 0.5), 0);
	// 				world.add(make_shared<Sphere>(center, 0.2, sphere_material));
	// 				world.add(make_shared<move_Sphere>(center1, center2, 0.0, 1.0, 0.2, sphere_material));
	// 			}
	//
				if (choose_mat > 0.8)
				{
					//metal
					auto albedo = random(0.5, 1);
					auto fuzz = random_double(0, 0.3);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
	// 			else
	// 			{
	// 				sphere_material = make_shared<dielectric>(1.5);
	// 				world.add(make_shared<Sphere>(center, 0.2, sphere_material));
	// 			}
			}
		}
	// auto material1 = make_shared<dielectric>(1.5);
	// world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));
	//
	// auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	// world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5, material3));
	
}

void random_scene() {
	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = random() * random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<Sphere>(center, 0.2, sphere_material));
				}
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

void two_perlin_spheres() {

	auto pertext = make_shared<noise_texture>(4);
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
}

void earth()
{
	auto earth_texture = make_shared<image_texture>("image/earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface);
	world.add(globe);
}

void simple_light() {

	auto pertext = make_shared<noise_texture>(4);
	world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	world.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
	
}

void cornell_box() {
	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	shared_ptr<hittable>box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	world.add(box1);

	shared_ptr<hittable>box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	world.add(box2);

	world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	world.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
}

void multithread(Mat& image, int thread_index, const camera& cam, BVH_node& BVH)
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
				pixel_color += ray_color(r, background, BVH, max_depth);
			}
			pixel_color /= samples_perpixel;
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
	int beg_sec = time((time_t*)nullptr);
	auto aspect_ratio = 16.0 / 9.0;
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
		windowWidth = 1080 - 200;
		windowHeight = static_cast<int>(static_cast<double>(windowWidth) * aspect_ratio);
	}

	point3 lookfrom;
	point3 lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;
	constexpr auto focal_length = 10.0;

	switch (0) {
	case 1:
		// random_scene();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		background = color(0.7, 0.8, 1.0);
		vfov = 20.0;
		aperture = 0.1;
		break;
	case 2:
		// checker_scene();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		background = color(0.7, 0.8, 1.0);
		vfov = 20.0;
		break;
	case 3:
		two_perlin_spheres();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		background = color(0.7, 0.8, 1.0);
		vfov = 20.0;
		break;
	case 4:
		earth();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		background = color(0.7, 0.8, 1.0);
		vfov = 20.0;
		break;
	case 5:
		simple_light();
		samples_perpixel = 400;
		background = color(0, 0, 0);
		lookfrom = point3(26, 3, 6);
		lookat = point3(0, 2, 0);
		vfov = 20.0;
		break;
	default:
	case 6:
		cornell_box();
		aspect_ratio = 1.0;
		samples_perpixel = 10000;
		background = color(0, 0, 0);
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
		break;
	}
	camera cam(lookfrom, lookat, vfov, aspect_ratio, aperture, focal_length, 0.0, 1.0);
	BVH_node BVH(world, 0, 1);


	cv::Mat image(height, width, CV_8UC3, Scalar(50, 50, 50));
	namedWindow("Rendering...", WINDOW_NORMAL);
	resizeWindow("Rendering...", windowWidth, windowHeight);

	
	// for (int j = height - 1; j >= 0; j--)
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
	// 			pixel_color += ray_color(r, BVH, max_depth);
	// 		}
	// 		pixel_color /= samples_perpixel;
	// 		//gamma矫正
	// 		pixel_color.e[0] = clamp(0.999, 0.0, sqrt(pixel_color.e[0]));
	// 		pixel_color.e[1] = clamp(0.999, 0.0, sqrt(pixel_color.e[1]));
	// 		pixel_color.e[2] = clamp(0.999, 0.0, sqrt(pixel_color.e[2]));
	// 		//[0,1]映射到[0,255]
	// 		pixel_color *= 255.999;
	//
	// 		image.at<cv::Vec3b>(height - 1 - j, i)[0] = static_cast<int>(pixel_color.e[2]);
	// 		image.at<cv::Vec3b>(height - j - 1, i)[1] = static_cast<int>(pixel_color.e[1]);
	// 		image.at<cv::Vec3b>(height - j - 1, i)[2] = static_cast<int>(pixel_color.e[0]);
	// 	}
	// 	if (!(j % (height / 100)))
	// 	{
	// 		imshow("Rendering...", image);
	// 		waitKey(1);
	// 	}
	// }


	// 创建多线程加速
	 thread ths[16];
	 for (int i = 0; i < nthread; i++)
	 {
	 	//实例化线程，给予线程任务
	 	ths[i] = thread(multithread, std::ref(image), i, std::ref(cam), std::ref(BVH));
	 }
	 for (auto& th : ths)
	 	//主线程main等待各个子进程
	 	th.join();


	std::cerr << "\nDone.\n";
	int end_sec = time((time_t*)nullptr);
	int run_time = end_sec - beg_sec;
	std::cerr << "渲染用时:" << run_time << "s" << std::endl;


	cv::imshow("Rendering...", image);
	cv::imwrite("image/Cornell Box.png", image);
	cv::waitKey(0);
	cv::destroyAllWindows();

	return 0;
}
