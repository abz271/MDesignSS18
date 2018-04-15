
#include "Arduino.h"
#include "StateMachine.h"

StateMachine StateM;
void setup() {

	Serial.begin(9600);
	Serial.println("Hallo, Robuino");
	// Startparameter der Odometrie setzen ( x, y, Winkel)
	StateM.getNavigation().setStartParameters(300, 1700, 270);
}


void loop() {
		StateM.UpdateData();			// Alle Werte aktualsieren
		StateM.evalStateMachine();		// Statemachine durchlaufen
}
