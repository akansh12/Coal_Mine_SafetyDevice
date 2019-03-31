#include <WiFi101.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <MQ2.h>


#define DHTPIN 2    
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);


char ssid[] = "MI";
char pass[] = "12345678";
int keyIndex = 0;
WiFiClient client;

//Coal mine Channel
unsigned long CoalMineChannelNumber = 746722;
const char * myWriteAPIKey = "919N98285GV1BJ34";

int SmokeSensor = A0;
int co;
int smoke;
int lpg;

MQ2 mq2(SmokeSensor);

void setup() {
Serial.begin(115200);
mq2.begin();
dht.begin();
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, pass); 
ThingSpeak.begin(client);
  
}


void loop() {
  //Smoke_Sensor
  float* values= mq2.read(true); 
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  

  //DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

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

  
  delay(1000);

}
