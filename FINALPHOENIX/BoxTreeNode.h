////////////////////////////////////////
// BoxTreeNode.h
////////////////////////////////////////

#ifndef CSE168_BOXTREENODE_H
#define CSE168_BOXTREENODE_H

#define MAXTRIANGLESPERBOX 10		// How many triangles at the lowest level of boxing

#include "Ray.h"
#include "Intersection.h"
#include "Triangle.h"

////////////////////////////////////////////////////////////////////////////////

class BoxTreeNode {
public: 
	BoxTreeNode() {} 
	~BoxTreeNode() {} 
	bool IntersectChildren(const Ray &ray, Intersection &hit);
	bool boundingBoxIntersect(const Ray &ray, Intersection &hit);
	void Construct(int count, Triangle **tri); 
	void BoxTreeNode::printer(int counting);

	static int leaves;

private: 
	Vector3 BoxMin, BoxMax;
	int numberTriangles;
	BoxTreeNode *Child1, *Child2; 
	Triangle *Tri[MAXTRIANGLESPERBOX];
}; 


////////////////////////////////////////////////////////////////////////////////

#endif