////////////////////////////////////////
// SphereVolume.cpp
////////////////////////////////////////

#include "SphereVolume.h"

////////////////////////////////////////////////////////////////////////////////

void SphereVolume::SetRadius(float r) {
	radius = r;
}

////////////////////////////////////////////////////////////////////////////////

void SphereVolume::SetCenter(Vector3 pos) {
	center = pos;
}

////////////////////////////////////////////////////////////////////////////////

bool SphereVolume::Intersect(const Ray &ray, Intersection &hit) {

	Vector3 q;
	float t;
	t = -(ray.Origin - center).Dot(ray.Direction);
	q = ray.Origin - ( (ray.Origin - center).Dot(ray.Direction) ) * ray.Direction;

	// Check if the ray is in the sphere
	if ( (q - center).Magnitude2() <= (radius*radius) && t < hit.HitDistance && t > EPSILON ) {
		float a;
		a = sqrt( (radius*radius) - (q - center).Magnitude2() );

		Vector3 q1, q2;
		q1 = ray.Origin + (t-a)*ray.Direction;
		q2 = ray.Origin + (t+a)*ray.Direction;
		hit.Q1 = q1;
		hit.Q2 = q2;
		hit.maxStep = maxStepSize;	
		hit.traceDistance = (q1.Distance(q2));		// might wanna double check this and above 3
		hit.volumeColor = baseColor;
		hit.sphereCenter = center;
		hit.sphereRadius = radius;

		/*if (ray.Origin.Distance(q2) > ray.Origin.Distance(hit.Position)) {
			hit.Q2 = hit.Position;
		}*/
	

		// !!!! I think this entire segment can be removed just need q1, q2
		Vector3 n; 
		if ( (t - a) >= 0 ) {		// Ray is hitting at q1
			n = (q1 - center) / radius;

			Vector3 rrr(0.0f, 1.0f, 0.0f);
			if (rrr.Dot(n) == 1.0f || rrr.Dot(n) == -1.0f) {
				rrr.Set(n.y, 0.0f, 0.0f);
			}
			hit.TangentU.Cross(rrr,n); 
			hit.TangentU.Normalize(); 
			hit.TangentV.Cross(n,hit.TangentU); 

			hit.Position = q1;
			hit.HitDistance = t;
			hit.Normal = n;
			return true;
		}
		else return false;
		// !!! This was removed, used to be 2 lines up. I don't think need
		// to know if we hit q2, only use it for ray marching
		/*else if ( (t + a) >= 0 ) {	// Ray is hitting at q2
			n = (q2 - center) / radius;

			hit.Position = q2;
			hit.HitDistance = t;
			hit.Normal = n;
			return true;
		}*/
	}
	else return false;

}

////////////////////////////////////////////////////////////////////////////////

