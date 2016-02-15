#include <cmath>
#include <iostream>

#include "light.h"


using namespace std;

//#define SINGLE_SIDED

double DirectionalLight::distanceAttenuation( const Vec3d& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


Vec3d DirectionalLight::shadowAttenuation( const Vec3d& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	// HINT: You can access the Scene using the getScene function inherited by Light object.

    // init
    Vec3d d = getDirection(P);
    Scene* scene = getScene();
    Vec3d atten = Vec3d(1,1,1);
    isect i;
    ray r = ray(P, d, r.SHADOW);

    while(scene->intersect(r,i) && i.t > RAY_EPSILON) {

		Material m = i.getMaterial();
		atten = prod(atten, m.kt(i));

		if (clamp(atten).iszero()) {
			break;
		}

		r = ray(r.at(i.t), d, r.SHADOW);
	}

	return atten;

}

Vec3d DirectionalLight::getColor() const
{
	return color;
}

Vec3d DirectionalLight::getDirection( const Vec3d& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const Vec3d& P ) const
{
	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, we assume no attenuation and just return 1.0

	Vec3d distance = this->position - P;
	
	return min(1.0, 1/(constantTerm + linearTerm * distance.length() + quadraticTerm * pow(distance.length(), 2)));
}

Vec3d PointLight::getColor() const
{
	return color;
}

Vec3d PointLight::getDirection( const Vec3d& P ) const
{
	Vec3d ret = position - P;

	if(ret!=Vec3d(0,0,0))
		ret.normalize();
	return ret;
}


Vec3d PointLight::shadowAttenuation(const Vec3d& P) const
{
    // You should implement shadow-handling code here.
	// HINT: You can access the Scene using the getScene function inherited by Light object.

	Vec3d d = getDirection(P);
	Vec3d d_light = P - position;
	if(d_light!=Vec3d(0,0,0))
	d_light.normalize();

	Scene* scene = getScene();
	Vec3d atten;

	isect i, i_light;

	ray r = ray(P, d, ray::VISIBILITY);
	ray r_light = ray(position, d_light, ray::SHADOW);

	scene->intersect( r, i );
	scene->intersect(r_light, i_light);

	if(i.t < i_light.t){
		atten = Vec3d(0,0,0);
	}
	else
		atten = Vec3d(1,1,1);

    return atten; 

}