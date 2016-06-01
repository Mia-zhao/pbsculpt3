#include "Arduino.h"
#include "behaviour.h"
#include <SoftPWM.h>

Behaviour exponentialBehaviour;

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	delay(1000); // Wait for serial to start

	Serial.println("Starting Firmware...");
	// Set up test LED

	Serial.println("Setting LED mode...");
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.println("Beginning SoftPWM...");
	SoftPWMBegin();

	SoftPWMSet(LED_BUILTIN, 0);

	Serial.println("Initializing behaviour...");
	exponentialBehaviour = Behaviour();

	// Timings in microseconds
	unsigned long timeValue[18][2] = {
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
	int n_timeValue = 18;

	for( int i = 0; i < n_timeValue; i++ ){
		exponentialBehaviour.points.add(timeValue[i]);

		Serial.printf("Added point: %d, %d\n",
				exponentialBehaviour.points.get(exponentialBehaviour.points.size()-1)[0],
				exponentialBehaviour.points.get(exponentialBehaviour.points.size()-1)[1]);
	}

	Serial.println("Finished boot sequence...");

	exponentialBehaviour.play();
}

// The loop function is called in an endless loop
void loop()
{

	Serial.printf("Looping... Value at %d\n", (int)exponentialBehaviour.value());
	exponentialBehaviour.loop();

	//SoftPWMSet(LED_BUILTIN, exponentialBehaviour.value());
	digitalWrite(LED_BUILTIN, HIGH);

	delay(10);
}
