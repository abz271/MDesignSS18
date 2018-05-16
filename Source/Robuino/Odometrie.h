#include <Arduino.h>
#include "Kommunikation.h"

#ifndef ODOMETRIE_H
#define ODOMETRIE_H

class Odometrie {
private:
	// Geometrie des Fahrzeugs
    float Achsabstand = 273.0*(360.0/361.0);	// in mm

	// Position
    float x_odometrie = 0;
    float y_odometrie = 0;
    float alpha_odometrie = 90;

    // Rad Geometrie
    const float radDurchmesserLinks = 52.85;
    const float radUmfangLinks = radDurchmesserLinks * PI;

    const float radDurchmesserRechts = 52.70;
    const float radUmfangRechts = radDurchmesserRechts * PI;

    // Encoder sind global, in der cpp definiert
    // Rotary Encoder Methoden
	void getDiffWheelDistance(float& leftWheelChange, float& rightWheelChange);
	void getDiffWheelAngle(float& leftWheelChange, float& rightWheelChange);
public:
    // Nach aussen benötigte Methoden
    Odometrie();
    void updateOdometrie();
    void setPosition(int x, int y);

    // Getter
    float getAngle();
    int getX_position();
    int getY_position();

    // Setter
    void setAngle(float alpha_odometrie);
    void setXposition (int x_odometrie);
    void setYposition (int y_odometrie);

    // Test Methoden
    void testOdometrie();
    void testRotary();
    void testBerechnung(float diffLeft[], float diffRight[], int arraySize);
};

#endif /* ODOMETRIE_H */






