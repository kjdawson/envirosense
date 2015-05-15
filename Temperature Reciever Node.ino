#include "DHT.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Code for temperature transmitter node
//

// Set up nRF24L01 radio on pins 9, 10 

RF24 radio(9,10);

//
// Infrastructure Setup
//

// Radio pipe addresses for nodes to communicate
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

#define DHTPIN 2     // pin used for sensor

// Define the specific sensor being used
#define DHTTYPE DHT11   // DHT 11 
//define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(57600);
  Serial.println("DHT11 temperature sensor!");
  
  printf_begin();
  dht.begin();
  radio.begin();
  
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(8);
  
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.startListening();
  radio.printDetails();
}

void loop() {
  
  unsigned long h = dht.readHumidity();
  // Read temperature as Celsius
  unsigned long t = dht.readTemperature();
  // Read temperature as Fahrenheit
  unsigned long f = dht.readTemperature(true);
  
  // Check if reads failed 
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Stop listening to send data
  radio.stopListening();

  unsigned long time = millis();
  printf("Temperature is ",t," at time",time);
  bool ok = radio.write(&t, sizeof(unsigned long) );
  
  if (ok)
    printf("SUCCESSFUL");
  else
    printf("FAILED\n\r");

  // Start listening for new data
  radio.startListening();

  unsigned long started_waiting_at = millis();
  bool timeout = false;
  while ( ! radio.available() && ! timeout )
    if (millis() - started_waiting_at > 200 )
      timeout = true;

  // Describe the results
  if ( timeout )
  {
    printf("FAILED, response timed out.\n\r");
  }
  else
  {
    // Get the response
    unsigned long got_time;
    unsigned long temp;
    radio.read( &temp, sizeof(unsigned long));

    // Show how long the response took to return
    printf("Got response %lu, round-trip delay: %lu\n\r",temp,millis()-got_time);
  }

  // wait before measuring again
  delay(2000);
}
