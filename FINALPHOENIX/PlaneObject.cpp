////////////////////////////////////////
// PlaneObject.cpp
////////////////////////////////////////

#include "PlaneObject.h"
#include "LambertMaterial.h"

////////////////////////////////////////////////////////////////////////////////

bool PlaneObject::Intersect(const Ray &ray, Intersection &hit) {

	float t;
	if ( ray.Direction.Dot(n) != 0 ) {		// Making sure the ray is not paralell to the plane
		t = (d - ray.Origin.Dot(n)) / (ray.Direction.Dot(n));
		if ( t >= EPSILON ) {
			hit.HitDistance = t;
			hit.Position = ray.Origin + t*ray.Direction;
			hit.Mtl = Mtl;
			hit.Normal = n;
			return true;
		}
	}

	return false;

}

////////////////////////////////////////////////////////////////////////////////

