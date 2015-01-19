////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#ifndef CSE168_LAMBERTMATERIAL_H
#define CSE168_LAMBERTMATERIAL_H

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial:public Material {
public:
	void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		col.Scale(DiffuseColor,1.0f/PI);
	}

	virtual void GenerateSample(Color &col,const Vector3 &in,Vector3 &out, Intersection &hit) {
		float randomS = float(rand())/RAND_MAX;	// Random between 0 and 1
		float randomT = float(rand())/RAND_MAX;

		float u, v;
		u = 2.0f*PI*randomS;
		v = sqrt(1.0f - randomT);

		float px, py, pz;
		px = v * cos(u);
		py = sqrt(randomT);
		pz = v * sin(u);

		Vector3 n;
		n = hit.Normal;
		n.Normalize();

		Vector3 rayd;
		rayd = px*hit.TangentU + pz*hit.TangentV + py*n;
		rayd.Normalize();
		
		out = rayd;
		col = DiffuseColor;

	}

	void SetDiffuseColor(Color c) {
		DiffuseColor = c;
	}

private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////

#endif
