#include "OdometrieTest.h"

OdometrieTest::OdometrieTest(float x, float y, float alpha){
	this->x = x;
	this->y = y;
	this->alpha = alpha;
}

float OdometrieTest::getX(){
	return x;
}

float OdometrieTest::getY(){
	return y;
}

float OdometrieTest::getAlpha(){
	return alpha;
}