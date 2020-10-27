#include"light.h"
#include<sstream>
#include<string>
#include<cmath>
#include<cstdlib>
#define ran() ( double( rand() % 32768 ) / 32768 )

Light::Light() {
	sample = rand();
	next = NULL;
	lightPrimitive = NULL;
}

void Light::Input( std::string var , std::stringstream& fin ) {
	if ( var == "color=" ) color.Input( fin );
}

void PointLight::Input( std::string var , std::stringstream& fin ) {
	if ( var == "O=" ) O.Input( fin );
	Light::Input( var , fin );
}


double PointLight::CalnShade( Vector3 C , Primitive* primitive_head , int shade_quality ) {
	Vector3 V = O - C;
	double dist = V.Module();
	for ( Primitive* now = primitive_head ; now != NULL ; now = now->GetNext() )
	{
		CollidePrimitive tmp = now->Collide(C, V);
		if ( EPS < (dist - tmp.dist) )  return 0;
	}

	return 1;
}

void SquareLight::Input( std::string var , std::stringstream& fin ) {
	if ( var == "O=" ) O.Input( fin );
	if ( var == "Dx=" ) Dx.Input( fin );
	if ( var == "Dy=" ) Dy.Input( fin );
	Light::Input( var , fin );
}


double SquareLight::CalnShade( Vector3 C , Primitive* primitive_head , int shade_quality ) {
//	//TODO: NEED TO IMPLEMENT
//
	// 在矩形面上采样若干个点，判断点到交点之间是否存在物体
    Vector3 xDir = Dx.GetUnitVector(), yDir = Dy.GetUnitVector(); // 横纵坐标轴
    double length = Dx.Module(), width = Dy.Module();

    int shadeCount = 0; // 被遮挡的采样点个数
    int sampleCount = 100;
	for (int i = 0; i < sampleCount; i++) {
	    double x = length * ( 2.0 * ran() - 1.0), y = width * (2.0 * ran() - 1.0);
	    Vector3 lightPoint = O + x * xDir + y * yDir;
        Vector3 V = lightPoint - C;
        double dist = V.Module();
        for ( Primitive* now = primitive_head ; now != NULL ; now = now->GetNext() )
        {
            CollidePrimitive tmp = now->Collide(C, V);
            if ( EPS < (dist - tmp.dist) )  {
                shadeCount++;
                break;
            }
        }
	}
    double shade = 1.0 - (double) shadeCount / (double) sampleCount;
	return shade;
}

Primitive* SquareLight::CreateLightPrimitive()
{
	PlaneAreaLightPrimitive* res = new PlaneAreaLightPrimitive(O, Dx, Dy, color);
	lightPrimitive = res;
	return res;
}



void SphereLight::Input( std::string var , std::stringstream& fin ) {
	if ( var == "O=" ) O.Input( fin );
	if ( var == "R=" ) fin>>R;
	Light::Input( var , fin );
}


double SphereLight::CalnShade( Vector3 C , Primitive* primitive_head , int shade_quality ) {
	int shade = 0;
	//TODO: NEED TO IMPLEMENT
	return 0;
}


Primitive* SphereLight::CreateLightPrimitive()
{
	SphereLightPrimitive* res = new SphereLightPrimitive(O, R, color);
	lightPrimitive = res;
	return res;
}

