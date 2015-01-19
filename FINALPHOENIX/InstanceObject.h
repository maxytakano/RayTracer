////////////////////////////////////////
// InstanceObject.h
////////////////////////////////////////

#ifndef CSE168_INSTANCEOBJECT_H
#define CSE168_INSTANCEOBJECT_H

#include "Object.h"
#include "Matrix34.h"

////////////////////////////////////////////////////////////////////////////////

class InstanceObject:public Object {
public: 
	InstanceObject() {}
	InstanceObject(Object &obj) {
		Child = &obj;
		Mtl = NULL;
	}
	bool Intersect(const Ray &ray, Intersection &hit) {
														
		Ray ray2; 
		Inverse.Transform(ray.Origin, ray2.Origin); 
		Inverse.Transform3x3(ray.Direction, ray2.Direction); 
		if(Child->Intersect(ray2, hit)==false) return false; 
		Matrix.Transform(hit.Position, hit.Position); 
		Matrix.Transform3x3(hit.Normal, hit.Normal); 
		hit.HitDistance=ray.Origin.Distance(hit.Position); // Correct for any scaling 
		
		if (Mtl != NULL) {
			hit.Mtl = Mtl;
		}
					
		return true; 
	} 
	void SetChild(Object &obj) {
		Child = &obj;
	}
	void SetMatrix(Matrix34 &mtx) {
		Matrix = mtx;
		Inverse = mtx;
		Inverse.Inverse();
	}
	
	void SetMaterial(Material *m) {
		Mtl = m;
	}


 
private: 
	Material *Mtl;
	Matrix34 Matrix; 
	Matrix34 Inverse; // Pre-computed inverse of Matrix 
	Object *Child; 
}; 

////////////////////////////////////////////////////////////////////////////////

#endif
