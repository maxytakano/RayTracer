////////////////////////////////////////
// FresnelDielectric.h
////////////////////////////////////////

#ifndef CSE168_FRESNELDIELECTRIC_H
#define CSE168_FRESNELDIELECTRIC_H

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class FresnelDielectric:public Material {
public:

	void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {
		col.Set(0.0f, 0.0f, 0.0f);
	}

	virtual void GenerateSample(Color &col,const Vector3 &in,Vector3 &out, Intersection &hit) {	
		float rand1 = float(rand())/float(RAND_MAX);	// Random number between 0 and 1 (epsilon 1)

		Vector3 n = hit.Normal;
		Vector3 z;
		Vector3 t;
		bool backFacing = false;	

		n.Normalize();

		if (in.Dot(hit.Normal) > 0.0f) {	// the triangle is back facing
			n.Negate();
			z = (nt / ni) * (in - (in.Dot(n)) * n);
		    t = z - sqrt(1.0f - z.Magnitude2()) * n;
			backFacing = true;
		}
		else {		// The triangle is forward facing
			z = (ni / nt) * (in - (in.Dot(n)) * n);
			t = z - sqrt(1.0f - z.Magnitude2()) * n;
			// BackFacing stays false
		}

		if (z.Magnitude2() > 1.0f) {	// we are reflecting
			out = in - 2 * (in.Dot(hit.Normal)) * hit.Normal;
			out.Normalize();
		}
		else {
			float rpar, rperp, fr;
			rpar = ( nt * hit.Normal.Dot(in) - ni * hit.Normal.Dot(t) ) / 
					( nt * hit.Normal.Dot(in) + ni * hit.Normal.Dot(t) );
			rperp = ( ni * hit.Normal.Dot(in) - nt * hit.Normal.Dot(t) ) / 
					( ni * hit.Normal.Dot(in) + nt * hit.Normal.Dot(t) );
			fr = 0.5f * ((rpar*rpar) + (rperp*rperp));

			if (rand1 < fr) {	
				out = in - 2 * (in.Dot(hit.Normal)) * hit.Normal;
				out.Normalize();
			}
			else {
				out = t;
				out.Normalize();

				if (backFacing == true) {	// We are not in dielectric
					hit.inDielectric = false;	// We refract into the dielectric (inDielectric = true)
				}
				else {		// we are already in dielectric
					hit.inDielectric = true;	// We refract out of the dielectric (inDielectric = false)
				}
			}
		}
		
	}

	void setNt(float n) {
		nt = n;
	}

	void setNi(float i) {
		ni = i;
	}

	float getRedAttenuation() {
		return red;
	}
	float getGreenAttenuation() {
		return green;
	}
	float getBlueAttenuation() {
		return blue;
	}

	void setRedAttenuation(float r) {
		red = r;
	}
	void setGreenAttenuation(float g) {
		green = g;
	}
	void setBlueAttenuation(float b) {
		blue = b;
	}

private:
	float nt, ni;
	float red, green, blue;
};

////////////////////////////////////////////////////////////////////////////////

#endif
