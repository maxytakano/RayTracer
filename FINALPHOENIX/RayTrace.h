////////////////////////////////////////
// RayTrace.h
////////////////////////////////////////

#ifndef CSE168_RAYTRACE_H
#define CSE168_RAYTRACE_H

#include "Core.h"
#include "Intersection.h"
#include "Scene.h"

////////////////////////////////////////////////////////////////////////////////

class RayTrace {
public: 
	RayTrace(Scene &s) {
		Scn = &s;
		MaxDepth = 10;
	}
	bool TraceRay(const Ray &ray, Intersection &hit, int depth=1); 
 
private: 
	Scene *Scn; 
 
	// Settings 
	int MaxDepth;
 
	// Statistics 
	int PrimaryRays; 
	int SecondaryRays; 
	int ShadowRays; 
}; 

////////////////////////////////////////////////////////////////////////////////

#endif
