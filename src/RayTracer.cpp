// The main ray tracer.

#pragma warning (disable: 4786)

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"

#include "parser/Tokenizer.h"
#include "parser/Parser.h"

#include "ui/TraceUI.h"
#include <cmath>
#include <algorithm>

extern TraceUI* traceUI;

#include <iostream>
#include <fstream>

using namespace std;

// Use this variable to decide if you want to print out
// debugging messages.  Gets set in the "trace single ray" mode
// in TraceGLWindow, for example.
bool debugMode = false;
bool anti_aliasing = true;

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
Vec3d RayTracer::trace( double x, double y )
{

	// Clear out the ray cache in the scene for debugging purposes,
	Vec3d ret;

		if (!traceUI->isMultithreading())
			scene->intersectCache.clear();

		ray r( Vec3d(0,0,0), Vec3d(0,0,0), ray::VISIBILITY );

    	scene->getCamera().rayThrough( x,y,r );
		ret = traceRay( r, Vec3d(1.0,1.0,1.0), traceUI->getDepth() );
		ret.clamp();
 
	return ret;
}

Vec3d RayTracer::traceRay( const ray& r, const Vec3d& thresh, int depth )
{
    isect i;

    if( scene->intersect( r, i ) ) {

        /* Return the sum of these three color intensities:
         * - Direct Component
         * - Reflective Component
         * - Refractive Component */

        /* 1. Determine the direct color with the pong illumination model
         *
         * Gets the material for the surface that was intersected,
         * and asks that material to provide a color for the ray. */
        const Material& m = i.getMaterial();
        Vec3d color = m.shade(scene, r, i);

        // Recursion based on depth
        if (depth > 0) {

            /* 2. Determine the reflected color component
             *
             * Calculate the reflection vector, then make a
             * recursive call to the traceRay function */
            ray reflectedRay = getReflectedRay(i, r);

            // Reflective property
            Vec3d v1 = m.kr(i);

            // Recursive call to get reflection vector
            Vec3d v2 = traceRay(reflectedRay, Vec3d(1.0, 1.0, 1.0), depth - 1);

            //Vec3d reflectedColor = m.kr(i) * traceRay(reflectedRay, Vec3d(1.0,1.0,1.0), (depth - 1));
            Vec3d reflectedColor = prod(v1,v2);

            color = color + reflectedColor;

            /* 3. Determine the refracted color component
             * (recursive call to the traceRay function,
             * test and handle for total internal refraction) */
        }
        return color;

    } else {
        // No intersection.  This ray travels to infinity, so we color
        // it according to the background color, which in this (simple) case
        // is just black.

        return Vec3d( 0.0, 0.0, 0.0 );
    }
}

RayTracer::RayTracer()
	: scene( 0 ), buffer( 0 ), buffer_width( 0 ), buffer_height( 0 ), m_bBufferReady( false )
{
}


RayTracer::~RayTracer()
{
	delete scene;
	delete [] buffer;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return sceneLoaded() ? scene->getCamera().getAspectRatio() : 1;
}

bool RayTracer::loadScene( const char* fn )
{
	ifstream ifs( fn );
	if( !ifs ) {
		string msg( "Error: couldn't read scene file " );
		msg.append( fn );
		traceUI->alert( msg );
		return false;
	}
	
	// Strip off filename, leaving only the path:
	string path( fn );
	if( path.find_last_of( "\\/" ) == string::npos )
		path = ".";
	else
		path = path.substr(0, path.find_last_of( "\\/" ));

	// Call this with 'true' for debug output from the tokenizer
	Tokenizer tokenizer( ifs, false );
    Parser parser( tokenizer, path );
	try {
		delete scene;
		scene = 0;
		scene = parser.parseScene();
	} 
	catch( SyntaxErrorException& pe ) {
		traceUI->alert( pe.formattedMessage() );
		return false;
	}
	catch( ParserException& pe ) {
		string msg( "Parser: fatal exception " );
		msg.append( pe.message() );
		traceUI->alert( msg );
		return false;
	}
	catch( TextureMapException e ) {
		string msg( "Texture mapping exception: " );
		msg.append( e.message() );
		traceUI->alert( msg );
		return false;
	}


	if( ! sceneLoaded() )
		return false;

	
	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];

	}
	memset( buffer, 0, w*h*3 );
	m_bBufferReady = true;
}

void RayTracer::tracePixel( int i, int j )
{
	Vec3d col;

	if( ! sceneLoaded() )
		return;

	

	if(anti_aliasing){

		col= Vec3d(0,0,0);
		int sample_rate = 10;

		for(int p = 0; p < sample_rate; p++){
			for(int q = 0; q < sample_rate; q++){
		
				double x = double(i + (p + (double)rand()/RAND_MAX) / sample_rate)/double(buffer_width);
				double y = double(j + (q + (double)rand()/RAND_MAX)/ sample_rate)/double(buffer_height);

				col += trace(x,y);
			}
		}
		col = col/(sample_rate * sample_rate);
	}


	else{
		double x = double(i)/double(buffer_width);
		double y = double(j)/double(buffer_height);
		col = trace( x, y);
	}

	

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

ray RayTracer::getReflectedRay( isect i, ray r )
{
    /* A ray has a position where the ray starts,
     * and a direction (which should be normalized) */

    // REFLECTION POSITION
    // Reflection ray starts where intersection occurs
    Vec3d Q = r.at(i.t);

    // REFLECTION DIRECTION
    // R = d + 2 (-d * N) N
    Vec3d R = r.getDirection() + 2 * (-r.getDirection() * i.N) * i.N;
    R.normalize();

    return ray ( Q, R, ray::REFLECTION );
}

