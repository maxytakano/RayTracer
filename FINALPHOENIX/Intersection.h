////////////////////////////////////////
// Intersection.h
////////////////////////////////////////

#ifndef CSE168_INTERSECTION_H
#define CSE168_INTERSECTION_H

#include "Vector3.h"
#include "Color.h"
class Material;

////////////////////////////////////////////////////////////////////////////////

class Intersection {
public:
	Intersection()			{HitDistance=1e10; Mtl=0; inDielectric = false;}

public:
	// Ray intersection data
	float HitDistance;
	Vector3 Position;
	Vector3 Normal;
	Material *Mtl;
	Vector3 TangentU, TangentV;
	bool inDielectric;	

	// Shaded color
	Color Shade;

	// Volume stuff not sure where to put all dis JANK
	Vector3 Q1, Q2;
	float maxStep;
	float traceDistance;
	Color volumeColor;
	Vector3 sphereCenter;
	float sphereRadius;
};

////////////////////////////////////////////////////////////////////////////////

#endif
