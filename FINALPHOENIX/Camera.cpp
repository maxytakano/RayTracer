////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"
#include <math.h>
#include <iostream>
#include "RayTrace.h"

////////////////////////////////////////////////////////////////////////////////
 
void Camera::Render(Scene &s) { 
	Vector3 tl, tr, bl, br;
	int x,y;
	float hfov;

	hfov = 2.0f * atan(aspect * tan(vfov / 2.0f));

	// Find four corners of virtual image
	tl = WorldMatrix.d - WorldMatrix.c + (WorldMatrix.b * tan(vfov / 2.0f)) - (tan(hfov / 2.0f) * WorldMatrix.a );	// top left
	tr = WorldMatrix.d - WorldMatrix.c + (WorldMatrix.b * tan(vfov / 2.0f)) + (tan(hfov / 2.0f) * WorldMatrix.a );	// top right 
	bl = WorldMatrix.d - WorldMatrix.c - (WorldMatrix.b * tan(vfov / 2.0f)) - (tan(hfov / 2.0f) * WorldMatrix.a );	// bot left
	br = WorldMatrix.d - WorldMatrix.c - (WorldMatrix.b * tan(vfov / 2.0f)) + (tan(hfov / 2.0f) * WorldMatrix.a );	// bot right

	Vector3 Point, right, left;

	BMP = *(new Bitmap(XRes, YRes));

	Ray ray;
	ray.Origin = WorldMatrix.d;

	RayTrace rayTracer(s);	

	float pixelSpace = 1.0f / (float)superSamples;
	float pixelOffset = pixelSpace / 2.0f;

	
	for (y = 0; y < YRes; y++) { 
		printf("y: %d\n", y);
		for (x = 0; x < XRes; x++) { 
			Color pixColor=Color::BLACK;
			for (int i = 0; i < superSamples; i++) {
				for (int j = 0; j < superSamples; j++) {					
					left.Lerp( ( ( (float)y + ((float)i * pixelSpace) + pixelOffset) / YRes ),bl,tl);		// Is it okay to do this like this? recalculates
					right.Lerp( ( ( (float)y + ((float)i * pixelSpace) + pixelOffset) / YRes ),br,tr);		// The same Y many times this way... 
					Point.Lerp( ( ( (float)x + ((float)j * pixelSpace) + pixelOffset) / XRes ),left,right);

					ray.Origin = generateAperatureOrigin();		// Camera EC jank for aperature
					ray.Direction = Point - ray.Origin;

					float randomTime = float(rand())/RAND_MAX;
					ray.setTime(randomTime);	// Set a random time between 0 and 1 for the ray

					Vector3 pointAimed = WorldMatrix.d + (focalPlane * ray.Direction);	// Scale by focal plane
					
					ray.Direction = pointAimed - ray.Origin;
					ray.Direction.Normalize();

					Intersection hitInfo;
					rayTracer.TraceRay(ray, hitInfo, 1);

					hitInfo.Shade.FromInt(hitInfo.Shade.ToInt());		// Clamp the color values for black spots
					pixColor.Add(hitInfo.Shade);
				}
			}
			pixColor.Scale(1.0f/float(superSamples*superSamples));
			BMP.SetPixel(x, y, pixColor.ToInt());
		}
	}

}

////////////////////////////////////////////////////////////////////////////////

void Camera::SaveBitmap(char *filename) {
	BMP.SaveBMP(filename);
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Camera::generateAperatureOrigin() {
	Vector3 aperaturePoint;

	// generate random point in aperature diameter circle
	float randomS = float(rand())/RAND_MAX;	// Random between 0 and 1
	float randomT = float(rand())/RAND_MAX;

	randomS *= aperture;
	randomT*= aperture;

	aperaturePoint = WorldMatrix.d + randomS*WorldMatrix.b + randomT*WorldMatrix.a;

	return aperaturePoint;
}


////////////////////////////////////////////////////////////////////////////////


