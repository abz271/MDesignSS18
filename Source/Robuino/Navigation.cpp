#include "Navigation.h"

// KlassenKonstruktor: Bei Initialisierung wird Zielposition auf 0 gesetzt
// 					=> Punkt X_Koordinaten[0] Y_Koordinaten[0]
Navigation::Navigation(){
	Position = 0;
}
// Getter, der die Referenz auf die in Navigation benutzte Odometrieinstanz wiedergibt
Odometrie& Navigation::getOdometrie(){
	return Odo;
}
// Getter, der die Referenz auf die in Navigation benutzte Motorinstanz wiedergibt
Motor& Navigation::getMotor(){
	return Moto;
}
// Getter, der die Referenz auf die in Navigation benutzte Kommunikatioinstanz wiedergibt
Kommunikation& Navigation::getJSON(){
	return JSON;
}

// Updatemethode:
// Aktualisiert alle Werte aus der Odometrieberechnung und der Motoransteuerung.
// Aktuelle Position berechnet sich auf zwei Möglichkeiten:
// 1 Möglichkeit: Werte von der Positionsbestimmung
// 2 Möglichkeit: Werte aus der Odometrie
void Navigation::UpdateData() {
	Odo.updateOdometrie();
	Moto.updateVelocity();
	float xFromPosition, yFromPosition, angleFromPosition;
	// beim Case startUp schon erlauben, dass Daten aus der Position übernehmen, aber nicht x_aktuell und y_aktuell gesetzt werden => Odometrie fahren
	// Grund: Keine Ausreisser beim Fahren 0,0 auf x,x => Falscher Winkel
	if (StateStartUp){
		if (JSON.getPosition(xFromPosition, yFromPosition, angleFromPosition)){
			x_PositionteamOld = int (xFromPosition);
			y_PositionteamOld = int (yFromPosition);
			x_PositionteamNew = int (xFromPosition);
			y_PositionteamNew = int (yFromPosition);
		}
	}

	// Daten von Positionteam sind gut
	if (JSON.getPosition(xFromPosition, yFromPosition, angleFromPosition) && takePosition){

		// Sind die Daten vom Positionteam die gleichen?
		// Grund: Positionsteam taktet nur mit 5 Hz, Odometrie mit max. Arduinogeschw.
		// => Odometrie werte nehmen
		// Falls ungleich: Positionswerte nehmen und Odometriewerte überschreiben
		if((x_PositionteamOld == x_PositionteamNew) && (y_PositionteamOld == y_PositionteamNew)) {
			x_aktuell = Odo.getX_position();
			y_aktuell = Odo.getY_position();
		}else{
			x_aktuell = int(xFromPosition);
			y_aktuell = int(yFromPosition);
			Odo.setPosition(x_aktuell, y_aktuell);
			//Odo.setAngle(angleFromPosition);		// Funktionalität seitens Team 1 nicht gegeben, zukünftig implementieren
			}
		// Falls beide Punte unterschiedlich sind, Neue Werte in alte Werte schreiben
		if ((x_PositionteamNew != x_PositionteamOld) && (y_PositionteamNew != y_PositionteamOld)){
			x_PositionteamOld = x_PositionteamNew;
			y_PositionteamOld = x_PositionteamNew;
		}

		// aktualisieren des neuen Punktes
		x_PositionteamNew = int(xFromPosition);
		y_PositionteamNew = int(yFromPosition);

		// Daten von Positionteam sind schlecht
		// => Nur mit Odometrie fahren
	} else {
		x_aktuell = Odo.getX_position();
		y_aktuell = Odo.getY_position();

		x_PositionteamOld = x_PositionteamNew;
		y_PositionteamOld = y_PositionteamNew;
	}
}


// Berechnung des Winkels zwischen der aktuellen Position und der Zielposition
float Navigation::getCalculateAngle(int x, int y) {

	double delta_x = x - x_aktuell;
	double delta_y = y - y_aktuell;
	double Winkel = atan2(delta_y, delta_x) * 180 / PI;

	return Winkel;
}
// Berechnet die Vektorlänge zwischen dem aktuellem Standort und dem Zielpunkt.
float Navigation::getLengthToPosition(int x, int y) {
	int delta_x = x - x_aktuell;
	int delta_y = y - y_aktuell;
	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

// Sich zum Zielwinkel drehen
void Navigation::turnToTargetAngle() {
	e = targetAngle - Odo.getAngle();	// Differenz zwischen Zielwinkel und Odometriewinkel
	Moto.turn(signum(e) * speed);		// Motordrehung initialisieren => Bei Vorzeichenwechsel Drehwechsel!
	
	// Ohne die Serial.print funktioniert die Drehung nicht, Ursache ungeklärt
	Serial.print("  targetAngle: ");
	Serial.print(targetAngle);
	Serial.print("  OdoAngle: ");
	Serial.print(Odo.getAngle());
	Serial.print(" speed: ");
	Serial.print(speed);
	Serial.print(" e: ");
	Serial.println(e);
	
	if (abs(e) < 50){					// Differenz geringer als 50?
		speed -= 5;						// Geschwindigkeit der Motoren runtersetzen
		if (speed <= 0){
			Moto.stop();
		}
	}
}


// Zum Zielpunkt fahren
// Implementiert als P-Regler, der die Motorgeschwindigkeit auf beiden Rädern ändert(Abhängig von der Abweichung)
void Navigation::driveToTargetPosition(){
	e = getLengthToPosition(X_Koordinaten[Position], Y_Koordinaten[Position]);
	// Implementierung P-Regler
	controlDeviation = getTargetAngle();	// Soll Winkel
	actualDeviation = Odo.getAngle();		// Ist Winkel
	//Reglerdifferenz verstärken und übertragen
	differenceDeviation = (controlDeviation - actualDeviation) * amplifierKp;
	Moto.driveStraightRegulated(speed, differenceDeviation);
}

// Positionserkennung:
// Ist der detektierte Gegenstand im Spielfeld
// Hintergrund: Im Lastenheft sind keine Gegenstande au�er das zweite Fahrzeug im Spielfeld
// => Gegenst�nde am Rand ausblendbar
// Anhand der aktuellen Position(Fahrzeugmittelpunkt) und des eingeschlagenen WInkels (Pythagoras) wird die Position des Objektes ermittelt
bool Navigation::DetectedEnemyInArea(){
	int xPositionObject = 0;
	int yPositionObject = 0;
	bool result = false;
	int radius = 270; 		// Radius des Fahrzeugs 150 + 100 Messentfernung + 20 Offset
	xPositionObject = cos(Odo.getAngle()* PI/180)*radius + x_aktuell;
	yPositionObject = sin(Odo.getAngle()* PI/180)*radius + y_aktuell;

	if (((xPositionObject > 0) && (xPositionObject < 3000)) && ((yPositionObject > 0) && (yPositionObject < 2000))){
		result = true;
	}
	return result;
}

// Erkennung Crash mit Spielfeldr�ndern
// Falls die Entfernung zum Spielfeldrand kleiner als 180 (Fahrzeugmittelpunkt) ist, wird ein True zur�ckgegeben
bool Navigation::CrashIncoming(){
	bool TooClose = false;
	int distance = 180;		// Fahrzeug Radius von 150, maximale Genauigkeit 30 mm vom Spielfeldrand

	Gerade G1(Vec(0, 0), Vec(1, 0));		// G1 und G2 parallel x-Achse
	Gerade G2(Vec(0, 2000), Vec(1, 0));
	Gerade G3(Vec(0, 0), Vec(0, 1));		// G3 und G4 parallel y-Achse
	Gerade G4(Vec(3000, 0), Vec(0, 1));

	// Richtungs- und Ortsvektor des Autos anlegen
	Vec o(x_aktuell, y_aktuell);
	Vec r(Odo.getAngle());
	// Gerade des Autos erzeugen
	Gerade Intersection(o, r);

	//Pr�fen ob, Schnittpunkt mit Spielfeldvektoren existieren
	float a = Intersection.getIntersection(G1);
	float b = Intersection.getIntersection(G2);
	float c = Intersection.getIntersection(G3);
	float d = Intersection.getIntersection(G4);

	if (((a >= 0) && (a < distance)) || ((b >= 0) && (b < distance)) || ((c >= 0) && (c < distance)) || ((d >= 0) && (d < distance))){
		TooClose = true;
	}
	return TooClose;
}

int Navigation::getTargetCoordinateX(){
	return X_Koordinaten[Position];
}

int Navigation::getTargetCoordinateY(){
	return Y_Koordinaten[Position];
}

float Navigation::getDeviation(){
	return e;
}

float Navigation::getSpeed(){
	return speed;
}
float Navigation::getTargetAngle(){
	float targetAngle = getCalculateAngle(X_Koordinaten[Position], Y_Koordinaten[Position]);
	return targetAngle;
}

int Navigation::getPosition(){
	return Position;
}

int Navigation::getY(){
	return y_aktuell;
}

int Navigation::getX(){
	return x_aktuell;
}

float Navigation::getSafetyRadius(){
	return safetyRadius;
}

int Navigation::getMaximalPosition(){
	return maxPosition;
}

void Navigation::setSpeed(int speed){
	this->speed = speed;
}


void Navigation::setTargetAngle(float angle) {
	if (angle >= 360.0){
		angle -= 360.0;
	}
	targetAngle = angle;
}

void Navigation::setNextPosition(){
	Position ++;
}

void Navigation::setPrePositionteams(bool StateStartUp){
	this->StateStartUp = StateStartUp;
}

void Navigation::setPosition(int Position){
	this->Position = Position;
}
void Navigation::setStartParameters(int x, int y, float angle){
	Odo.setXposition(x);
	Odo.setYposition(y);
	Odo.setAngle(angle);
}

void Navigation::setPositionteam(bool Calculate){
	takePosition = Calculate;
}

// Gibt Vorzeichen des übergebenen Wertes zurück
int Navigation::signum(float sign){
	int NumberSign = 0;
	if (sign > 0){
		NumberSign = 1;
	}
	if (sign < 0){
		NumberSign = -1;
	}
	return NumberSign;
}


