#include "Arduino.h"
#include "Gerade.h"
#include "Vec.h"

Gerade::Gerade() {
	k = 0;
}

// Gerade anlegen aus Orts- und Richtungsvektor
Gerade::Gerade(Vec o, Vec r) :
		o(o), r(r) {
	k = 0;
}

// Schnittpunkt zweier Geraden berechnen
// Übergabewert ist eine Gerade g
// Rückgabewert ist die Verstärkung Lamdba eines Richtungsvektors, um einen Schnittpunkt zu erhalten
float Gerade::getIntersection(Gerade g) {
	float Lambda = 0;
	Lambda = (g.o.x * g.r.y + o.y * g.r.x - g.o.y * g.r.x - g.r.y * o.x)
			/ (g.r.y * r.x - r.y * g.r.x);

	return Lambda;
}

// Berechnet einen resultierenden Vektor:
// Aus aktuellen Ortsvektor und einem mit Lambda multiplizierten Richtungsvektor
// => Aktuelle Schnittpunktskoordinaten mit einer anderen Geraden kann berechnet werden
Vec Gerade::getDirectVec(float Lambda){
	Vec result(o.x+Lambda*r.x, o.y+Lambda*r.y);
	return result;
}

