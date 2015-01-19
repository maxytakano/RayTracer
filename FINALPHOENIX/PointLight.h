////////////////////////////////////////
// PointLight.h
////////////////////////////////////////

#ifndef CSE168_POINTLIGHT_H
#define CSE168_POINTLIGHT_H

#include "Light.h"

////////////////////////////////////////////////////////////////////////////////

class PointLight:public Light {
public: 
	PointLight() {}
	float Illuminate(const Vector3 &pos, Color &col, Vector3 &toLight, Vector3 &ltPos) { 
		toLight = Position - pos; 
		float bright = Intensity / toLight.Magnitude2(); // Inverse square falloff 
		toLight.Normalize(); 
		col = BaseColor; 
		ltPos = Position; 
		return bright; 
	}
	void SetPosition(Vector3 &pos) {
		Position = pos;
	}
 
private: 
	Vector3 Position; 
}; 


////////////////////////////////////////////////////////////////////////////////

#endif
