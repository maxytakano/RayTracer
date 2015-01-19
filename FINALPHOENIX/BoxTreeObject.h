////////////////////////////////////////
// BoxTreeObject.h
////////////////////////////////////////

#ifndef CSE168_BOXTREEOBJECT_H
#define CSE168_BOXTREEOBJECT_H

#include "MeshObject.h"
#include "BoxTreeNode.h"

////////////////////////////////////////////////////////////////////////////////

class BoxTreeObject:public Object {
public: 
	BoxTreeObject() {}
	~BoxTreeObject() {}
	void Construct(MeshObject &mesh) {
		int a;
		a = mesh.getNumTriangles();
		Triangle *trianglePointer = mesh.getTriangles();
		Triangle **triangleArray = new Triangle*[a];
		int i;
		for (i = 0; i < a; i++) {
			triangleArray[i] = &(trianglePointer[i]);
		}
		RootNode = new BoxTreeNode;
		RootNode->Construct(a, triangleArray);
		printf("%d\n", RootNode->leaves);

		//int countstart = 0;
		//RootNode->printer(countstart);
		//system("pause");
	}
	bool Intersect(const Ray &ray, Intersection &hit) {
		Intersection volHit;
		if(RootNode->boundingBoxIntersect(ray, volHit)) {
			if(RootNode->IntersectChildren(ray, hit)) {
				return true;
			}
			else return false;
		}
		else return false;
	}
 
private: 
	BoxTreeNode *RootNode; 
}; 

////////////////////////////////////////////////////////////////////////////////

#endif