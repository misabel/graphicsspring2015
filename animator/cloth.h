#ifndef __CLOTH_H__
#define __CLOTH_H__

#include "vec.h"
#include <vector>
#include "force.h"
#include "clothParticle.h"

class Cloth {
private: 
		int x_num; // number of particle in x-asix
		int y_num;
		//ClothParticle cpList[];
		std::vector<ClothParticle> cpList;

public:
	/** Constructor **/
	Cloth(Vec3f origin, float width, float height, int x, int y);

	/** Destructor **/
	virtual ~Cloth();

	/** Simulation fxns **/
	// This fxn should render all particles in the system,
	// at current time t.
	virtual void drawCloth();

	// This function should compute forces acting on all particles
	// and update their state (pos and vel) appropriately.
	// virtual void computeForcesAndUpdateParticles(float t);

	// // This function should reset the system to its initial state.
	// // When you need to reset your simulation, PLEASE USE THIS FXN.
	// // It sets some state variables that the UI requires to properly
	// // update the display.  Ditto for the following two functions.
	// virtual void resetSimulation(float t);

	// // This function should start the simulation
	// virtual void startSimulation(float t);

	// // This function should stop the simulation
	// virtual void stopSimulation(float t);

	Vec3f findNormal(Vec3f p1, Vec3f p2, Vec3f p3);


	// These accessor fxns are implemented for you
	bool isSimulate() { return simulate; }
	bool isDirty() { return dirty; }
	void setDirty(bool d) { dirty = d; }


protected:

	/** General state variables **/
	bool simulate;						// flag for simulation mode
	bool dirty;							// flag for updating ui (don't worry about this)
};


#endif __CLOTH_H__