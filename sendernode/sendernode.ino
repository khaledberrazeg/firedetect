/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <IRremote.h>

int IR_PIN = 5; // the pin of the IR receiver
IRrecv recepteur(IR_PIN);
decode_results results;
RF24 radio(7,8);                   

RF24Network network(radio);        


const uint16_t this_node = 01;        
const uint16_t other_node = 00;      

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload
 // unsigned long ms;
  //unsigned long counter;
   //String payloadstring;
   int messageInt;
};
int message=1;
void setup(void)
{
  Serial.begin(57600);
//  Serial.begin(2000000);
  Serial.println("RF24Network Bda yconecti");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  recepteur.enableIRIn();
}

void loop() {
  
  network.update();                          // Check the network regularly

  
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

   // Serial.print("Sending...");
    payload_t payload = { message };
       //payload_t payload = { millis(), packets_sent++ };
    RF24NetworkHeader header(/*to node*/ other_node);
    delay(200);
    if (recepteur.decode(&results))
    {
        Serial.println("Zifet");
     //Serial.println(results.value, HEX);
     message=1;
     payload = { message };
      bool ok = network.write(header,&payload,sizeof(payload));
     Serial.println("Zifet");
    }
     recepteur.resume();
          message=-1;
     payload = { message };
    bool ok = network.write(header,&payload,sizeof(payload));
    //if (ok)
      //Serial.println("ok.");
    //else
      //Serial.println("failed.");
  }
}
