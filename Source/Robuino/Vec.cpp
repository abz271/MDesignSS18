#include "Arduino.h"
#include "Vec.h"

// Konstruktur
Vec::Vec() {
    x = 0;
    y = 0;

}
// Vektor bekannten Winkel berechnen
Vec::Vec(float angle){
	x = cos ((angle)*M_PI/180);
	y = sin ((angle)*M_PI/180);
}

// Vektor aus bekannten x und y Koordinaten anlegen
Vec::Vec(float x, float y) {
    this->x = x;
    this->y = y;
}

// Vectorüberladung, Addition zweier Vektoren
Vec Vec::operator+(Vec v) {
    Vec r(x + v.x, y + v.y);
    return r;
}

// Länge eines Vektors berechnen
float Vec::getLength() {
    return sqrt(x*x + y*y);
}

// Winkel eines Vektors berechnen
float Vec::getAngle() {
    return (atan2(y, x) *180 / M_PI);
}

float Vec::getX(){
	return x;
}

float Vec::getY(){
	return y;
}

void Vec::setX(float x) {
    this->x = x;
}

void Vec::setY(float y) {
    this->y = y;
}
