////////////////////////////////////////
// MotionObject.h
////////////////////////////////////////

#ifndef CSE168_MOTIONOBJECT_H
#define CSE168_MOTIONOBJECT_H

#include "Object.h"
#include "Matrix34.h"

////////////////////////////////////////////////////////////////////////////////

class MotionObject:public Object {
public: 
	MotionObject() {}
	MotionObject(Object &obj) {
		Child = &obj;
		Mtl = NULL;
	}
	bool Intersect(const Ray &ray, Intersection &hit) {						
		
		Matrix34 Matrix;		// Lerp over each component of matrix
		Matrix.a = InitialMatrix.a + ray.time*(FinalMatrix.a - InitialMatrix.a);
		Matrix.b = InitialMatrix.b + ray.time*(FinalMatrix.b - InitialMatrix.b);
		Matrix.c = InitialMatrix.c + ray.time*(FinalMatrix.c - InitialMatrix.c);
		Matrix.d = InitialMatrix.d + ray.time*(FinalMatrix.d - InitialMatrix.d);

		Matrix34 Inverse;		// Get the inverse matrix on the fly
		Inverse = Matrix;
		Inverse.Inverse();
		
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

	void SetInitialMatrix(Matrix34 &mtx) {
		InitialMatrix = mtx;
		//InitialInverse = mtx;
		//InitialInverse.Inverse();
	}
	void SetFinalMatrix(Matrix34 &mtx) {
		FinalMatrix = mtx;
		//FinalInverse = mtx;
		//FinalInverse.Inverse();
	}

	
	void SetMaterial(Material *m) {
		Mtl = m;
	}

private: 
	Material *Mtl;
	Matrix34 InitialMatrix, FinalMatrix; 
	Matrix34 InitialInverse, FinalInverse; // Pre-computed inverse of Matrix 
	Object *Child; 
}; 

////////////////////////////////////////////////////////////////////////////////

#endif
