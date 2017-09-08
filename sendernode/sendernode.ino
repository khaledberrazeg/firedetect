
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <IRremote.h>

int IR_PIN = 5; 

int led = 4; 

const int gasPin = A0; 
IRrecv recepteur(IR_PIN);

decode_results results,results_tmp;
RF24 radio(7,8);                   

RF24Network network(radio);        

bool loadBoot=true;
const uint16_t this_node = 01;        
const uint16_t other_node = 00;     
int gasValues[]={0,0,0,0,0}; 

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


struct payload_t {                  // Structure of our payload

   //String payloadstring;
   int messageInt;
  
};
int message=1;
 int minimum_ppm=0;
 int gas_value=0;
   bool conf_mode=false;
void setup(void)
{
  Serial.begin(57600);

  Serial.println("Handshake of RF protocol. The communication is begining.");
 pinMode(led, OUTPUT);
 delay(1000);
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
 recepteur.enableIRIn();
   
   
}

void loop() {
if (loadBoot)
{
   digitalWrite(led, HIGH);
  for (int i=0;i<5;i++)
  {
    
    delay(1000);
    gas_value=analogRead(gasPin);
    
    gasValues[i]=gas_value;
    Serial.print("Reading gas value:");
    Serial.println(gas_value);
  }
  minimum_ppm=gasValues[0];
  for (int i=0;i<4;i++)
  {
    if (minimum_ppm>0 && gasValues[i+1]-gasValues[i]<minimum_ppm)
    minimum_ppm=gasValues[i];
  }
  loadBoot=false;
  digitalWrite(led, LOW);
}
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
          Serial.println("Reading the hex value of the IR Received value");
        Serial.println(results.value, HEX);
     message=1;
     payload = { message };
      bool ok = network.write(header,&payload,sizeof(payload));
    
     if (ok)
     { while (true)
      {
         Serial.println("Sending the alert:");
        network.write(header,&payload,sizeof(payload));
        Serial.println(message);
   
      }
     }
      recepteur.resume();
    }


    
    recepteur.resume();
    gas_value=analogRead(gasPin);
     Serial.print("Loop the gas value picked contiously. Value: ");
   Serial.println(gas_value);
     if ( gas_value>=minimum_ppm)
     {
    message=analogRead(gasPin);
     payload = { message };
      bool ok = network.write(header,&payload,sizeof(payload));
  
       if (ok)
       {
         Serial.println("Sending the alert via RF with the value of PPM on the payload of the message");
      while (true)
      {
           Serial.println("Sending PPM Value: ");
        network.write(header,&payload,sizeof(payload));
        Serial.println(message);
      }
     }
     }
          message=-1;
     payload = { message };
    bool ok = network.write(header,&payload,sizeof(payload));
    
  
}
