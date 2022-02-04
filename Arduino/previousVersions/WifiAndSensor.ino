// v1 code C++ pour l'arduino mkr1010
// Fait fonctionner le wifi + le capteur dht11

// HttpClient - Version: Latest 
#include <HttpClient.h>
#include <b64.h>

#include <WiFiNINA.h>
#include "defines.h"
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SPI.h>

#define DHTPIN 3
#define DHTTYPE DHT11

float temp[5] = {0, 0, 0, 0, 0};
float hum[5] = {0, 0, 0, 0, 0};
uint32_t delayMS;

// SSID ET PASSWORD À METTRE DANS ONGLET SECRETS (VOIR DOC WIFIINA)
char ssid[] =  SECRET_SSID;
char pass[] =  SECRET_PASS;
int status = WL_IDLE_STATUS;

int counter = 0;
IPAddress server(192,168,95,217);

WiFiSSLClient client;
LiquidCrystal_I2C lcd(0x3F,20,4);
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  dht.begin();

  while (status != WL_CONNECTED) {
    Serial.print("Tentative de connexion au réseau: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connecté au réseau !");
  Serial.println("----------------------------------------");

  while (!Serial);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected to network");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP: " + WiFi.localIP());

  printData();

  delay(50);
}

void loop() {
  getSensorData();
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println(F("\nDisconnecting from server."));
    client.stop();

    // do nothing forevermore:
    while (true);
  }
  delay(2000);

}

void printData() {

  Serial.println("Board Information:");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP : ");
  Serial.println(ip);
  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void getSensorData() {
    temp[4] = dht.readTemperature();
  hum[4] = dht.readHumidity();
  lcd.setCursor(0,1);
  if ((counter/2) % 2){
    lcd.setCursor(0,1);
    lcd.print("TEMP: " + String(temp[4],1) + (char)223 + "C");
  } else {
    lcd.print("HUM : "+ String(hum[4],1) + "% ");
  }
  for (int i=1;i<5;i++){
    temp[i-1]=temp[i];
    hum[i-1] = hum[i];
  }
  counter++;
  if (counter==4){
    counter = 0;
    float avgTemp = 0;
    float avgHum  = 0;
    for (int i=0; i<5;i++){
      avgTemp += temp[i];
      avgHum += hum[i];
    }
    Serial.println("Hum= " + String(avgHum/5,1));
    Serial.println("Tem= " + String(avgTemp/5,1));
    Serial.println(server+"/upload?temp="+String(avgTemp/5,1)+"&hum="+String(avgHum/5,1));
    
    //client.connect(server+"/upload?temp="+String(avgTemp/5,1)+"&hum="+String(avgHum/5,1), 5010);
  }
}
