////////////////////////////////////////
// RayTrace.cpp
////////////////////////////////////////

#include "RayTrace.h"
#include "LambertMaterial.h"

////////////////////////////////////////////////////////////////////////////////

bool RayTrace::TraceRay(const Ray &ray, Intersection &hit, int depth) {
	bool justVolume = false;
	bool volumeANDworld = false;
	bool lightAdded = false;

	// Find ray intersection 
	/*if (Scn->Intersect(ray,hit) == false) {		// !!! gonna need to add another check for vols
		hit.Shade=Scn->GetSkyColor(); 
		if (Scn->IntersectVolume(ray,hit)) {
			justVolume = true; 
		}
		else return false;
	} else if (Scn->IntersectVolume(ray,hit)) {
		volumeANDworld = true;
	}*/

	if (Scn->Intersect(ray,hit) == false) {		// !!! original
		hit.Shade=Scn->GetSkyColor(); 
		justVolume = true;
		//return false;
	}
	
	if (justVolume != true) {
		bool lightAdded = true;
		// Compute shade due to lighting 
		Vector3 lightPosition, toLight;
		Color lightColor, pixelColor;
		float intensity, lightDotNormal;

		Ray shadowRay;
		shadowRay.setTime(ray.time);
		Intersection shadowHitInfo;
		shadowRay.Origin = hit.Position;
		hit.Shade=Color::BLACK; 

		// tracing to the lights // 
		for (int i = 0; i < Scn->GetNumLights(); i++) { 
			// Compute light.Illuminate() 
			intensity = Scn->GetLight(i).Illuminate(hit.Position, lightColor, toLight, lightPosition);
			intensity *= PI;					// !!!!!!!!!! Should I do it like this???

			// if(illum==0 or hit.Normal doesn’t face light) continue;  
			lightDotNormal = hit.Normal.Dot(toLight);
			if (intensity == 0 || lightDotNormal < 0) continue;

			// Create shadow ray towards light 
			shadowRay.Direction = toLight;
			shadowHitInfo.HitDistance = lightPosition.Distance(shadowRay.Origin);

			// if(shadow ray isn’t blocked) add contribution from this light to hit.Shade 
			if (Scn->Intersect(shadowRay, shadowHitInfo) == false) {
				hit.Mtl->ComputeReflectance(lightColor, ray.Direction, toLight, hit);
				lightColor.Scale(intensity);			// Is it right to do this here?
				lightColor.Scale(lightDotNormal);
			
				hit.Shade.Add(lightColor);
			}
		} 

		Vector3 toVolume;
		Vector3 volCenter;
		float volRadius;

		Ray volumeRay;
		Intersection volumeHitInfo;

		float volDotNormal;
		float randomS;
		float randomT;
		float volBright;

		float u, v;
		Vector3 randPoint;

		// tracing straight to volumes
		for (int i = 0; i < Scn->GetNumVolumes(); i++) { 

			Scn->GetVolume(i).Illuminate(hit.Position, toVolume, volCenter, volRadius, volBright);

			// random sample generation //
			randomS = float(rand())/RAND_MAX;	// Random between 0 and 1
			randomT = float(rand())/RAND_MAX;	// Random between 0 and 1

			u = 2.0f * PI * randomS;
			v = sqrt(randomT * (1.0f - randomT));

			randPoint.Set(2.0f * v * cos(u), 1.0f - 2.0f*randomT, 2.0f * v * sin(u));
			
			randPoint.Scale(volRadius);			// normalize point to correct scale
			volCenter = volCenter + randPoint;  // add random variation to volume's center

			volumeRay.Origin = hit.Position;
			volumeRay.Direction = volCenter - hit.Position;
			volumeRay.Direction.Normalize();

			// set the volume's time // 
			volumeRay.setTime(ray.time);
			
			
			//volumeRay.Direction = toVolume;
			volumeHitInfo.HitDistance = volCenter.Distance(volumeRay.Origin);
			volDotNormal = hit.Normal.Dot(volumeRay.Direction);
		
			if (Scn->Intersect(volumeRay, volumeHitInfo) == false || volDotNormal >= 0.0f) {
				if (Scn->IntersectVolume(volumeRay,volumeHitInfo)) {	// If the ray hits a volume
					Color volumeRadiance(0.0f,0.0f,0.0f);	// init to no color?
					// !!!! This stuff would ideally be put into the volume class or somwhere else
					// dunno if this will roll a new rand everytime...
		
					float distFromCenter;
		
					// clamp q1 by tmax
					Vector3 traceDirection;
					Vector3 previousPoint;
					Vector3 samplePoint;
					samplePoint = volumeHitInfo.Q1;
		

					while (volumeHitInfo.Q1.Distance(samplePoint) < volumeHitInfo.Q1.Distance(volumeHitInfo.Q2)) {
						traceDirection = volumeRay.Direction;
						traceDirection.Normalize();
						traceDirection.Scale( (float(rand())/float(RAND_MAX)) * volumeHitInfo.maxStep );
						previousPoint = samplePoint;
						samplePoint.Add(traceDirection);

						float distFromCenter;

						distFromCenter = (samplePoint.Distance(volumeHitInfo.sphereCenter));
						//printf("1: %f\n", distFromCenter);
						//printf("rad: %f\n", volRadius);
						distFromCenter /= volRadius;
						//printf("2: %f\n", distFromCenter);
						distFromCenter = 1.0f - distFromCenter;

						//float distColor = 1000.f + (pow(distFromCenter, 1.0f/5.0f)) * (-1000.f);

						//start + t(end- start)
						//distFromCenter *= 100000.0f;
						float volumeDotNormal = hit.Normal.Dot(toVolume);
						
						Color Ex = volumeHitInfo.volumeColor;	
						Ex.Scale( (volBright) );
						//Ex.Scale(volumeDotNormal);
						Vector3 test;
						test = volCenter - hit.Position;
						Ex.Scale(1.0f/test.Magnitude2());
						///Ex.Scale(distFromCenter);
						//Ex.Scale(distColor);
						///Ex.Scale(previousPoint.Distance(samplePoint));	// I don't use dist2 right
						
						volumeRadiance.Add(Ex);	
						
					}
					hit.Shade.Add(volumeRadiance);
				} 
			}
		}
		
		



		// Path tracing recursive section //
		if(depth == MaxDepth) return true; 
	
		Ray recursiveRay;
		recursiveRay.setTime(ray.time);
		recursiveRay.Origin = hit.Position;
		Intersection reflectHitInfo;
		Color reflectColor;


		// generates the sample knowing whether we are in a delectric or not
		hit.Mtl->GenerateSample(reflectColor, ray.Direction, recursiveRay.Direction, hit);

		if(recursiveRay.Direction.Magnitude() != 0.0f) {
			TraceRay(recursiveRay, reflectHitInfo, (depth + 1));
			if (hit.inDielectric == true) {	// not sure if this is the correct time to do beers...
				// beer lambert color calculation	

				// need to get the red attenuation and shift it, create a new var to put in there
				//float redatten;
				//redatten = hit.Mtl->getRedAttenuation();
				//redatten = -100.0f;
				//redatten = redatten * (hit.Position.y * 10000.0f);
				//redatten = redatten + (hit.Position.Distance(Vector3(0.0f,0.04f,0.0f)) * 1000.0f);
				//printf("%f\n", redatten);
				
			

				float red, green, blue;
				red = exp(-hit.Mtl->getRedAttenuation()*reflectHitInfo.HitDistance)*reflectHitInfo.Shade.getRed();
				//red = exp(-redatten*reflectHitInfo.HitDistance)*reflectHitInfo.Shade.getRed();
				green = exp(-hit.Mtl->getGreenAttenuation()*reflectHitInfo.HitDistance)*reflectHitInfo.Shade.getGreen();
				blue = exp(-hit.Mtl->getBlueAttenuation()*reflectHitInfo.HitDistance)*reflectHitInfo.Shade.getBlue();
		
				Color beerColor;
				beerColor.Set(red, green, blue);

				reflectColor.Multiply(beerColor);
				hit.Shade.Add(reflectColor);
			} 
			else {
				reflectColor.Multiply(reflectHitInfo.Shade);
				hit.Shade.Add(reflectColor);
			}
		}
	} 
	
	//////////////////////////// Volume section //////////////////////////////////

    if (Scn->IntersectVolume(ray,hit) && lightAdded == false) {	// If the ray hits a volume
		Color volumeRadiance(0.0f,0.0f,0.0f);	// init to no color?
		// !!!! This stuff would ideally be put into the volume class or somwhere else
		// dunno if this will roll a new rand everytime...
		
		float distFromCenter;
		
		// clamp q1 by tmax
		Vector3 traceDirection;
		Vector3 previousPoint;
		Vector3 samplePoint;
		samplePoint = hit.Q1;
		

		while (hit.Q1.Distance(samplePoint) < hit.Q1.Distance(hit.Q2)) {
			traceDirection = ray.Direction;
			traceDirection.Normalize();
			traceDirection.Scale( (float(rand())/float(RAND_MAX)) * hit.maxStep );
			previousPoint = samplePoint;
			samplePoint.Add(traceDirection);

			float distFromCenter;

			distFromCenter = (samplePoint.Distance(hit.sphereCenter));
			distFromCenter /= hit.sphereRadius;
			distFromCenter = 1.0f - distFromCenter;

			//float distColor = 1000.f + (pow(distFromCenter, 1.0f/5.0f)) * (-1000.f);

			//start + t(end- start)
			//distFromCenter *= 100000.0f;
		
			Color Ex = hit.volumeColor;	
			
			Ex.Scale( (10.0f) );
			Ex.Scale(distFromCenter);
			//Ex.Scale(distColor);
			Ex.Scale(previousPoint.Distance(samplePoint));	// I don't use dist2 right
			// try multiplying here

			volumeRadiance.Add(Ex);	
		}

		hit.Shade.Add(volumeRadiance);
	}

	

	return true; 
}