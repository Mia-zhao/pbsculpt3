/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <XBee.h>

/*
This example is for Series 2 XBee and a Teensy on Sparkfun's Teensy XBee board
Receives a ZB RX packet and sets a PWM value based on packet data.
Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

int statusLed = LED_BUILTIN;
int errorLed = LED_BUILTIN;
int dataLed = LED_BUILTIN;

void flashLed(int pin, int times, int wait) {
    
    for (int i = 0; i < times; i++) {
      digitalWrite(pin, HIGH);
      delay(wait);
      digitalWrite(pin, LOW);
      
      if (i + 1 < times) {
        delay(wait);
      }
    }
}

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);
  pinMode(dataLed,  OUTPUT);
  
  Serial.begin(9600);

  delay(1000);

  Serial.println("Booting");
  
  // start serial
  Serial1.begin(9600);
  xbee.begin(Serial1);
  
  flashLed(statusLed, 3, 50);
}

// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
    
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      Serial.println("Got Something");
      
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        Serial.println("It's a ZB_RX_RESPONSE!");
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
            
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            Serial.println("ACKed");
            // the sender got an ACK
            flashLed(statusLed, 10, 10);
        } else if (rx.getOption() == ZB_BROADCAST_PACKET) {
            Serial.println("BCASTed");
            // we got it (obviously) but sender didn't get an ACK
            flashLed(errorLed, 2, 20);
        } else {
            Serial.println("NACKed");
            // we got it (obviously) but sender didn't get an ACK
            flashLed(errorLed, 2, 20);
        }

        Serial.printf("From (16-bit): %d\n", rx.getRemoteAddress16());

        Serial.printf("Data Length: %d\n", rx.getDataLength());
        
        // set dataLed PWM to value of the first byte in the data
        flashLed(dataLed, rx.getData(0), 100);
      } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
        Serial.print("It's a MODEM_STATUS_RESPONSE!");
        xbee.getResponse().getModemStatusResponse(msr);
        // the local XBee sends this response on certain events, like association/dissociation
        
        if (msr.getStatus() == ASSOCIATED) {
          // yay this is great.  flash led
          flashLed(statusLed, 10, 10);
        } else if (msr.getStatus() == DISASSOCIATED) {
          // this is awful.. flash led to show our discontent
          flashLed(errorLed, 10, 10);
        } else {
          // another status
          flashLed(statusLed, 5, 10);
        }
      } else if (xbee.getResponse().getApiId() == ZB_TX_REQUEST){ 
        Serial.print("It's a ZB_TX_REQUEST!");
        
      } else {
      	// not something we were expecting
        flashLed(errorLed, 1, 25);    
      }
    } else if (xbee.getResponse().isError()) {
      Serial.print("Error reading packet.  Error code: ");  
      Serial.println(xbee.getResponse().getErrorCode());
    }
}
