#include "Arduino.h"
#include "GaussianRandom.h"

#define DEBUG 0

#define MEAN 10000
#define STD 500

GaussianRandom r = GaussianRandom(MEAN, STD);

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	delay(1000); // Wait for serial to start
}

// The loop function is called in an endless loop
void loop()
{
	Serial.println(r.randLong());
	delay(10);
}
