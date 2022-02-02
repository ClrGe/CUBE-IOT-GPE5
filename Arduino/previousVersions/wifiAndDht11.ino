/* Author : ClG - Date : 2022-02-01T09:32:23Z
Gestion de la connexion de l'arduino à internet et de la récupération des données du capteur DHT11 */


// librairies pour la connection wifi
#include <WiFiNINA.h>
// librairies pour le capteur DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// librairies pour le LCD 1602 I2C
#include <LiquidCrystal.h>

#define DHTPIN 3
#define DHTTYPE           DHT11     // DHT 11 

DHT_Unified dht(DHTPIN, DHTTYPE);
float hum;                                       
float temp;                                      
uint32_t delayMS;

// configuration de la connection  (défini dans arduino-secrets.h)
char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASSWD; 
int status = WL_IDLE_STATUS;/


void setup() {
  Serial.begin(9600);
  
  dht.begin();
// définition données de température et d'humidité
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Capteur :       "); Serial.println(sensor.name);
  Serial.print  ("Maximum :    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Minimum :    "); Serial.print(sensor.min_value); Serial.println(" *C");
  
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidité");
  Serial.print  ("Capteur :       "); Serial.println(sensor.name);
  Serial.print  ("Maximum :    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Minimum :    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.println("------------------------------------");
  delayMS = sensor.min_delay / 1000;
  

  Serial.println("------------------------------------");
  
  while (!Serial);

  while (status != WL_CONNECTED) {
    Serial.print("Tentative de connexion au réseau: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connecté  !");
  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");

  Serial.println ();
  Serial.println ();
  Serial.println ("+ ----------------------------- +");

  delay(50);
}

void loop() {

  delay(10000);
// afficher les infos réseau
  printNetworkData();
// récupérer les données du capteur
  getSensorData();

  delay(2000);

}

void printNetworkData() {

  Serial.println( "Informations réseau:");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.println();
}

void getSensorData() {
  delay(delayMS);
  
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) 
  {
    Serial.println("Erreur lecture temperature !");
  }
  else 
  {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.print(" *C    ");
  }
  
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
    Serial.println("Erreur lecture humidité !");
  }
  else 
  {
    Serial.print("Humidité: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
}
