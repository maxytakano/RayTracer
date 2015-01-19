////////////////////////////////////////
// AshikhminMaterial.h
////////////////////////////////////////

#ifndef CSE168_ASHIKHMINMATERIAL_H
#define CSE168_ASHIKHMINMATERIAL_H

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class AshikhminMaterial:public Material {
public:
	void ComputeReflectance(Color &col, const Vector3 &in, const Vector3 &out, const Intersection &hit) {

		Vector3 n, l, v, h; 

		n = hit.Normal;
		n.Normalize();

		l = out;					// tolight vector
		l.Normalize();

		v = in;					// view vector
		v.Negate();				// Flip to point to camera
		v.Normalize();

		h = (l + v);	// This is wrong right?
		h.Normalize();

		// Generate any useful aliases
		float VdotN = v.Dot(n);
		float LdotN = l.Dot(n);
		float HdotN = h.Dot(n);
		float HdotL = h.Dot(l);
		float HdotT = h.Dot(hit.TangentU);
		float HdotB = h.Dot(hit.TangentV);

		float rs1, rd1;
		rs1 = specularLevel;
		rd1 = diffuseLevel;
		
		// Compute the diffuse term
		float Pd = (28.0f * rd1) / ( 23.0f * 3.14159f );
		Pd *= (1.0f - rs1);
		Pd *= (1.0f - pow(1.0f - (LdotN / 2.0f), 5.0f));
		Pd *= (1.0f - pow(1.0f - (VdotN / 2.0f), 5.0f));

		// Compute the specular term
		float ps_num_exp = nu * (HdotT * HdotT) + nv * (HdotB * HdotB);
		ps_num_exp /= (1.0f - (HdotN * HdotN));
 
		float Ps_num = sqrt( (nu + 1) * (nv + 1) );
		Ps_num *= pow( HdotN, ps_num_exp );
 
		float Ps_den = 8.0f * PI * HdotL;
		Ps_den *= Max( LdotN, VdotN );
 
		float Ps = rs1 * (Ps_num / Ps_den);		// why rs * this?...
		Ps *= ( rs1 + (1.0f - rs1) * pow( 1.0f - HdotL, 5.0f ) );
		
		Vector3 finalterm = (rs*Ps + rd*Pd);

		Color finalColor;
		finalColor.Set(finalterm.x, finalterm.y, finalterm.z);

		col.Multiply(finalColor);
	}

	virtual void GenerateSample(Color &col,const Vector3 &in,Vector3 &out, Intersection &hit) {
		Vector3 n, k1, k2, h;

		n = hit.Normal;
		n.Normalize();

		k1 = in;					// view vector k1
		k1.Negate();				// Flip to point to camera
		k1.Normalize();

		float rando = float(rand())/float(RAND_MAX);	// Random number between 0 and 1 (specular vs diffuse distribution)
		
		if (rando < specularLevel) {
			col = sC;

			float rand1 = float(rand())/float(RAND_MAX);	// Random number between 0 and 1 (epsilon 1)
			float rand2 = float(rand())/float(RAND_MAX);	// Random number between 0 and 1 (epsilon 2)
			float rand3 = float(rand())/float(RAND_MAX);	// Random number between 0 and 1 (Used for hemisphere distribution)

			// Generate Phi	
			float phi;
			phi = atan( sqrt( (nu + 1.0f) / (nv + 1.0f) ) * tan( (PI*rand1) / 2.0f ) );				

			// Find Theta
			float theta;
			theta = acos(pow( (1.0f - rand2), ( 1.0f / ((nu * (cos(phi) * cos(phi))) + (nv * (sin(phi) * sin(phi))) + 1) ) ));	
		
			// Convert to xyz coordinates from spherical coordinates
			float x, y, z;
			x = sin(theta) * cos(phi);
			y = cos(theta);
			z = sin(theta) * sin(phi);

			// Distrubute to one of four quandrants
			if ( rand3 < 0.5f ) {
				x = x * -1.0f;
			}

			if ( (rand3 >= 0.0f && rand3 <= 0.25f) || (rand3 >= 0.5f && rand3 <= 0.75f) ) {
				z = z * -1.0f;
			}

			// Multiply through matrix to get halfway vector
			h = x*hit.TangentU + z*hit.TangentV + y*n;
			h.Normalize();

			// Calculate k2 using h
			k2 = -k1 + 2.0f * (k1.Dot(h)) * h;
			k2.Normalize();

			if (k2.Dot(n) < 0) {
				k2.Set(0.0f, 0.0f, 0.0f);
			}

			out = k2;
		}
		else {
			col = dC;
			
			float randomS = float(rand())/RAND_MAX;	// Random number between 0 and 1 (epsilon 1)
			float randomT = float(rand())/RAND_MAX; // Random number between 0 and 1 (epsilon 2)

			// Calculate out based on cosine weighted hemisphere
			float uD, vD;
			uD = 2.0f*PI*randomS;
			vD = sqrt(1.0f - randomT);

			float px, py, pz;
			px = vD * cos(uD);
			py = sqrt(randomT);
			pz = vD * sin(uD);

			Vector3 Dsample;
			Dsample = px*hit.TangentU + pz*hit.TangentV + py*n;
			Dsample.Normalize();

			out = Dsample;
		}
	}

	void SetDiffuseLevel(float dl) {
		diffuseLevel = dl;
	}
	
	void SetSpecularLevel(float sl) {
		specularLevel = sl;
	}

	void SetSpecularColor(Color sc) {
		sC = sc;
		rs.x = sc.getRed();
		rs.y = sc.getGreen();
		rs.z = sc.getBlue();
	}

	void SetDiffuseColor(Color dc) {
		dC = dc;
		rd.x = dc.getRed();
		rd.y = dc.getGreen();
		rd.z = dc.getBlue();
	}
	
	void SetRoughness(float n1, float n2) {
		nu = n1;
		nv = n2;
	}


private:
	Vector3 rs, rd;
	float specularLevel, diffuseLevel;
	float nv, nu;
	Color sC, dC;
};

////////////////////////////////////////////////////////////////////////////////

#endif
	