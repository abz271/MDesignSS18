#ifndef STATEMACHINE_H
#define	STATEMACHINE_H

#include "Arduino.h"
#include "Navigation.h"
#include "Gerade.h"

class StateMachine {
private:
	Navigation Navi;
	int switchPin = 52;				// Einschalter

	// Geschwindigkeitsfahrparameter
	int speedmaxturn = 100;
	int speedStartUp = 45;
	int speedRegulated = 150;
	int speedmax = 255;
	int speedStop = 0;

	// Ausweichverhalten
	bool Master = true;
	float actualAvoidAngle = 0;
	float a = 0; float b =  0; float c = 0; float d = 0;

	// Zeitverhalten
	unsigned long timeCur = millis();
	unsigned long playTime = millis();
	const unsigned long interval = 1000;
	const unsigned long intervalStop = 3000;
	const unsigned long intervalDrive = 1500;
	const unsigned long intervalPlaytime = 90000;
public:
	StateMachine();
	void evalStateMachine();
	void UpdateData();
	void applyOutputToProcess();

	Navigation& getNavigation();
};

#endif	/* STATEMACHINE_H */

