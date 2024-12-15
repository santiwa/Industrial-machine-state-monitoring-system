/*
Name: Tiwari Ramesh Sanjana
Date: 19-3-2024
Title: Industrial Machine State Monitoring System
Description: Arduino IDE combined with the Ethernet module acts as the SERVER, and ThingSpeak is considered the CLIENT.
The analog values from Arduino board is shared to ThingSpeak Cloud. Following sensors are used as an example:
LM35 temperature at A0, LDR at A1, 2 Potentiometers at A2 and A3, respectively.
Changes in the sensor readings can be observed on Serial monitor and ThingSpeak  cloud.
Data is updated in the interval of 10 seconds.
*/

#include <Ethernet.h>
#include "secrets.h"
#include "ThingSpeak.h" // Always include thingspeak header file after other header files and custom macros

byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 170);
IPAddress myDns(192, 168, 0, 104);

EthernetClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Input Sensors Configuration
const int lm35Pin = A0;  
const int ldrPin = A1;
const int potPin1 = A2;
const int potPin2 = A3;

void setup() {
  // Initialize input pins;
  pinMode(lm35Pin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(potPin1, INPUT);
  pinMode(potPin2, INPUT);

  Ethernet.init(10);  // Most Arduino Ethernet hardware
  Serial.begin(9600);  // Initialize serial communication
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for Leonardo native USB port only
  }
      
  // Start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware presence
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // Do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // Try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // Give the Ethernet shield a second to initialize:
  delay(1000);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Set the fields with the values
  ThingSpeak.setField(1, A0);
  ThingSpeak.setField(2, A1);
  ThingSpeak.setField(3, A2);
  ThingSpeak.setField(4, A3);
  
  // Read analog input from thingspeak  
  String temperatureReading = String(analogRead(A0));
  delay(500);
  String lightIntensityReading = String(analogRead(A1));
  delay(500);
  String potentiometer1Reading = String(analogRead(A2));
  delay(500);
  String potentiometer2Reading = String(analogRead(A3));
  delay(500);

  // Write to ThingSpeak
  // LM35 Temperature Sensor reading
  int a = ThingSpeak.writeField(myChannelNumber, 1, temperatureReading, myWriteAPIKey);// Or use ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey) to write all fields at once!
  if(a == 200){
    Serial.println("Channel updated: Field1 data update successful.");
    Serial.print("LM35 Temperature Sensor value: ");
    Serial.println(temperatureReading);
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(a));
  }
  delay(10000); // Wait 10 seconds to update the channel again

  // LDR Sensor reading
  int b = ThingSpeak.writeField(myChannelNumber, 2, lightIntensityReading, myWriteAPIKey);
  if(b == 200){
    Serial.println("Channel Updated: Field2 data update successful.");
    Serial.print("LDR Sensor value: ");
    Serial.println(lightIntensityReading);
  }
  else{
    Serial.println("Problem updating channel, Filed2. HTTP error code " + String(b));
  }
  delay(10000); // Wait 10 seconds to update the channel again

  // Potentiometer1 reading
  int c = ThingSpeak.writeField(myChannelNumber, 3, potentiometer1Reading, myWriteAPIKey);
  if(c == 200){
    Serial.println("Channel Updated: Field3 data update successful.");
    Serial.print("Potentiometer1 value: ");
    Serial.println(potentiometer1Reading);
  }
  else{
    Serial.println("Problem updating channel, Feild3. HTTP error code " + String(c));
  }
  delay(10000); // Wait 10 seconds to update the channel again

  // Potentiometer2 reading
  int d = ThingSpeak.writeField(myChannelNumber, 4, potentiometer2Reading, myWriteAPIKey);
  if(d == 200){
    Serial.println("Channel Updated: Field4 data update successful.");
    Serial.print("Potentiometer2 value: ");
    Serial.println(potentiometer2Reading);
  }
  else{
    Serial.println("Problem updating channel, Field4. HTTP error code " + String(d));
  }
  delay(10000); // Wait 10 seconds to update the channel again
}