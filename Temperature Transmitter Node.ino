#include "DHT.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Code for temperature reciever node
//

// Set up nRF24L01 radio on pin 9, 10

RF24 radio(9,10);

//
// Infrastructure Setup
//

// Radio pipe addresses for nodes to communicate
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

#define DHTPIN 2     // sensor pin used

// define the type of sensor being used 
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(57600);
  Serial.println("DHT11 temperature sensor!");
  
  printf_begin();
  dht.begin();
  radio.begin();
  
  radio.setRetries(15,15);

  // reducing the payload size to improve
  // reliability
  radio.setPayloadSize(8);
  
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);

  radio.startListening();
  radio.printDetails();
}

void loop() {
// if there is data
    if ( radio.available() )
    {
      unsigned long temp;
      bool done = false;
      while (!done)
      {
        // Fetch the data
        done = radio.read( &temp, sizeof(unsigned long) );

        // Show the temperature in the console
        printf("The Temperature is %lu...",temp);

        delay(20);
      }

      // Stop listening to reply
      radio.stopListening();

      // Send a reply back
      radio.write( &temp, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Start listening for new data
      radio.startListening();
    }
  }
