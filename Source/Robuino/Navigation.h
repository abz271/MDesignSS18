#ifndef NAVIGATION_H
#define NAVIGATION_H

// Includes der Klassenheader
#include "Kommunikation.h"
#include "Odometrie.h"
#include "Gerade.h"
#include "Motor.h"

class Navigation {
private:
	// Klassen anlegen
    Kommunikation JSON;
    Odometrie Odo;
    Motor Moto;
private:
    bool StateStartUp = false;	// Auskunft über Statemachinenzustand "StartUp"

    // Attribute f�r die Motoransteuerung
    float speed = 100;		// MotorPWM-Signal für die Vorwärtsfahrt eines Rades
    float targetAngle = 0;	// Zielwinkel
    // Attribute f�r die Positionsbestimmung

    int X_Koordinaten[4] = {0, 0, 0, 0};		// Alpha Fahrzeug Koordinaten in mm
    int Y_Koordinaten[4] = {0, 0, 0, 0};		// Alpha Fahrzeug Koordinaten in mm

    //int X_Koordinaten[6] = {0, 0, 0, 0, 0, 0};		// Beta Fahrzeug Koordinaten in mm
    //int Y_Koordinaten[6] = {0, 0, 0, 0, 0, 0};		// Beta Fahrzeug Koordinaten in mm

    int maxPosition = 3;		// Maximal anzufahrende Punkte; Arrayzählweise ab 0
    int Position;				// Variable zum Auswählen der x und y Koordinaten, Startwert 0

    int x_aktuell = 0;			// aktuelle Position des Fahrzeugs [x]
    int y_aktuell = 0;			// aktuelle Position des Fahrzeugs [y]

    // Werte vom Positionsteam
    int x_PositionteamOld = 0;
    int x_PositionteamNew = 0;
    int y_PositionteamOld = 0;
    int y_PositionteamNew = 0;

    bool takePosition = false;		// Werte von Positionsteam übernehmen? Siehe Statemachine

    // Reglungswerkzeuge
	float controlDeviation = 0;		// Sollwert Geradeausfahrt (Drehwinkel)
	float actualDeviation = 0;		// Istwert Geradeausfahrt (Drehwinkel)
	float differenceDeviation = 0;	// Differenz Soll-Ist-Winkel Geradeausfahrt
	float amplifierKp = 10;			// Verstärkungsfaktor
	float safetyRadius = 23;		// Sicherheitsradius bei anfahren von Punkten
    float e = 0.0;					// Differenz Soll-Ist-Winkel
    float direction = 0;			// Kürzeste Richtung fürs Drehen
public:	// Allgemeines
    Navigation();
    bool PositionInLava();
    void UpdateData();
    void turnToTargetAngle();
    void driveToTargetPosition();
    bool DetectedEnemyInArea();
    bool CrashIncoming();
    Odometrie& getOdometrie();
    Kommunikation& getJSON();
    Motor& getMotor();
private:	// Allgemeines
    int signum(float sign);
public: // Getter
    float getAngle();
    float getLengthToPosition(int x, int y);
    float getCalculateAngle(int x, int y);
    float getSafetyRadius();
    float getTargetAngle();
    float getDeviation();
    float getSpeed();
    int getTargetCoordinateX();
    int getTargetCoordinateY();
    int getCurrentQuarter();
    int getMaximalPosition();
    int getPosition();
    int getY();
    int getX();
public:	// Setter
    void setStartParameters(int x, int y, float angle);
    void setPrePositionteams(bool StateStartup);
    void setDirection(float odo, float ziel);
    void setPositionteam(bool Calculate);
    void setTargetAngle(float angle);
    void setPosition(int Position);
    void setSpeed(int speed);
    void setNextPosition();
	
    void testAngle(float odo, float ziel);
};

#endif /* NAVIGATION_H */

