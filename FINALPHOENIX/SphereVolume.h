////////////////////////////////////////
// SphereVolume.h
////////////////////////////////////////

#ifndef CSE168_SPHEREVOLUME_H
#define CSE168_SPHEREVOLUME_H

#include "Volume.h"

////////////////////////////////////////////////////////////////////////////////

class SphereVolume:public Volume {
public:
	bool Intersect(const Ray &ray, Intersection &hit);
	void SetRadius(float r);
	void SetCenter(Vector3 pos);
	//void RayMarch(Color &col, const Vector3 q1, const Vector3 q2);	

	void setMaxStep() {		// Set after setting the radius
		maxStepSize = (radius * 2.0f) / 10.0f;
	}

	void setBaseColor(Color col) {	
		baseColor = col;
	}

	void Illuminate(const Vector3 &pos, Vector3 &toVolume, Vector3 &VolPos, float &rad, float &bright) { 
		toVolume = center - pos; 
		VolPos = center;
		toVolume.Normalize();
		rad = radius;
		bright = brightness;
	}

	void setBrightness(float b) {
		brightness = b;
	}

private:
	float radius;
	Vector3 center;
	float maxStepSize;
	Color baseColor;
	float brightness;

};

////////////////////////////////////////////////////////////////////////////////

#endif
