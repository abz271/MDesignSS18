#include "Motor.h"
#include "Odometrie.h"
#include <Arduino.h>

Motor::Motor() {
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
}

// Allgemein: Die Motortreiber werden mit PWM-SIgnalen gefüttert, die über "analogWrite" angesteuert werden
// über die Höhe des PWM Signals (0-255) können Geschwindigkeiten für die Motoren eingestellt werden

Motor::~Motor() {
}

// Geschwindkeit auf die PWM Ausgänge geben
void Motor::updateVelocity(){
	analogWrite(pwmA, nextVelocityPwmLeft);
	analogWrite(pwmB, nextVelocityPwmRight);
}
// Motorstop
void Motor::stop() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = 0;
	nextVelocityPwmRight = 0;
}
// Notbremsung
void Motor::stoppInstantForward(unsigned char velocity){
	digitalWrite(in1, HIGH);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, HIGH);
// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = velocity;
	nextVelocityPwmRight = velocity;
}

// ungeregelte Geradeausfahrt
void Motor::driveStraight(){
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
// Geschwindigkeiten für die Motoren einstellen
	nextVelocityPwmLeft = 150;
	nextVelocityPwmRight = 150;
}

// Geregelte Geradeausfahrt
void Motor::driveStraightRegulated(unsigned char velocity, float difference){
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
// Begrenzung der PWM Drehzahl auf max 250 bei Geschwindigkeit im Programm 150
	if(difference >= 100){
		difference = 100;
	}
	nextVelocityPwmLeft = velocity - char(difference);
	nextVelocityPwmRight = velocity + char(difference);
}

// Drehung des Fahrzeugs auf der Stelle
void Motor::turn(float velocity) {
	if(velocity <= 0) {
		digitalWrite(in1, LOW);
		digitalWrite(in2, HIGH);
		digitalWrite(in3, HIGH);
		digitalWrite(in4, LOW);
	} else {
		digitalWrite(in1, HIGH);
		digitalWrite(in2, LOW);
		digitalWrite(in3, LOW);
		digitalWrite(in4, HIGH);
	}

	nextVelocityPwmLeft = abs(velocity);
	nextVelocityPwmRight = abs(velocity);
}




