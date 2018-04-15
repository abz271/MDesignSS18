#ifndef KOMMUNIKATION_H
#define KOMMUNIKATION_H
#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>


class Kommunikation {
private:

	int positionAddress = 1;
	int hardwareAddress = 4;

	int stringLength = 60;

	void DataFromHardware(String& comString);
	void DataToHardware(String comString);
	void DataFromPosition(byte* comString);

public:

	Kommunikation();
	bool getStopEnemy();
	bool getPosition(float& xPos, float& yPos, float& angle);

	// Zum testen Objekt im Robi.cpp anlegen und dann
	// diese Funktion einfach im loop laufen lassen
	void testAsMaster();
	void testAsSlave();
	void testKommunikation();



};

#endif /* KOMMUNIKATION_H */
