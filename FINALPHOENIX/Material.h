////////////////////////////////////////
// Material.h
////////////////////////////////////////

#ifndef CSE168_MATERIAL_H
#define CSE168_MATERIAL_H

#include "Intersection.h"
#include "Core.h"

////////////////////////////////////////////////////////////////////////////////

class Material {
public:
	virtual void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit)=0;
	virtual void GenerateSample(Color &col,const Vector3 &in,Vector3 &out, Intersection &hit)=0;	// changed hit to non const...
	virtual float getRedAttenuation() {return 0.0f;}
	virtual float getGreenAttenuation() {return 0.0f;}
	virtual float getBlueAttenuation() {return 0.0f;}
	virtual void setRedAttenuation(float r) {}
	virtual void setGreenAttenuation(float g) {}
	virtual void setBlueAttenuation(float b) {}
};

////////////////////////////////////////////////////////////////////////////////

#endif
