#include "cloth.h"
#include <FL/gl.h>
#include <math.h>

class Constraint {
// public:
// 	Particle particleA, particleB;
// 	float restLength; // init distance btw two particles

// 	Constraint(Particle p1, Particle p2) {
// 		Vec3f distance = p1.getPosition() - p2.getPosition();
// 		restLength = distance.length();
// 	}

// 	// Assume that an array of constraints, m_constraints, exists
// 	// ????????
// void satisfyConstraints() {
// for(int j=0; j<NUM_ITERATIONS; j++) {
//       for(int i=0; i<NUM_CONSTRAINTS; i++) {
//             Constraint& c = m_constraints[i];
//                   Vector3& x1 = m_x[c.particleA];
//             Vector3& x2 = m_x[c.particleB];
// Vector3 delta = x2-x1;
// float deltalength = sqrt(delta*delta);
// float diff=(deltalength-c.restlength)/deltalength;
// x1 += delta*0.5*diff;
// x2 -= delta*0.5*diff;
//       }
 
//       // Constrain one particle of the cloth to origo
//       m_x[0] = Vector3(0,0,0);
//       }
// }
	

};
