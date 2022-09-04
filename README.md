# Raytracing in the next week
这是为了改正自己半途而废的习惯而重启的项目
一个简单的软光线追踪器
## next week
## create by chankkep on 2022/8/30
1. 增加开始时间和结束时间在camera，material
2. 增加move_Sphere类
3. 实现motionBlur效果
* PS:camera会在time0-time1内随机生成射线，movingSphere在这个时间的center不一样，光线射中地方也不一样
## update by chankkep on 2022/8/31
1. 增加BVH_node，aabb类
2. 更新hittable以及它的派生类hittable_list，sphere，move_sphere，以让他们能够生成b-box
* PS:经历几个小时的调试，终于成功了，但是由于motionBlur的时候添加了两个球会有bug哈哈
* 这张图是加上BVH还有多线程在4885s渲染出来
* 没有多线程和BVH情况下80s，单BVH 18s，BVH+多线程 2s
## update by chankkep on 2022/9/2
1. 增加texture抽象类和纯色texture
2. 增加柏林噪声贴图
### second update
* 增加image texture
## update by chankkep on 2022/9/3 2:26
1. 增加rectangle，只是矩形而已
2. 渲染Cornell box
## update by chankkep on 2022/9/4 
1. 添加box实例，由六个rectangle组成
2. 添加translate、rotate_y
3. 渲染Cornell box完整版（将近18个小时，效果还是很不错的）