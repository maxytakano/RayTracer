////////////////////////////////////////
// BoxTreeNode.cpp
////////////////////////////////////////

#include "BoxTreeNode.h"
int BoxTreeNode::leaves = 0;
////////////////////////////////////////////////////////////////////////////////

void BoxTreeNode::Construct(int count, Triangle **tri) {
	BoxMin.x = 1000000.0;
	BoxMin.y = 1000000.0;
	BoxMin.z = 10000000.0;
	BoxMax.x = -1000000.0;
	BoxMax.y = -1000000.0;
	BoxMax.z = -1000000.0;

	// Compute BoxMin & BoxMax to fit around all tri’s
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < 3; j++) {
			if (tri[i]->GetVtx(j).Position.x < BoxMin.x) BoxMin.x = tri[i]->GetVtx(j).Position.x;
			if (tri[i]->GetVtx(j).Position.y < BoxMin.y) BoxMin.y = tri[i]->GetVtx(j).Position.y;
			if (tri[i]->GetVtx(j).Position.z < BoxMin.z) BoxMin.z = tri[i]->GetVtx(j).Position.z;

			if (tri[i]->GetVtx(j).Position.x > BoxMax.x) BoxMax.x = tri[i]->GetVtx(j).Position.x;
			if (tri[i]->GetVtx(j).Position.y > BoxMax.y) BoxMax.y = tri[i]->GetVtx(j).Position.y;
			if (tri[i]->GetVtx(j).Position.z > BoxMax.z) BoxMax.z = tri[i]->GetVtx(j).Position.z;
		}
	}
	
	// Check if this is a leaf node 
	if(count <= MAXTRIANGLESPERBOX) { 
		this->leaves++;
		// Copy triangles to BoxTreeNode’s Tri array 
		for (int i = 0; i < count; i++) {
			Tri[i] = tri[i];
		}

		Child1 = NULL;
		Child2 = NULL;
		numberTriangles = count;
		return; 
	} 
	
	numberTriangles = 0;
 
	// Determine largest box dimension x, y, or z
	float xLen, yLen, zLen;
	float maxDim;
	xLen = abs(BoxMax.x - BoxMin.x);
	yLen = abs(BoxMax.y - BoxMin.y);
	zLen = abs(BoxMax.z - BoxMin.z);

	maxDim = Max(xLen, yLen, zLen);
	int largestDim = 0;

	if (maxDim == xLen) {
		largestDim = 0;
	}
	else if (maxDim == yLen) {
		largestDim = 1;
	}
	else if (maxDim == zLen) {
		largestDim = 2;
	}


	float splittingPlane;
	splittingPlane = maxDim;
	splittingPlane = splittingPlane / (float) 2;

	// Allocate two new temporary arrays 
	Triangle **tri1 = new Triangle*[count]; 
	Triangle **tri2 = new Triangle*[count]; 
	int count1, count2; 
	count1 = 0;
	count2 = 0;

	// Place triangles into group 1 or group 2
	for (int i = 0; i < count; i++) {
		// Compute center of triangle & determine which side of splitting plane 
		Vector3 center;
		center = tri[i]->GetVtx(0).Position + tri[i]->GetVtx(1).Position + tri[i]->GetVtx(2).Position;
		center.Scale(0.33333f);  // fix dat value

		// Point is on the plane
		/*if (center[largestDim] == splittingPlane + BoxMin[largestDim]) {
			// Add to appropriate group 
			tri1[count1] = tri[i];
			count1++;
		}
		else */if (center[largestDim] < (splittingPlane + BoxMin[largestDim])) {		// Point is on side normal is pointing
			// Add to appropriate group 
			tri1[count1] = tri[i];
			count1++;
		}
		else if (center[largestDim] > (splittingPlane + BoxMin[largestDim])) {		// Point is on opposite normal side
			// Add to appropriate group 
			tri2[count2] = tri[i];
			count2++;
		}

	}
 
	// Check if either group is empty. If so, move (at least) 1 triangle into that group
	if (count1 == 0) {
		tri1[0] = tri2[count2 - 1];
		count2--;
		count1++;
	}
	else if (count2 == 0) {
		tri2[0] = tri1[count1-1];
		count1--;
		count2++;
	}
 
	// Recursively build sub-trees 
	Child1=new BoxTreeNode; 
	Child2=new BoxTreeNode; 

	Child1->Construct(count1,tri1); 
	Child2->Construct(count2,tri2); 
 
	// Free up arrays 
	delete []tri1; delete []tri2; 
}

////////////////////////////////////////////////////////////////////////////////

bool BoxTreeNode::boundingBoxIntersect(const Ray &ray, Intersection &hit) {
	float tx1, tx2, ty1, ty2, tz1, tz2;
	float tmax, tmin;
	float invDirX = 1.0f / ray.Direction.x;
	float invDirY = 1.0f / ray.Direction.y;
	float invDirZ = 1.0f / ray.Direction.z;

	Vector3 t1, t2;

	//t1 = (BoxMin - ray.Origin) / ray.Direction;
	//t2 = (BoxMax - ray.Origin) / ray.Direction;

	tx1 = (BoxMin.x - ray.Origin.x) * invDirX;
	tx2 = (BoxMax.x - ray.Origin.x) * invDirX;
	ty1 = (BoxMin.y - ray.Origin.y) * invDirY;
	ty2 = (BoxMax.y - ray.Origin.y) * invDirY;
	tz1 = (BoxMin.z - ray.Origin.z) * invDirZ;
	tz2 = (BoxMax.z - ray.Origin.z) * invDirZ;

	Vector3 tmins;
	Vector3 tmaxs;

	if (tx1 < tx2) {
		tmins.x = tx1;
		tmaxs.x = tx2;
	}
	else {
		tmins.x = tx2;
		tmaxs.x = tx1;
	}
	if (ty1 < ty2) {
		tmins.y = ty1;
		tmaxs.y = ty2;
	}
	else {
		tmins.y = ty2;
		tmaxs.y = ty1;
	}
	if (tz1 < tz2) {
		tmins.z = tz1;
		tmaxs.z = tz2;
	}
	else {
		tmins.z = tz2;
		tmaxs.z = tz1;
	}


	tmin = Max(tmins.x, tmins.y, tmins.z);
	tmax = Min(tmaxs.x, tmaxs.y, tmaxs.z);

	/*tmin = Max(Min(tx1,tx2), Min(ty1,ty2), Min(tz1,tz2));
	tmax = Min(Max(tx1,tx2), Max(ty1,ty2), Max(tz1,tz2));*/

	/*tmin = Max(Min(t1.x,t2.x), Min(t1.y,t2.y), Min(t1.z,t2.z));
	tmax = Min(Max(t1.x,t2.x), Max(t1.y,t2.y), Max(t1.z,t2.z));*/
	if(tmin > tmax) {
		return false;
	}

	if (tmin <= tmax) {
		if (tmin < 0) {
			//if (tmax < hit.HitDistance) {  // !!!!!! DO I NEED THIS CHECK HERE?
				hit.HitDistance = 0;
				return true;
			//}
		}
		else {
			if (tmin < hit.HitDistance) {
				hit.HitDistance = tmin;
				return true;
			}
		}
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////

bool BoxTreeNode::IntersectChildren(const Ray &ray, Intersection &hit) {

	bool success = false;

	if (numberTriangles > 0) {
		for(int i=0; i < numberTriangles; i++) { 
			if(Tri[i]->Intersect(ray,hit)) {
				success = true;
			}
		}
		return success;
	}

	Intersection volHitC1, volHitC2;

	Child1->boundingBoxIntersect(ray, volHitC1);
	Child2->boundingBoxIntersect(ray, volHitC2);

	if (volHitC1.HitDistance < volHitC2.HitDistance) {
		if (volHitC1.HitDistance < hit.HitDistance) {
			if (Child1->IntersectChildren(ray, hit)) success = true;
		}
		if (volHitC2.HitDistance < hit.HitDistance) {
			if (Child2->IntersectChildren(ray, hit)) success = true;
		}
	}
	else {
		if (volHitC2.HitDistance < hit.HitDistance) {
			if (Child2->IntersectChildren(ray, hit)) success = true;
		}
		if (volHitC1.HitDistance < hit.HitDistance) {
			if (Child1->IntersectChildren(ray, hit)) success = true;
		}
	}

	return success;
}

////////////////////////////////////////////////////////////////////////////////

void BoxTreeNode::printer(int counting) {
	//printf("BoxMax: %f, %f, %f\n", BoxMax.x, BoxMax.y, BoxMax.z);
	//printf("BoxMin: %f, %f, %f\n", BoxMin.x, BoxMin.y, BoxMin.z);
	//printf("Number Triangles: %d\n", numberTriangles);
	/*for(int i = 0; i < numberTriangles; ++i) {
		printf("Triangle--\n");
		Tri[i]->GetVtx(0).Position.Print();
		Tri[i]->GetVtx(1).Position.Print();
		Tri[i]->GetVtx(2).Position.Print();
		printf("--------\n");
	}*/
	//counter++;
	int counter = counting;
	counter++;
	if(counter > 100) {
		printf("%d\n", counter);
	}

	if (Child1 != NULL) {
		Child1->printer(counter);
	}
	if (Child2 != NULL) {
		Child2->printer(counter);
	}
}

//int BoxTreeNode::countLeaves() {

//}