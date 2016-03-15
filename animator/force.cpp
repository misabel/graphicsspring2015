#include "force.h"


Force::Force() {}

Gravity::Gravity(float G) : _Gravity(G) {}

ViscousDrag::ViscousDrag(float kDrag, Vec3f Dir) : _Drag(kDrag), _Velocity(Dir) {}
