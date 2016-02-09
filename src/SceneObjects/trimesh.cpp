#include <cmath>
#include <float.h>
#include "trimesh.h"

using namespace std;

Trimesh::~Trimesh()
{
	for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
		delete *i;
}

bool Trimesh::hasPerVertexNormals()
{
	return !(this->normals.empty());
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

void Trimesh::addTextureUV( const Vec2d &n )
{
    textureuvs.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt )
        return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    faces.push_back( newFace );
    scene->add(newFace);
    return true;
}

char *
Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

// Calculates and returns the normal of the triangle too.
bool TrimeshFace::intersectLocal( const ray& r, isect& i ) const
{
// init

    const Vec3d& a = parent->vertices[ids[0]];
    const Vec3d& b = parent->vertices[ids[1]];
    const Vec3d& c = parent->vertices[ids[2]]; 

    Vec3d p = r.getPosition();
    Vec3d d = r.getDirection();

    Vec3d n = (b - a)^(c - a);
    n.normalize();

    // check if parallel
    if((n * d) < RAY_EPSILON) {
        return false;
    }

    double t = (n * a - n * p) / (n * d);
    if(t < RAY_EPSILON) {
        return false;
    }

    // intersect point
    Vec3d q = r.at(t);

    // inside-outside testing
    if((((b - a)^(q - a)) * n) < 0) {
        return false;
    }

    if((((c - b)^(q - b)) * n) < 0) {
        return false;
    }

    if((((a - c)^(q - c)) * n) < 0) {
        return false;
    }
    
	// TrimeshFace::parent->hasPerVertexNormals tells you if the triangle has per-vertex normals.
	// If it does, you should compute and return the interpolated normal at the intersection point.
	// If it does not, you should return the normal of the triangle's supporting plane.
	if(TrimeshFace::parent->hasPerVertexNormals()) {

        double alpha = (((c - b)^(q - b)) * n) / (((b - a)^(c - a)) * n);
        double beta = (((a - c)^(q - c)) * n) / (((b - a)^(c - a)) * n);
        double gamma = (((b - a)^(q - a)) * n) / (((b - a)^(c - a)) * n);

        Vec3d tmp = alpha * (parent->normals[ids[0]]) + beta * (parent->normals[ids[1]]) + gamma * (parent->normals[ids[2]]);
        tmp.normalize();
        i.setN(tmp);

    }
    else {

        i.setN(n);

    }

    i.obj = this;
    return true; 
    
    /*
    // YOUR CODE HERE:
    // Add triangle intersection code here.
    // it currently ignores all triangles and just return false.
    //
    // Note that you are only intersecting a single triangle, and the vertices
    // of the triangle are supplied to you by the trimesh class.
    //
    // You should retrieve the vertices using code like this:
    //
    // const Vec3d& a = parent->vertices[ids[0]];
    // const Vec3d& b = parent->vertices[ids[1]];
    // const Vec3d& c = parent->vertices[ids[2]];
	//
	// TrimeshFace::parent->hasPerVertexNormals tells you if the triangle has per-vertex normals.
	// If it does, you should compute and return the interpolated normal at the intersection point.
	// If it does not, you should return the normal of the triangle's supporting plane.
	// 

    i.obj = this;

    Vec3d p = r.getPosition();
    Vec3d d = r.getDirection();
    const Vec3d& a = parent->vertices[ids[0]];
    const Vec3d& b = parent->vertices[ids[1]];
    const Vec3d& c = parent->vertices[ids[2]];

    Vec3d n = (b - a) ^ (c - a);
    double magnitude = sqrt( n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

    n = n / magnitude;

    if(n * d == 0)
        return false;

    Vec3d dp = n * a;

    double t = (dp - n * p) / (n * d);

    Vec3d q = r.at(t);

    if( (((b - a) ^ (q - a)) * n) >= 0 &&
        (((c - b) ^ (q - b)) * n) >= 0 &&
        (((a - c) ^ (q - c)) * n) >= 0){

        

        if(parent->hasPerVertexNormals()){
            //TODO
        }

        else {
            i.setT(t);
            i.setN(t);
        }

        return true;
    }

    return false;
    */
}


void
Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
        Vec3d a = vertices[(**fi)[0]];
        Vec3d b = vertices[(**fi)[1]];
        Vec3d c = vertices[(**fi)[2]];
        
        Vec3d faceNormal = ((b-a) ^ (c-a));
		faceNormal.normalize();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
		{
            normals[i]  /= numFaces[i];
			if (normals[i].length() != 0)
			{
				normals[i].normalize();
			}
		}
    }

    delete [] numFaces;
}

