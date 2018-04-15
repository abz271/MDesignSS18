
#ifndef MOTOR_H
#define	MOTOR_H
class Motor {
private:
	// Pins der H Brücke
	// Left Wheel
	unsigned char pwmA = 10;	// Pwm Signal, steuert Motorgeschwindigkeit
	unsigned char in1 = 22;		// Drehrichtung HIGH	LOW		LOW
	unsigned char in2 = 23;		// Drehrichtung	LOW		HIGH	LOW
	// Right Wheel
	unsigned char in3 = 24;		// Drehrichtung
	unsigned char in4 = 25;			// Drehrichtung
	unsigned char pwmB = 11;	// Pwm Signal, steuert Motorgeschwindigkeit

	// Geschwindigkeiten der Räder
	unsigned int driveOffset = 10;				// Offset zur Korrektur bei Zickzackfahrt
    unsigned char maxVelocity = 255;			// Wert zwischen 0 und 255, maximale Geschwindigkeit

    unsigned char nextVelocityPwmLeft = 0;		// Ziel Geschwindigkeit
    unsigned char nextVelocityPwmRight = 0;		// Ziel Geschwindigkeit



public:

    Motor();
    ~Motor();

    void updateVelocity();
    void driveStraightRegulated(unsigned char velocity, float difference);
    void driveStraight();
    void turn(float velocity);
    void stoppInstantForward(unsigned char velocity);
    void stop();
};

#endif	/* MOTOR_H */

