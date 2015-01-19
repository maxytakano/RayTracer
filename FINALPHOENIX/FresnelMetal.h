////////////////////////////////////////
// FresnelMetal.h
////////////////////////////////////////

#ifndef CSE168_FRESNELMETAL_H
#define CSE168_FRESNELMETAL_H

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class FresnelMetal:public Material {
public:
	void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		col.Set(0.0f, 0.0f, 0.0f);		// Nothing bounces staight back to camera? even on a metal?
	}

	virtual void GenerateSample(Color &col,const Vector3 &in,Vector3 &out, Intersection &hit) {
		//float rand1 = float(rand())/float(RAND_MAX);	// Random number between 0 and 1 (epsilon 1)
		
		float rpar2, rperp2, fr;
		rpar2 = ( (pow(nt, 2) + pow(kt, 2)) * pow(hit.Normal.Dot(in), 2) + 2 * nt * hit.Normal.Dot(in) + 1 ) / 
				( (pow(nt, 2) + pow(kt, 2)) * pow(hit.Normal.Dot(in), 2) - 2 * nt * hit.Normal.Dot(in) + 1 );
		rperp2 = ( (pow(nt, 2) + pow(kt, 2)) + pow(hit.Normal.Dot(in), 2) + 2 * nt * hit.Normal.Dot(in) ) / 
				( (pow(nt, 2) + pow(kt, 2)) + pow(hit.Normal.Dot(in), 2) - 2 * nt * hit.Normal.Dot(in) );
		fr = 0.5f * (rpar2*rpar2 + rperp2*rperp2);	// amount of reflected 

		out = in - 2 * (in.Dot(hit.Normal)) * hit.Normal;
		out.Normalize();

		// white color and tint color, multiply white by reflectance, and tint by 1 minus that
		Color white(1.0f, 1.0f, 1.0f);
		white.Scale(fr);

		Color finalColor;
		finalColor = metalColor;
		finalColor.Scale(1.0f - fr);

		finalColor.Add(white);
		col = finalColor;


		/*if (rand1 < fr) {
			out = in - 2 * (in.Dot(hit.Normal)) * hit.Normal;
			out.Normalize();
		}
		else {
			out.Set(0.0f,0.0f,0.0f);
		}*/


		//col.Scale(fr);

		//col.Set(.9f, .9f, .9f);
	}

	void setNt(float n) {
		nt = n;
	}

	void setKt(float k) {
		kt = k;
	}
	
	void setMetalColor(Color m) {
		metalColor = m;
	}

private:
	float nt, kt;
	Color metalColor;
};

////////////////////////////////////////////////////////////////////////////////

#endif
