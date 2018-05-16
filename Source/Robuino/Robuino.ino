#include "Arduino.h"
#include "StateMachine.h"

StateMachine stateM;
void setup() {
	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	// Startparameter der Odometrie setzen ( x, y, Winkel)
	stateM.getNavigation().setStartParameters(0,0, 270);
}

void loop() {
	stateM.UpdateData();			// Alle Werte aktualsieren
	stateM.evalStateMachine();		// Statemachine durchlaufen
}
