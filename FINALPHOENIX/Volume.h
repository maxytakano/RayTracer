////////////////////////////////////////
// Volume.h
////////////////////////////////////////

#ifndef CSE168_VOLUME_H
#define CSE168_VOLUME_H

#include "Ray.h"
#include "Intersection.h"

////////////////////////////////////////////////////////////////////////////////

class Volume {
public:
	virtual ~Volume()	{}
	// virtual evaluate at position here
	virtual bool Intersect(const Ray &ray, Intersection &hit)=0;
	virtual void Illuminate(const Vector3 &pos, Vector3 &toVolume, Vector3 &VolPos, float &rad, float &bright)=0;
};

////////////////////////////////////////////////////////////////////////////////

#endif
