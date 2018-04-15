/*
* Simple Odometrie-Mock-class. Simulates coordinates and angle.
* Author: Max Rostock
* Date: 28.03.2018
*/

#ifndef ODOMETRIETEST_H
#define ODOMETRIETEST_H

class OdometrieTest{
private:
	float x;
	float y;
	float alpha;
public:
	OdometrieTest(float x, float y, float alpha);
	float getX();
	float getY();
	float getAlpha();
};

#endif