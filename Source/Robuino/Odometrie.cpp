// Includes
#include "Odometrie.h"
#include <Encoder.h>
#include <Arduino.h>

// Globale Encoder, werden für die Bibliothek benötigt um die Stellung der Räder zu erfassen
// Es war nicht möglich diese als Attribute der Klasse um zu setzen
Encoder leftWheel(2, 6);
Encoder rightWheel(3, 5);

// Konstruktur
Odometrie::Odometrie() {
  alpha_odometrie = 90;
}

void Odometrie::setXposition(int x_odometrie){
	this->x_odometrie = x_odometrie;
}

void Odometrie::setYposition(int y_odometrie){
	this->y_odometrie = y_odometrie;
}

void Odometrie::setAngle(float alpha_odometrie){
	this->alpha_odometrie = alpha_odometrie;
}
/*
 * Funktion setPosition() aktualisiert die Positionsdaten aus der Odometrie
 * mit den Daten aus dem Positionsbestimmungsteam
 */
void Odometrie::setPosition(int x, int y) {
	x_odometrie = x;
	y_odometrie = y;

}

void Odometrie::updateOdometrie() {
	// Variablen
	float leftWheelChange = 0;
	float rightWheelChange = 0;
	float d_center = 0; // relative Bewegung der Mitte des Roboters

	// Werte updaten, aktuelle Werte aus den Encodern holen
	getDiffWheelDistance(leftWheelChange, rightWheelChange);

	// Bewegung, Differenz in der Position des Fahrzeuges berechnen
	d_center = (rightWheelChange - leftWheelChange) / 2;

	// Veränderungen der x und y Position des Fahrzeugs
	// Updaten der Attribute

	x_odometrie += d_center * cos((alpha_odometrie * PI) / 180);
	y_odometrie += d_center * sin((alpha_odometrie * PI) / 180);

	//Winkel des Fahrzeugs berechnen und Attribute updaten
	alpha_odometrie += (-1.0 * (leftWheelChange + rightWheelChange) * 180) / (Achsabstand * PI);
}

float Odometrie::getAngle() {
	return alpha_odometrie;
}

int Odometrie::getX_position() {
	return x_odometrie;
}

int Odometrie::getY_position() {
	return y_odometrie;
}

/*
 * Gibt die Difference der aktuellen und seit der letzten berechnugn zurück gelegten Strecke zurück
 * TODO: Möglicherweise müssen die Vorzeichen geändert werden um die Drehrichtung der Räder zu ber�cksichtigen
 */
void Odometrie::getDiffWheelDistance(float& leftWheelChange,
		float& rightWheelChange) {

	// Variablen für Winkek
	float leftRotaryChange = 0.0;
	float rightRotaryChange = 0.0;

	// Winkelwerte mit getRotaryChange abrufen
	getDiffWheelAngle(leftRotaryChange, rightRotaryChange);

	// Berechnung zurück gelegte Strecke mit dem Umfang der Räder
	leftWheelChange = leftRotaryChange / 360.0 * radUmfangLinks;
	rightWheelChange = rightRotaryChange / 360.0 * radUmfangRechts;
}

/*
 * Berechnet die Änderung des Drehwinkels der Räder
 */
void Odometrie::getDiffWheelAngle(float& leftRotaryChange,
		float& rightRotaryChange) {

	// Variablen für die Encoder Werte anlegen
	float leftRotary = 0.0;
	float rightRotary = 0.0;

	// Werte der Encoder abrufen
	leftRotary = leftWheel.read();
	rightRotary = rightWheel.read();

	// Winkel berechnen und zurück geben
	// Linkes Rad ist negiert, aufgrund der definition Uhrzeigersin = positiv
	// Rechtes Rad ist negiert, aufgrund der definition Uhrzeigersin = positiv
	leftRotaryChange = -1.0 * leftRotary * 360.0 / 2400.0;
	rightRotaryChange = -1.0 * rightRotary * 360.0 / 2400.0;

	// Wieder auf 0 setzen
	leftWheel.write(0);
	rightWheel.write(0);
}

/*
 * Testet die Funktionalität der Funktion
 * getDiffWheelDistance
 */
void Odometrie::testRotary() {
	float left;
	float right;

	static float xLast = -999;
	static float yLast = -999;

	// nur 10 mal pro sekunde abfragen
	static unsigned long timeLast = millis();
	unsigned long timeCur = millis();

	if (timeCur >= timeLast + 500) {
		timeLast = timeCur;

		getDiffWheelDistance(left, right);

		if ((left != xLast || right != yLast)) {

			Serial.print(left, 4);
			Serial.print("    ");
			Serial.print(right, 4), Serial.println(" ");

			xLast = left;
			yLast = right;
		}
	}
}

void Odometrie::testBerechnung(float diffLeft[], float diffRight[],
		int arraySize) {

	float d_center = 0; // relative Bewegung der Mitte des Roboters

	int i = 0;
	for (i = 0; i < arraySize; i++) {
		// Bewegung, Differenz in der Position des Fahrzeuges berechnen
		d_center = (diffRight[i] - diffLeft[i]) / 2;

		// Veränderungen der x und y Position des Fahrzeugs
		// Updaten der Attribute
		x_odometrie += d_center * cos((alpha_odometrie * PI) / 180);
		y_odometrie += d_center * sin((alpha_odometrie * PI) / 180);

		//Winkel des Fahrzeugs berechnen und Attribute updaten
		alpha_odometrie += (-1.0 * (diffLeft[i] + diffRight[i]) * 180)
				/ (Achsabstand * PI);

		if (alpha_odometrie <= 360){
			alpha_odometrie = 0;
		}
		// Alles ausgeben und mal schauen wie es so aussieht
		Serial.print("x_Pos: ");
		Serial.print(x_odometrie);
		Serial.print(", ");
		Serial.print("y_Pos: ");
		Serial.print(y_odometrie);
		Serial.print(", ");
		Serial.print("Alpha: ");
		Serial.print(alpha_odometrie);
		Serial.println();
		Serial.println();
	}
}

void Odometrie::testOdometrie(){
	static int i = 0;

	// nur 10 mal pro sekunde abfragen
	static unsigned long timeLast = millis();
	unsigned long timeCur = millis();

	if (timeCur >= timeLast + 500) {
		timeLast = timeCur;

		updateOdometrie();

		Serial.print("Durchlauf ");
		Serial.print(i);
		Serial.print(" :");
		Serial.print("  x: ");
		Serial.print(x_odometrie);
		Serial.print("  y: ");
		Serial.print(y_odometrie);
		Serial.print(" angle: ");
		Serial.print(alpha_odometrie);
		Serial.println();

		i++;
	}
}
