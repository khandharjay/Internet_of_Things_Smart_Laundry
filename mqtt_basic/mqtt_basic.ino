/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
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
byte ip[]     = { 192, 168, 0, 2 };

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
    client.publish("public/laundarypal","hello world-from Jay");
    //client.subscribe("inTopic");
  }
}

void loop()
{
  int i=0;
  client.loop();
  for(i=0;i<10;i++)
  {
    client.publish("public/laundarypal","hello world-from Jay");
  }
}

