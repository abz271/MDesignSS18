#include "DebuggingTestClass.h"

DebuggingTestClass::DebuggingTestClass(OdometrieTest& newOdo) : odo(newOdo){
	x = 0;
	y = 0;
	alpha = 0;
	transmitX = false;
	transmitY = false;
	transmitAlpha = false;
}

void DebuggingTestClass::transmitData(){
	//read command from terminal
	String command = Serial.readString();
	
	x = odo.getX();
	y = odo.getY();
	alpha = odo.getAlpha();
	
	//choose which data should be transmitted, sending character toggles bool-flag
	if(command == "x"){
		transmitX ^= true;
	}
	else if(command == "y"){
		transmitY ^= true;
	}
	else if(command == "a"){
		transmitAlpha ^= true;
	}
	
	if(transmitX){
		Serial.print("x-Koordinate: ");
		Serial.println(x);
	}
	if(transmitY){
		Serial.print("y-Koordinate: ");
		Serial.println(y);
	}
	
	if(transmitAlpha){
		Serial.print("Winkel alpha: ");
		Serial.println(alpha);
	}
}