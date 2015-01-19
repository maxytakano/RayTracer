////////////////////////////////////////
// Triangle.cpp
////////////////////////////////////////

#include "Triangle.h"

////////////////////////////////////////////////////////////////////////////////

bool Triangle::Intersect(const Ray &ray, Intersection &hit) {
	Matrix34 A;
	Vector3 negd, n, x;
	float detA, t, alpha, beta;

	Vector3 vA, vB, vC;
	Vector3 nA, nB, nC;
	Vector3 vAB, vAC;
	vA = Vtx[0]->Position;
	vB = Vtx[1]->Position;
	vC = Vtx[2]->Position;
	nA = Vtx[0]->Normal;
	nB = Vtx[1]->Normal;
	nC = Vtx[2]->Normal;
	vAB = vB - vA;
	vAC = vC - vA;

	negd = ray.Direction;
	negd.Negate();

	A.a = negd;
	A.b = vAB;
	A.c = vAC;

	Vector3 AcrossB;
	AcrossB.Cross(vAB, vAC);
		
	Vector3 OminusA;
	OminusA = (ray.Origin - vA);
	Vector3 OminusAcrossAC;
	OminusAcrossAC.Cross(OminusA, vAC);
	Vector3 ABcrossOminusA;
	ABcrossOminusA.Cross(vAB, OminusA);

	detA = negd.Dot((AcrossB)); 
	float invdetA = 1.0f/detA;
	t = (OminusA.Dot(AcrossB)) * invdetA ;
	alpha = (negd.Dot(OminusAcrossAC)) * invdetA ;
	beta = (negd.Dot(ABcrossOminusA)) * invdetA;

	float alphaPlusBeta;
	alphaPlusBeta = alpha + beta;

	if ( alphaPlusBeta < 1 && alpha > 0 && beta > 0 && t > EPSILON && t < hit.HitDistance ) {
		Color color;

		x = vA + alpha*vAB + beta*vAC;
		n = (1-alpha-beta)*nA + alpha*nB + beta*nC;
		n.Normalize();

		Vector3 rrr(0.0f, 1.0f, 0.0f);
		if (rrr.Dot(n) == 1.0f || rrr.Dot(n) == -1.0f) {
			rrr.Set(n.y, 0.0f, 0.0f);
		}
		hit.TangentU.Cross(rrr,n); 
		hit.TangentU.Normalize(); 
		hit.TangentV.Cross(n,hit.TangentU); 
		

		hit.HitDistance = t; 
		hit.Position = x;
		hit.Mtl = Mtl;
		hit.Normal = n;
			
		return true;
	}
	else {
		return false;		
	}
				
}
////////////////////////////////////////////////////////////////////////////////
