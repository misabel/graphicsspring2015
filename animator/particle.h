#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "vec.h"
#include <vector>
#include "force.h"

using namespace std;

class Particle {
public:
	Particle();
	Particle(float Mass, const Vec3f &Position, const Vec3f &Velocity);

	float getMass() { return _Mass; }
	Vec3f getPosition() { return _Position; }
	Vec3f getVelocity() { return _Velocity; }
	void setMass(float Mass) { _Mass = Mass; }
	void setPosition(Vec3f Position) { _Position = Position; }
	void setVelocity(Vec3f Velocity) { _Velocity = Velocity; }
	
	virtual void Draw() = 0;
	void addForce(Force* f) { _Forces.push_back(f); }

protected:
	float _Mass;
	Vec3f _Position;
	Vec3f _Velocity;
	vector<Force *> _Forces;
};

#endif __PARTICLE_H__