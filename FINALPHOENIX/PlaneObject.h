////////////////////////////////////////
// PlaneObject.h
////////////////////////////////////////

#ifndef CSE168_PLANEOBJECT_H
#define CSE168_PLANEOBJECT_H

#include "Object.h"

////////////////////////////////////////////////////////////////////////////////

class PlaneObject:public Object {
public:
	PlaneObject() {
		n = Vector3(0, 1, 0);
		x = Vector3(0, 0, 0);
		d = n.Dot(x);
	}
	~PlaneObject() {}

	bool Intersect(const Ray &ray, Intersection &hit);


private:
	Vector3 n, x;
	float d;
	Material *Mtl;
};

////////////////////////////////////////////////////////////////////////////////

#endif
