#include "DebuggingTestClass.h"
#include "OdometrieTest.h"

float x = 45.7;
float y = 27.3;
float alpha = 90.05;

OdometrieTest odo(x, y, alpha);
DebuggingTestClass debug(odo);

void setup() {
  Serial.begin(9600);
}

void loop() {
  debug.transmitData();
}
