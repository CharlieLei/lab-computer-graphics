#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include"color.h"
#include"vector3.h"
#include"bmp.h"
#include<iostream>
#include<sstream>
#include<string>
#include<vector>

#include "Eigen/Dense"

extern const double EPS;
extern const double PI;
const double BIG_DIST = 1e100;

class Blur {
public:
	virtual std::pair<double, double> GetXY() = 0;
};

class ExpBlur : public Blur {
public:
	std::pair<double, double> GetXY();
};

class Material {
public:
	Color color , absor; // 材质颜色 光从内部折射到外部时会吸收的光
	double refl , refr;  // 反射系数 折射系数（每一次迭代，返回的颜色乘一次这些系数）
	double diff , spec;  // 漫反射系数 镜面反射系数（参考phong模型）
	double rindex;       // 折射率
	double drefl;        // 跟模糊有关？
	Bmp* texture;
	Blur* blur;

	Material();
	~Material() {}

	void Input( std::string , std::stringstream& );
};

struct CollidePrimitive;

class Primitive {
protected:
	int sample;
	Material* material;
	Primitive* next;

public:

	Primitive();
	Primitive( const Primitive& );
	~Primitive();

	int GetSample() { return sample; }
	Material* GetMaterial() { return material; }
	Primitive* GetNext() { return next; }
	void SetNext( Primitive* primitive ) { next = primitive; }

	virtual void Input( std::string , std::stringstream& );
	virtual CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V ) = 0;
	virtual Color GetTexture(Vector3 crash_C) = 0;
	virtual bool IsLightPrimitive(){return false;}
};

struct CollidePrimitive {
	bool isCollide;
	Primitive* collide_primitive;
	Vector3 N , C;
	double dist;
	bool front;
	CollidePrimitive(){isCollide = false; collide_primitive = NULL; dist = BIG_DIST;}
	Color GetTexture(){return collide_primitive->GetTexture(C);}
};

class Sphere : public Primitive {
protected:
	Vector3 O , De , Dc; // 球心坐标 用于将球面转成纹理坐标的两个坐标轴（纵轴和横轴）
	double R;            // 半径

public:
	Sphere();
	~Sphere() {}

	void Input( std::string , std::stringstream& );
	CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
	Color GetTexture(Vector3 crash_C);
};

class SphereLightPrimitive : public Sphere{
public:
	SphereLightPrimitive(Vector3 pO, double pR, Color color) : Sphere()
	{O = pO; R = pR; material->color = color; }
	bool IsLightPrimitive(){return true;}
};

class Plane : public Primitive {
protected:
	Vector3 N , Dx , Dy; // 法向量 用于将平面转成纹理坐标的两个坐标轴（平面的横轴和纵轴）
	double R;            // 原点沿法向量方向到面的距离，R*N 是面上的一个点

public:
	Plane() : Primitive() {}
	~Plane() {}

	void Input( std::string , std::stringstream& );
	CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
	Color GetTexture(Vector3 crash_C);
};

class Square : public Primitive {
protected:
	Vector3 O , Dx , Dy; // 中心点 横轴，其长度为矩形长度的一半 纵轴，其长度为矩形宽度的一半

public:
	Square() : Primitive() {}
	~Square() {}

	void Input( std::string , std::stringstream& );
	CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
	Color GetTexture(Vector3 crash_C);
};

class PlaneAreaLightPrimitive : public Square{
public:
	PlaneAreaLightPrimitive(Vector3 pO, Vector3 pDx, Vector3 pDy, Color color): Square()
	{O = pO; Dx = pDx; Dy = pDy; material->color = color; }
	bool IsLightPrimitive(){return true;}
};

class Cylinder : public Primitive {
	Vector3 O1, O2; // 底面圆心 顶面圆心
	double R; // 半径

public:
	Cylinder() : Primitive() {}
	Cylinder(Vector3 pO1, Vector3 pO2, double pR) : Primitive() {O1 = pO1; O2 = pO2; R = pR; }
	~Cylinder() {}

	void Input( std::string , std::stringstream& );
	CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
	Color GetTexture(Vector3 crash_C);
    /**
     * 判断射线和圆柱侧面的交点
     * @param ray_O 射线源点
     * @param ray_V 射线方向
     * @return
     */
    CollidePrimitive CollideAtSide( Vector3 ray_O , Vector3 ray_V );
    /**
     * 判断射线和圆柱上下底面的交点
     * @param ray_O 射线源点
     * @param ray_V 射线方向
     * @param O 底面圆心
     * @return
     */
    CollidePrimitive CollideAtBase( Vector3 ray_O , Vector3 ray_V, Vector3 O );
};

class Bezier : public Primitive {
	Vector3 O1, O2;
	Vector3 N, Nx, Ny;
	std::vector<double> R;
	std::vector<double> Z;
	int degree; // 控制点个数
	Cylinder* boundingCylinder;

public:
	Bezier() : Primitive() {boundingCylinder = NULL; degree = -1;}
	~Bezier() {}

	void Input( std::string , std::stringstream& );
	CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
	Color GetTexture(Vector3 crash_C);

private:
    CollidePrimitive CollideAtBezierCurve( Vector3 ray_O , Vector3 ray_V , double t, double u, double theta );
    /**
     * 计算贝塞尔曲线系数 Bernstein polynomial B_i,n(u) = C_n,i * u^i * (1-u)^(n-i)
     * @param u
     * @param n
     * @param i
     * @return
     */
    double B(double u, int n, int i);
    /**
     * 计算贝塞尔曲线上某一点的某个分量
     * @param u
     * @param ctrlPointsComponents 控制点在这个分量上的值
     * @return
     */
    double P(double u, std::vector<double> ctrlPointsComponents);
    /**
     * 计算贝塞尔曲线上某一点在某个分量上的关于u的微分
     * @param u
     * @param ctrlPointsComponents 控制点在这个分量上的值
     * @return
     */
    double dP(double u, std::vector<double> ctrlPointsComponents);
    /**
     * 光线 C = O + t * V
     * @param t 距离
     * @param ray_O 源点
     * @param ray_V 光线方向
     * @return
     */
    Vector3 C(double t, Vector3 ray_O , Vector3 ray_V);
    /**
     * 贝塞尔曲线上的某一点
     * @param u 该点在 包围圆柱高度方向 上的投影距离
     * @param theta 该点在 包围圆柱底面 上的投影的极坐标角度
     * @return
     */
    Vector3 S(double u, double theta);
    /**
     * 要求解的方程 F = S - C = 0
     * 相当于 光线上的点C 到 贝塞尔曲线上的点S 的 向量CS
     * @param t 光线距离
     * @param u 贝塞尔曲线点在 包围圆柱高度方向 上的投影距离
     * @param theta 贝塞尔曲线点在 包围圆柱底面 上的投影的极坐标角度
     * @param ray_O 光线源点
     * @param ray_V 光线方向
     * @return
     */
    Vector3 F(double t, double u, double theta, Vector3 ray_O , Vector3 ray_V);
    /**
     * 要方程 F 在 u 上的微分
     * @param t 光线距离
     * @param u 贝塞尔曲线点在 包围圆柱高度方向 上的投影距离
     * @param theta 贝塞尔曲线点在 包围圆柱底面 上的投影的极坐标角度
     * @param ray_V 光线方向
     * @return
     */
    Eigen::Matrix3d dF(double t, double u, double theta, Vector3 ray_V);
};

#endif
