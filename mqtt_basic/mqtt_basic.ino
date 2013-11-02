/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes sensor data to the topic "outTopic"
  
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
//byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//byte server[] = { 172, 16, 0, 2 };
//byte ip[]     = { 172, 16, 0, 100 };

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 10, 217, 2, 1 };
//byte ip[]     = { 192, 168, 0, 2 };

// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 15;  // threshold value to decide when the detected sound is a knock or not-Original 100
int sensorReading = 0;      // variable to store the value read from the sensor pin

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client("q.m2m.io", 1883, callback, ethClient);

void setup()
{
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // this check is only needed on the Leonardo:
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
   }
  //Ethernet.begin(mac, ip);  For static IP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  if (client.connect("arduinoClient")) {
    //client.publish("public/laundarypal","hello world-from Jay");
    client.publish("public/laundarypal","\"info\":\"bootup\"");
    //client.subscribe("inTopic"); 
  }
  
  
}

void loop()
{
  int count=0,loop_count=0;
  client.loop();
  
  
 sensorReading = analogRead(knockSensor);   
 
 //Keep looping till sensor value is lower than the threshold 
 
 while(sensorReading < threshold)
 {
    sensorReading = analogRead(knockSensor);   
    //Serial.println(sensorReading,DEC);
 }
 
 //Vibration started, send Start message
 client.publish("public/laundarypal","\"started\":true");
 Serial.println("Started");
 //Keep looping till sensor value is lower than the threshold 
 do
 {
   count=0;
   for(loop_count=20 ; loop_count>0; loop_count--)
   {
    sensorReading = analogRead(knockSensor);
    if(sensorReading >= threshold)
     {
       count++;
       //Serial.println(sensorReading,DEC);
     }   
    delay(100);
   }
 }while(count>=1);
 
//Verify if vibration has stopped
 do
 {
   count=0;
   for(loop_count=20; loop_count>0; loop_count--)
   {
     sensorReading = analogRead(knockSensor);   
     if(sensorReading == 0)
      {
        count++;
        //Serial.println(sensorReading,DEC);
      }
     delay(100);
   }
 }while(count != 20);
 
 //Vibration Stopped-Send Update
 client.publish("public/laundarypal","\"started\":false");
 Serial.println("Stopped"); 
}

