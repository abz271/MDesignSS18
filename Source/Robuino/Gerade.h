#ifndef GERADE_H
#define GERADE_H

#include "Vec.h"

class Gerade {
public:
	Vec o;		// Ortsvektor
	Vec r;		// Richtungsvektor
	float k;	// Lambda
public:
	Gerade();
	Gerade(Vec o, Vec r);
	float getIntersection(Gerade g);
	Vec getDirectVec(float i);
};

#endif /* GERADE_H */
