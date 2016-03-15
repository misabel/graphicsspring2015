#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerui.h"
#include "force.h"
#include "particle.h"


#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
//#include <limits>

using namespace std;

static float prevT;

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() : restitution("Restitution", 0.0f, 2.0f, 1.0f, 0.1f)
{
	//Gravity G = new Gravity();

	bake_fps = 0;
	bake_start_time = 0;
	bake_end_time = 0;

	// Leave these here; the UI needs them to work correctly.
	dirty = false;
	simulate = false;
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{

}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	bake_start_time = 0;

	// These values are used by the UI ...
	// negative bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	bake_start_time = 0;

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	if (simulate){

		const float deltaT = 0.0025;

		for (auto &particle : _Particles) {
			Vec3f force = Vec3f(0.0f, 0.0f, 0.0f);
			Vec3f velocity = particle->getVelocity();
			Vec3f position = particle->getPosition();

			for(auto &f : _Forces){
				force += f->getForce()*particle->getMass();
			}

			velocity += force / particle->getMass() * deltaT;
			position += velocity * deltaT;
			particle->setVelocity(velocity);
			particle->setPosition(position);
		}
	
	}
	

	// Debugging info
	/*if( t - prevT > .08 )
		printf("(!!) Dropped Frame %lf (!!)\n", t-prevT);*/
	//prevT = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// YOUR CODE HERE
	if (simulate) {
		for (vector<Particle*>::iterator it = particles[t].begin(); it != particles[t].end(); it++) {
			(*it)->Draw();
		}
	}
}




/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO (baking is extra credit)

}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO (baking is extra credit)

}



