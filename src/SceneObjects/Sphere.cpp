#include <cmath>
#include <iostream>

#include "Sphere.h"

using namespace std;


bool Sphere::intersectLocal( const ray& r, isect& i ) const
{
    i.obj = this;

	Vec3d p = r.getPosition();
	Vec3d d = r.getDirection();

	double px = p[0];
	double py = p[1];
	double pz = p[2];

	float radius = 1.0;
	Vec3d c = Vec3d (0.0, 0.0, 0.0);
	float cx = 0.0;
	float cy = 0.0;
	float cz = 0.0;

	double dx = d[0];
	double dy = d[1];
	double dz = d[2];

	double discriminant = pow( d * ( p - c),2) - (d * d) * ((p - c) * (p - c) - pow(radius, 2));

	if( discriminant >= 0 ) {

		Vec3d N = 2*(p-c);
		N.normalize();
		i.setN(N);

		discriminant = sqrt(discriminant);

		double t1 = (-d * (p - c) + discriminant) / (d * d);
		double t2 = (-d * (p - c) - discriminant) / (d * d);

		Vec3d p1 = r.at(t1);
		Vec3d p2 = r.at(t2);

		double distance1 = sqrt( pow( px - p1[0] ,2) + pow(py - p1[1], 2) + pow(pz - p1[2], 2));
		double distance2 = sqrt( pow( px - p2[0] ,2) + pow(py - p2[1], 2) + pow(pz - p2[2], 2));

		if(distance1 < distance2){

			i.setT(t1);
		}
		else
			i.setT(t2);
		return true;


	}

    return false;
}

