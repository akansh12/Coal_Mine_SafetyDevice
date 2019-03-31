#include "DHT.h"
#include <MQ2.h>
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;  
char pass[] = SECRET_PASS; 
int keyIndex = 0;            
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
#define DHTPIN D2    
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

int SmokeSensor = A0;
int co;
int smoke;
int lpg;
MQ2 mq2(SmokeSensor);


void setup() {
  Serial.begin(9600);  // Initialize serial
  mq2.begin();
  dht.begin();
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

//DHT SENSOR
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(false);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(hic);
  Serial.print(F("°C "));Serial.println();
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

//MQ2 sensor
  float* values= mq2.read(true); 
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  ThingSpeak.setField(5, co);
  ThingSpeak.setField(4, smoke);
  
 
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  
  
  delay(10000); // Wait 10 seconds to update the channel again
}
