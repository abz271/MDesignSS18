/*
* Test-class for testing bluetooth-debugging. Transmits coordinates and angle form OdometrieTest-object if desired.
* Author: Max Rostock
* Date: 28.03.2018
*/

#ifndef DEBUGGING_H
#define DEBUGGING_H

#include "Arduino.h"
#include "OdometrieTest.h"

class DebuggingTestClass{
private:
	OdometrieTest& odo;
	float x;
	float y;
	float alpha;
	
	bool transmitX;
	bool transmitY;
	bool transmitAlpha;
public:
	DebuggingTestClass(OdometrieTest& newOdo);
	void transmitData();
};

#endif