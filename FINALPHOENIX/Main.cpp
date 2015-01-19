////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "MeshObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "Sphere.h"
#include "PlaneObject.h"
#include "BoxTreeObject.h"
#include "LambertMaterial.h"
#include "FresnelMetal.h"
#include "FresnelDielectric.h"
#include "AshikhminMaterial.h"
#include "MotionObject.h"
#include "SphereVolume.h"

#include <ctime>

void project3();

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
	project3();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

float RangeRand(float min,float max) {
	return min+(max-min)*float(rand()) / float(RAND_MAX);
}

void project3() { 
	// Create scene 
	Scene scn; 
	//scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
	//scn.SetSkyColor(Color(0.0f, 0.0f, 0.0f));
	scn.SetSkyColor(Color(0.2f, 0.2f, 0.2f));

	// Materials 
	const int nummtls=4; 
	AshikhminMaterial mtl[nummtls]; 

	// If I comment this out it turns dragon three black? why
	LambertMaterial lambert2; 
	lambert2.SetDiffuseColor(Color(0.6f,0.6f,0.6f));			// Had to change the name of lambert's set diffuse wtf
 
	// Diffuse															// swapping mtl 0 with 3 messes up the third dragon
	mtl[0].SetSpecularLevel(0.0f); 
	mtl[0].SetDiffuseLevel(1.0f); 
	mtl[0].SetDiffuseColor(Color(0.7f,0.7f,0.7f)); 
 
	// Roughened copper 
	mtl[1].SetDiffuseLevel(0.0f); 
	mtl[1].SetSpecularLevel(1.0f); 
	mtl[1].SetSpecularColor(Color(0.9f,0.6f,0.5f)); 
	mtl[1].SetRoughness(100.0f,100.0f); 
 
	// Anisotropic gold 
	mtl[2].SetDiffuseLevel(0.0f); 
	mtl[2].SetSpecularLevel(1.0f); 
	mtl[2].SetSpecularColor(Color(0.95f,0.7f,0.3f)); 
	mtl[2].SetRoughness(1.0f,1000.0f); 
 
	// Red plastic 
	mtl[3].SetDiffuseColor(Color(1.0f,0.1f,0.1f)); 
	mtl[3].SetDiffuseLevel(0.8f); 
	mtl[3].SetSpecularLevel(0.2f); 
	mtl[3].SetSpecularColor(Color(1.0f,1.0f,1.0f)); 
	mtl[3].SetRoughness(1000.0f,1000.0f); 

	// Material setup //
	FresnelMetal fmtl;
	fmtl.setNt(4.37f);
	fmtl.setKt(0.37f);
	fmtl.setMetalColor(Color(0.9f*0.9f,0.4f*0.4f,0.15f*0.15f));

	FresnelMetal fmtl2;
	fmtl2.setNt(0.37f);
	fmtl2.setKt(2.82f);
	//fmtl2.setMetalColor(Color(1.0f*1.0f,1.0f*1.0f,1.0f*1.0f));


	/*FresnelDielectric fdial;
	fdial.setNi(1.0003f);
	fdial.setNt(2.31f);
	fdial.setRedAttenuation(200.3f);
	fdial.setGreenAttenuation(0.3f);
	fdial.setBlueAttenuation(0.3f);*/
	// for ice: attenuate all red, almost no blue or green slight less blue than green nt 1.31
	FresnelDielectric fdial;
	fdial.setNi(1.0003f);
	fdial.setNt(2.42f);
	fdial.setRedAttenuation(0.0f);
	fdial.setGreenAttenuation(0.0f);
	fdial.setBlueAttenuation(0.0f);

	FresnelDielectric fdial2;
	fdial2.setNi(1.0003f);
	fdial2.setNt(2.42f);
	fdial2.setRedAttenuation(0.0f);
	fdial2.setGreenAttenuation(0.0f);
	fdial2.setBlueAttenuation(0.0f);

	FresnelDielectric fdial3;
	fdial3.setNi(1.0003f);
	fdial3.setNt(2.42f);
	fdial3.setRedAttenuation(0.0f);
	fdial3.setGreenAttenuation(1.0f);
	fdial3.setBlueAttenuation(1.0f);

	LambertMaterial lamberttest; 
	lamberttest.SetDiffuseColor(Color(0.6f,0.6f,0.6f));
 
	/////////// Crystal setup //////////// 

	MeshObject dragon; 
	dragon.LoadPLY("crystal.ply");		// diamond 14 is decent

	// Create box tree 
	BoxTreeObject tree; 
	tree.Construct(dragon); 
	
	/////////// Phoenix setup //////////// 

	MeshObject fenix;
	fenix.LoadPLY("fenix2.ply");
	//finex.Smooth();

	BoxTreeObject fenixTree;
	fenixTree.Construct(fenix);

	Matrix34 fenixMtx;
	fenixMtx.MakeRotateX(-PI/2.0f);
	Matrix34 transY;
	transY.MakeTranslate(Vector3(0.0f,150.0f,0.0f));
	fenixMtx.Dot(transY, fenixMtx);
	//Matrix34 scaler;
	//scaler.MakeScale(0.005f);
	//fenixMtx.Dot(scaler, fenixMtx);

	InstanceObject *fenixObj = new InstanceObject(fenixTree);
	fenixObj->SetMatrix(fenixMtx);
	fenixObj->SetMaterial(&fmtl);
	scn.AddObject(*fenixObj);
	
	////////// Create Diamond lights //////////////////
	Matrix34 mtx; 
	//for(int i=0;i<nummtls;i++) { 
	for(int i=0;i<2;i++) { 
		InstanceObject *inst=new InstanceObject(tree); 
		
		Matrix34 crystalMtx;
		crystalMtx.MakeRotateX(-PI/2.0f);
		Matrix34 ctransY;
		ctransY.MakeTranslate(Vector3(-7.0f + (i*13.0f),-2.8f,0.0f));
		crystalMtx.Dot(ctransY, crystalMtx);
		Matrix34 cscaler;
		cscaler.MakeScale(100.0f);
		crystalMtx.Dot(cscaler, crystalMtx);

		//mtx.MakeRotateX(-PI / 2.0f);
		//mtx.MakeTranslate(-500.0f, -500.0f, 0.0f);

		inst->SetMatrix(crystalMtx); 
		//inst->SetMaterial(&lamberttest); 

		if(i == 0) {
			inst->SetMaterial(&fdial);
		}
		if(i == 1) {
			inst->SetMaterial(&fdial2);
		}
		
		scn.AddObject(*inst); 
	} 

	// Add volume
	/*Sphere volposition;
	volposition.SetRadius(20.0f);
	volposition.SetCenter(Vector3(20.0f, 555.0f, 0.0f));
	volposition.SetMtl(fdial);
	scn.AddObject(volposition);

	Sphere volposition2;
	volposition2.SetRadius(20.0f);
	volposition2.SetCenter(Vector3(-20.0f, 555.0f, 0.0f));
	volposition2.SetMtl(fdial);
	scn.AddObject(volposition2);*/

	SphereVolume blob;
	blob.SetCenter(Vector3(-540.0f, 50.0f, 0.0f));
	//blob.SetCenter(Vector3(-0.025f, 0.14f, -0.1f));
	blob.SetRadius(100.0f);
	blob.setMaxStep();
	Color blobColor(0.95f, 0.0f, 0.0f);
	blob.setBaseColor(blobColor);
	blob.setBrightness(100000.0f);
	scn.AddVolume(blob);

	SphereVolume blob2;
	blob2.SetCenter(Vector3(520.0f, 50.0f, 0.0f));
	//blob.SetCenter(Vector3(-0.025f, 0.14f, -0.1f));
	blob2.SetRadius(100.0f);
	blob2.setMaxStep();
	Color blob2Color(0.0f, 0.65f, 0.65f);
	blob2.setBaseColor(blob2Color);
	blob2.setBrightness(100000.0f);
	scn.AddVolume(blob2);

	SphereVolume sun;
	sun.SetCenter(Vector3(0.0f, 400.0f, -200.0f));
	//blob.SetCenter(Vector3(-0.025f, 0.14f, -0.1f));
	sun.SetRadius(400.0f);
	sun.setMaxStep();
	Color sunColor(1.0f, 0.5f, 0.0f);
	sun.setBaseColor(sunColor);
	sun.setBrightness(100000.0f);
	scn.AddVolume(sun);

	SphereVolume eye1;
	eye1.SetCenter(Vector3(20.0f, 555.0f, 0.0f));
	//blob.SetCenter(Vector3(-0.025f, 0.14f, -0.1f));
	eye1.SetRadius(5.0f);
	eye1.setMaxStep();
	Color eye1Color(0.0f, 1.0f, 0.0f);
	eye1.setBaseColor(eye1Color);
	eye1.setBrightness(1000.0f);
	scn.AddVolume(eye1);

	SphereVolume eye2;
	eye2.SetCenter(Vector3(-20.0f, 555.0f, 0.0f));
	//blob.SetCenter(Vector3(-0.025f, 0.14f, -0.1f));
	eye2.SetRadius(5.0f);
	eye2.setMaxStep();
	Color eye2Color(0.0f, 1.0f, 0.0f);
	eye2.setBaseColor(eye2Color);
	eye2.setBrightness(1000.0f);
	scn.AddVolume(eye2);



	
	MeshObject background;
	background.MakeBox(4000.0f,4000.0f,1.0f);

	InstanceObject backInst(background);
	Matrix34 backmtx;
	backmtx.MakeTranslate(0.0f, 0.0f, -1500.0f);
	backInst.SetMatrix(backmtx);
	backInst.SetMaterial(&fmtl);	// try changing this up
	scn.AddObject(backInst);

 
	// Create lights 
	/*DirectLight sunlgt; 
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f)); 
	sunlgt.SetIntensity(1.0f); 
	sunlgt.SetDirection(Vector3(2.0f, -3.0f, -2.0f)); 
	scn.AddLight(sunlgt); */


	
	// Create camera 
	Camera cam; 
	//cam.LookAt(Vector3(-0.05f,0.04f,0.04f),Vector3(-.012f,0.025f,0.0f),Vector3(0.0f,1.0f,0.0f));	// regular project 3 view
	cam.LookAt(Vector3(0.0f,0.0f,2515.0f),Vector3(0.0f,0.0f,0.0f),Vector3(0.0f,1.0f,0.0f)); // origin model analyze view
	//cam.LookAt(Vector3(-0.22f,0.1f,0.23f),Vector3(0.0f,0.15f,-0.15f),Vector3(0.0f,1.0f,0.0f));	// regular project 3 view
	//cam.LookAt(Vector3(-0.5f,0.25f,-0.2f),Vector3(0.0f,0.15f,-0.15f),Vector3(0.0f,1.0f,0.0f));	// extra credit project 3
	
	cam.SetFOV(40.0f); 
	cam.SetAspect(1.33f); 
	//cam.SetResolution(800,600); 
	cam.SetResolution(1300,1000); 
	cam.SetSuperSample(2); 
	
	// Focal jank
	//cam.setAperature(0.013f);
	cam.setAperature(0.0f);

	cam.setFocalPlane(0.60f);
 
	// Render image 
	cam.Render(scn); 
	cam.SaveBitmap("bsb.bmp"); 
}


////////////////////////////////////////////////////////////////////////////////
