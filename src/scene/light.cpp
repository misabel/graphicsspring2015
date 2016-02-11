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
    return Vec3d(1,1,1);
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
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, we assume no attenuation and just return 1.0

	float distance = sqrt(P[0] * P[0] + P[1] * P[1] + P[2] * P[2]);


	return min(1.0, (1/(constantTerm + linearTerm * distance + quadraticTerm * pow(distance, 2))));
}

Vec3d PointLight::getColor() const
{
	return color;
}

Vec3d PointLight::getDirection( const Vec3d& P ) const
{
	Vec3d ret = position - P;
	ret.normalize();
	return ret;
}


Vec3d PointLight::shadowAttenuation(const Vec3d& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	// HINT: You can access the Scene using the getScene function inherited by Light object.
    return Vec3d(1,1,1);
}
