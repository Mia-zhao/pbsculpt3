#include "Arduino.h"
#include "AdaptiveCapacitor.h"

#define TOUCH_PIN A2

data::AdaptiveCapacitor ac(100);
Metro ledMetro(1000);

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	delay(1000); // Wait for serial to start

	pinMode(LED_BUILTIN, OUTPUT);

	//DBGLN("ACA", "Starting...");
}

// The loop function is called in an endless loop
void loop()
{
	ac.loop();

	int r = touchRead(TOUCH_PIN);
	//DBGF("ACA","%d",r);

	ac.addReading(r);

	if(ac.isActivated()){
		Serial.println("Activation!");
		digitalWrite(LED_BUILTIN, HIGH);
		ledMetro.reset();
	}

	if( ledMetro.check() ){
		digitalWrite(LED_BUILTIN, LOW);
	}

	//delay(100);
}
