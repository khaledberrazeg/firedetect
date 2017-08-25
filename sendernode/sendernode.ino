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

int IR_PIN = 5; 

int led = 4; 

 
int IR_PIN_CFG = 2; // configIR
const int gasPin = A0; 
IRrecv recepteur(IR_PIN);
IRrecv recepteur_config(IR_PIN_CFG);
decode_results results,results_tmp;
RF24 radio(7,8);                   

RF24Network network(radio);        


const uint16_t this_node = 01;        
const uint16_t other_node = 00;      

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload

   //String payloadstring;
   int messageInt;
  
};
int message=1;
 int minimum_ppm=150;
 int gas_value=0;
   bool conf_mode=false;
void setup(void)
{
  Serial.begin(57600);

  Serial.println("RF24Network Bda yconecti");
 pinMode(led, OUTPUT);
 delay(1000);
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
 recepteur.enableIRIn();
   recepteur_config.enableIRIn();
   
}

void loop() {

  delay(100); // bilach
   


      results=results_tmp;
  network.update();                          // Check the network regularly

  

   // Serial.print("Sending...");
    payload_t payload = { message };
  
    RF24NetworkHeader header(/*to node*/ other_node);
   
     //   Serial.println();
    if (recepteur.decode(&results))
    {
        delay(200);
        Serial.println(results.value, HEX);
     message=1;
     payload = { message };
      bool ok = network.write(header,&payload,sizeof(payload));
    
     if (ok)
     { while (true)
      {
         Serial.println("Zifet infrarouge bel 1");
        network.write(header,&payload,sizeof(payload));
        Serial.println(message);
   
      }
     }
      recepteur.resume();
    }


    
    recepteur.resume();
    gas_value=analogRead(gasPin);
   Serial.println(gas_value);
     if ( gas_value  >=minimum_ppm)
     {
    message=analogRead(gasPin);
     payload = { message };
      bool ok = network.write(header,&payload,sizeof(payload));
  
       if (ok)
      while (true)
      {
           Serial.println("Zifet el gaz bel la valeur du ppm");
        network.write(header,&payload,sizeof(payload));
        Serial.println(message);
      }
     }
          message=-1;
     payload = { message };
    bool ok = network.write(header,&payload,sizeof(payload));
    
  
}
