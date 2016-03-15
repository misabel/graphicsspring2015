#ifndef __FORCE_H__
#define __FORCE_H__

#include "vec.h"
#include <vector>

class Force {
public:
	Force();
	
	virtual Vec3f getForce() { 
		return Vec3f(0.0f, 0.0f, 0.0f); 
	}
};

class Gravity : public Force {
public:
	Gravity(float g);

	Vec3f getForce() { 
		return Vec3f(0.0f, -_Gravity, 0.0f); 
	}

	void setGravity(float g) { 
		_Gravity = g; 
	}

protected:
	float _Gravity;
};

class ViscousDrag : public Force {
public:
	ViscousDrag(float k, Vec3f dir);

	Vec3f getForce() {
		return -_Drag * _Velocity;
	}

	void setViscousDrag(float k, Vec3f dir) {
		_Drag = k;
		_Velocity = dir;
	}

protected:
	float _Drag;
	Vec3f _Velocity;
};

#endif __FORCE_H__