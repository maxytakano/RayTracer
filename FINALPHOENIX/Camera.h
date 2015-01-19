////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#ifndef CSE168_CAMERA_H
#define CSE168_CAMERA_H

#include "Vector3.h"
#include "Scene.h"
#include "Matrix34.h"
#include "Bitmap.h"

////////////////////////////////////////////////////////////////////////////////

class Camera { 
public: 
	Camera() {}	
 
	void SetFOV(float f) {
		vfov = PI*f/180.0f;
	}
	void SetAspect(float a) {
		aspect = a;
	}
	void SetResolution(int x,int y) {
		XRes = x;
		YRes = y;
	}
	void LookAt(Vector3 &pos,Vector3 &target,Vector3 &up) {
		WorldMatrix.d = pos;
		WorldMatrix.c = WorldMatrix.d - target;
		WorldMatrix.c.Normalize();	
		WorldMatrix.a.Cross( up, WorldMatrix.c);
		WorldMatrix.a.Normalize();
		WorldMatrix.b.Cross(WorldMatrix.c, WorldMatrix.a);
	}
 
	void Render(Scene &s); 
	void SaveBitmap(char *filename);

	void SetSuperSample(int s) {
		superSamples = s;
	}

	Vector3 Camera::generateAperatureOrigin();

	void setAperature(float a) {
		aperture = a;
	}

	void setFocalPlane(float f) {
		focalPlane = f;
	}

private: 
	// Camera focus stuff
	float aperture;		// Diameter of the lens
	float focalPlane;	// Distance of focal plane in front of the camera

	int XRes,YRes; 
	int superSamples;
	Matrix34 WorldMatrix; 
	float vfov; 
	float aspect; 
	Bitmap BMP; 
};

////////////////////////////////////////////////////////////////////////////////

#endif
