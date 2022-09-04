#pragma once
//function
#include <opencv2/opencv.hpp>
#include <thread>
#include "geometry.h"
#include <mutex>
#include <ctime>
#include <conio.h>
#include "camera.h"
#include "material.h"

//hittable
#include "hittable_list.h"
#include "BVH.h"
#include "Sphere.h"
#include "aarect.h"
#include "move_Sphere.h"
#include "box.h"
#include "constant_medium.h"