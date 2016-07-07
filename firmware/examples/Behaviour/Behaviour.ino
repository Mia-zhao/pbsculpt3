#include "Arduino.h"
#include "behaviour.h"
#include <SoftPWM.h>

Behaviour exponentialBehaviour;

// The number of points
const int n_timeValue = 18;

int divisor = 1;

// Timings in microseconds
unsigned long timeValue[n_timeValue][2] = {
		{0L, 0L},
		{100000L, 30L},
		{200000L, 86L},
		{300000L, 134L},
		{400000L, 174L},
		{500000L, 206L},
		{600000L, 230L},
		{700000L, 246L},
		{800000L, 254L},
		{900000L, 254L},
		{1000000L, 246L},
		{1100000L, 230L},
		{1200000L, 206L},
		{1300000L, 174L},
		{1400000L, 134L},
		{1500000L, 86L},
		{1600000L, 30L},
		{1700000L, 0L}
};

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	delay(1000); // Wait for serial to start

	// Set up test LED
	pinMode(LED_BUILTIN, OUTPUT);

	SoftPWMBegin();
	SoftPWMSet(LED_BUILTIN, 0);

	Serial.println("Initializing behaviour...");
	exponentialBehaviour = Behaviour();

	for( int i = 0; i < n_timeValue; i++ ){
		Point *point = new Point();
		point->time = timeValue[i][0];
		point->value = timeValue[i][1];

		exponentialBehaviour.points.add(point);
	}

	exponentialBehaviour.play();
}

// The loop function is called in an endless loop
void loop()
{
	exponentialBehaviour.loop();

	SoftPWMSet(LED_BUILTIN, exponentialBehaviour.value());

	if( !exponentialBehaviour.isPlaying() ){
		divisor *= 2;
		exponentialBehaviour.setValueDivisor(divisor);
		exponentialBehaviour.play();

		Serial.printf("Playing at 1/%d value\n", divisor);

		if( divisor > 512 ){
			divisor = 1;
		}
	}

	delay(10);
}
