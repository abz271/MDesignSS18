#ifndef VEC_H
#define	VEC_H

class Vec {
public:
    float x;				// x Wert des Vektors
    float y;				// y Wert des Vektors
    Vec();					// Konstruktor
    Vec(float angle);		// Konstruktur mit Winkel
    Vec(float x, float y);	// Konstruktor mit x und y Position
    Vec operator+(Vec v);
    float getLength();
    float getAngle();
    float getX();
    float getY();
    void setX(float x);
    void setY(float y);
};

#endif	/* VEC_H */
