# Raytracing
这是为了改正自己半途而废的习惯而重启的项目
一个简单的软光线追踪器
## first week
## create by chankkep on 2022/8/23
* 更新了vec3 class

## update by chankkep on 2022/8/24
* 更新了ray class

## update by chankkep on 2022/8/26
1. 实现光追球体图像
2. 增加hittable抽象类用于派生球对象（隐式定义）还有hittable_list
3. 成功使用共享指针渲染图像

## update by chankkep on 2022/8/27
1. 增加random_double以及其他随机算法函数
2. 增加clamp函数用于限制数值范围
3. lazy hack渲染不准确diffuse
4. 设置光线反射迭代深度限制，防止stack boom
5. 使用gamma矫正获得准确的颜色强度
* 渲染了十分钟的图像效果还是可以的
### second update on 2022/8/28 0:37
1. 增加material抽象类以及lambert、dielectric、metal材质
* 渲染十五分钟，效果极佳