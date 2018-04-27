#include "Statemachine.h"

enum states {
	initState,
	nextPoint,
	turnToTargetAngle,
	turnToAvoidTargetAngle,
	startUp,
	driveShortlyStraight,
	driveStraightRegulated,
	stopMotor,
	avoidCrash,
	finished,
	finishedOutOfTime
};

static enum states currentState = initState;
static unsigned long timeLast = millis();
static unsigned long timeStop = millis();
static unsigned long timeToPlay = millis();

Navigation& StateMachine::getNavigation(){
	return Navi;
}

StateMachine::StateMachine() {
	pinMode(switchPin, INPUT_PULLUP);
}


// Aktualisiert alle Daten aus Odometrie und Kommunikation
void StateMachine::UpdateData() {
	Navi.UpdateData();
	timeCur = millis();
	playTime = millis();
}

// Statemachine zur Steuerung des Fahrzyklus
void StateMachine::evalStateMachine() {
	// Globale Spielzeit definieren
	if ((playTime - timeToPlay) >= intervalPlaytime){
		if (currentState != initState){
			Navi.setSpeed(speedStop);
			currentState = finishedOutOfTime;
		}
	}
	Navi.setPrePositionteams(true);			// Case = StartUp => Werte des Positionsteam vorbereiten
	Navi.setPositionteam(false);			// Keine Koordinaten vom Positionsteam �bernehmen

	// switch Case für die Ausgaben
	switch (currentState) {
		case initState: {
			timeToPlay = playTime;
		}
			break;
		case nextPoint: {
			playTime = millis();
			break;
		}
		case turnToTargetAngle: {
			Navi.turnToTargetAngle();
		}
			break;
		case turnToAvoidTargetAngle: {
			Navi.turnToTargetAngle();
		}
			break;
		case driveShortlyStraight: {
			Navi.getMotor().driveStraight();
		}
			break;

		case startUp: {
			Navi.driveToTargetPosition();
			Navi.setSpeed(60);
		}
			break;
		case driveStraightRegulated: {
			Navi.setPrePositionteams(false);		// Werte vom Poisitionsteam nicht mehr vorbereiten (gleichsetzen)
			Navi.setPositionteam(true);				//  => Werte sind jetzt nutzbar und können überschrieben werden
			Navi.driveToTargetPosition();
		}
			break;
		case stopMotor: {
			Navi.getMotor().stoppInstantForward(speedmax);	// Nothalt einleiten
			if (timeCur >= timeStop + 40) {					// nach 40ms Motor normal stoppen
				Navi.setSpeed(speedStop);
				Navi.getMotor().stop();
			}
			break;
		}
		case avoidCrash: {
		}
			break;
		case finished: {
			Navi.setTargetAngle(270);						// Beim Erreichen des Zieles um einen Winkel drehen
			Navi.turnToTargetAngle();
		}
			break;
		case finishedOutOfTime: {
			Navi.getMotor().stop();
		}
			break;
	}
	
	switch (currentState) {
	// switch Case für Weiterschaltbedingungen
		// InitState
		case initState: {
			if (!digitalRead(switchPin)) {					// Taster wird abgefragt, um das Programm zu starten
				timeLast = timeCur;
				timeToPlay = playTime;
				currentState = nextPoint;
				Serial.println("nextPoint");
			}
		}
			break;

		// Nächsten Punkt anfahren:
		// Kurzes Interval abwarten dann:
		// Zielwinkel berechnen
		// Zum Zielwinkel drehen
		case nextPoint: {
			if ((timeCur - timeLast) >= interval) {
				Navi.setSpeed(speedmaxturn);
				Navi.setTargetAngle(Navi.getTargetAngle());
				currentState = turnToTargetAngle;
				Serial.println("turnToTargetAngle");
			}
		}
			break;

		// Sich zum Zielwinkel drehen:
		// Wenn Drehung abgeschlossen worden ist, Übergang zur Anfahrfahrt
		// Während der Drehung wird keine Auswertung der Ausweichsensoren gemacht
		case turnToTargetAngle: {
			if (Navi.getSpeed() == speedStop) {
				Navi.setSpeed(speedStartUp);
				timeLast = timeCur;
				currentState = startUp;
				Serial.println("startUp");
			}
		}
			break;
		// Zum Ausweichwinkel drehen
		// Wenn Drehung abgeschlossen worden ist, Übergang zur kurzzeiten Geradeausfahrt
		// Während der Drehung wird keine Auswertung der Ausweichsensoren gemacht
		case turnToAvoidTargetAngle: {
			if (Navi.getSpeed() == speedStop) {
				// Drehung fertig?
				Navi.setSpeed(speedStartUp);
				timeLast = timeCur;
				currentState = driveShortlyStraight;
				Serial.println("driveShortlyStraight");
			}
		}
			break;
		// Anfahrfahrt
		// Kurzfristig mit geringerer Geschwindigkeiten anfahren als im regulären Betrieb, um starkes Ruckeln zu verhindern
		// Falls Gegner erkannt und im Spielfeld sich befindet, Motoren sofort stoppen
		// Falls Anfahrzeit abgelaufen ist, geregelt Geradeausfahrt einleiten
		case startUp: {
			if (Navi.getJSON().getStopEnemy() && Navi.DetectedEnemyInArea()) {
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("stopMotor");
			} else if ((timeCur - timeLast) >= interval) {
				Navi.setSpeed(speedRegulated);
				currentState = driveStraightRegulated;
				Serial.println("driveStraightRegulated");
			}
		}
			break;
		// Für eine kurze Zeit Geradeausfahrt einleiten
		// Falls Gegner erkannt und im Spielfeld sich befindet, Motoren sofort stoppen
		case driveShortlyStraight: {
			if (Navi.getJSON().getStopEnemy()&& Navi.DetectedEnemyInArea()) {
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("stopMotor");
			}else if ((timeCur - timeLast) >= intervalDrive) {
				Navi.setSpeed(speedmaxturn);
				timeLast = timeCur;
				currentState = nextPoint;
				Serial.println("nextPoint");
			}
		}
			break;
		// Geregelt Geradeausfahrt einleiten, bis Zielradius erreicht wird
 		// Falls Gegner erkannt und im Spielfeld sich befindet, Ausweichverhalten einleiten
		// Falls Fahrzeug zu nah an die Planken heranfahren, Ausweichverhalten einleiten
		// Falls Fahrzeug im bestimmten Radius innerhalb des Zielpunktes sich befindet, Motoren abschalten
		case driveStraightRegulated: {
			if (Navi.getJSON().getStopEnemy()&& Navi.DetectedEnemyInArea()) {
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				timeLast = timeCur;
				currentState = avoidCrash;
				Serial.println("avoidCrash");
			} else if (Navi.getDeviation() < Navi.getSafetyRadius()) {
				// Zielkreis erreicht?
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				currentState = stopMotor;
				Serial.println("stopMotor");
			} else if (Navi.CrashIncoming() ){
				// Zu nah an den Spielplanken
				Navi.setSpeed(speedStop);
				timeStop = timeCur;
				timeLast = timeCur;
				currentState = avoidCrash;
				Serial.println("avoidCrash");
			}
		}
			break;
		// Motoren sind ausgeschaltet
		// Falls ein Gegner erkannt worden ist, Ausweichverhalten einleiten
		// Falls der letzte Punkt erreicht worden ist, ist die Fahrt beendet
		// Falls die Motoren aus keinen der obrigen Gründe ausgeschaltet worden sind, nächsten Punkt anfahren
		case stopMotor: {
			// Weiterschalten in AvoidCrash
			if (Navi.getJSON().getStopEnemy()) {
				timeLast = timeCur;
				currentState = avoidCrash;
				Serial.println("avoidCrash");
			// Letzte Position erreicht?
			} else if (Navi.getPosition() == Navi.getMaximalPosition()) {
				Navi.setSpeed(speedmaxturn);
				currentState = finished;
				Serial.println("finished");
			// Wenn an Position gebremst worden ist, nächste Position anfahren
			} else if ((Navi.getSpeed() == 0))  {
				timeLast = timeCur;
				Navi.setNextPosition();
				currentState = nextPoint;
				Serial.println("nextPoint");
			}

		}
			break;
			
		// Ausweichverhalten zur Vermeidung von Kollisionen
		// Anlegen von Spielfeldgeraden. 
		// Richtungsvektor des Roboters um 90 Grad drehen.
		// Schnittpunkt jeder Gerade des Spielfeldes mit gedrehten Richtungsvektor bestimmen (Einheitsvektor) => Lambda
		// Überprüfen, ob Schnittpunkte sich im Spielfeld befinden
		// => Schnittpunkt mit grösstem Lambda ist Ausweichroute
		case avoidCrash: {
			// Spielfeldgeraden anlegen

			/*************G2*************
			 * 							            *
			 * 	  					            *
			 * 							            *
			 G3 	  				            G4
			 * 		 					            *
			 * 							            * 
			 * 						            	*
			 *************G1*************/
			Gerade G1(Vec(0, 0), Vec(1, 0));		// G1 und G2 parallel x-Achse
			Gerade G2(Vec(0, 2000), Vec(1, 0));
			Gerade G3(Vec(0, 0), Vec(0, 1));		// G3 und G4 parallel y-Achse
			Gerade G4(Vec(3000, 0), Vec(0, 1));

			// Vektor des Autos anlegen mit gedrehten Richtungsvektor um 90 Grad
			Vec o(Navi.getX(), Navi.getY());
			Vec r(Navi.getOdometrie().getAngle() + 90);
			// Gerade des Autos erzeugen
			Gerade Intersection(o, r);

			//gedrehter Richtungsvektor: Prüfen ob, Schnittpunkt mit Spielfeldvektoren existieren
			a = Intersection.getIntersection(G1);
			b = Intersection.getIntersection(G2);
			c = Intersection.getIntersection(G3);
			d = Intersection.getIntersection(G4);

			// Schnittpunkte in Vektoren einspeichern
			Vec IntersectionG1 = Intersection.getDirectVec(a);
			Vec IntersectionG2 = Intersection.getDirectVec(b);
			Vec IntersectionG3 = Intersection.getDirectVec(c);
			Vec IntersectionG4 = Intersection.getDirectVec(d);

			//Länge zu Schnittpunkten berechnen
			float aimLength = 0;
			float lengthG1 = Navi.getLengthToPosition(IntersectionG1.getX(),
					IntersectionG1.getY());
			float lengthG2 = Navi.getLengthToPosition(IntersectionG2.getX(),
					IntersectionG2.getY());
			float lengthG3 = Navi.getLengthToPosition(IntersectionG3.getX(),
					IntersectionG3.getY());
			float lengthG4 = Navi.getLengthToPosition(IntersectionG4.getX(),
					IntersectionG4.getY());

			// Überprüfen, welcher Schnittpunkt Sinn ergibt und welcher am weitesten entfernt ist
			// Weit entferntester Schnittpunkt ist die Ausweichroute
			if ((IntersectionG1.getY() >= 0) && (IntersectionG2.getY() <= 2000)) {
				if ((IntersectionG1.getX() >= 0)
						&& (IntersectionG1.getX() <= 3000)) {
					if (lengthG1 > aimLength) {
						aimLength = lengthG1;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG1.getX(), IntersectionG1.getY());
					}
				}
			}
			if ((IntersectionG2.getY() >= 0) && (IntersectionG2.getY() <= 2000)) {
				if ((IntersectionG2.getX() >= 0)
						&& (IntersectionG2.getX() <= 3000)) {
					if (lengthG2 > aimLength) {
						aimLength = lengthG2;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG2.getX(), IntersectionG2.getY());
					}
				}
			}
			if ((IntersectionG3.getY() >= 0) && (IntersectionG3.getY() <= 2000)) {
				if ((IntersectionG3.getX() >= 0)
						&& (IntersectionG3.getX() <= 3000)) {
					if (lengthG3 > aimLength) {
						aimLength = lengthG3;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG3.getX(), IntersectionG3.getY());
					}
				}
			}
			if ((IntersectionG4.getY() >= 0) && (IntersectionG4.getY() <= 2000)) {
				if ((IntersectionG4.getX() >= 0)
						&& (IntersectionG4.getX() <= 3000)) {
					if (lengthG4 > aimLength) {
						aimLength = lengthG4;
						actualAvoidAngle = Navi.getCalculateAngle(
								IntersectionG4.getX(), IntersectionG4.getY());
					}
				}
			}
			// Ausweichwinkel abspeichern
			Navi.setTargetAngle(actualAvoidAngle);

			if ((timeCur - timeLast) >= intervalStop) {
				// Masterfahrzeug: Nach Zeitablauf zu neuem Winkel drehen
				if (Master) {
					timeLast = timeCur;
					Navi.setSpeed(speedmaxturn);
					currentState = turnToAvoidTargetAngle;
					Serial.println("turnToAvoidTargetAngle");
				// Betafahrzeug: Wenn Zeit abgelaufen ist und das andere Fahrzeug verschwunden ist weiterfahren
				} else {
					if (!Navi.getJSON().getStopEnemy()) {
						timeLast = timeCur;
						Navi.setSpeed(speedStartUp);
						currentState = startUp;
						Serial.println("nextPoint");
					}
				}
			}

		}
			break;

		// Mit erneuter Betätigung des Tasters beginnt eine neue Fahrt
		case finished: {
			if (!digitalRead(switchPin)) {
				timeLast = timeCur;
				timeToPlay = playTime;
				Navi.setPosition(0);
				currentState = nextPoint;
				Serial.println("nextPoint");
			}
		}
			break;
		// Mit erneuter Betätigung des Tasters beginnt eine neue Fahrt
		case finishedOutOfTime: {
			if ((!digitalRead(switchPin) && (Navi.getSpeed() == 0))) {
				timeLast = timeCur;
				timeToPlay = playTime;
				Navi.setPosition(0);
				currentState = nextPoint;
				Serial.println("nextPoint");
			}
		}
			break;
	}
}
