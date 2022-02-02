/* AUTHORS: itsy-pandacrp, ClG-DI21
 *
 * Station météo Groupe 5
 * 
 * Pinout:
 * LCD device:
 * GND => GND
 * VCC => 5V
 * SDA => 11 (SDA)
 * SCL => 12 (SCL)
 * 
 * DHT device:
 * SOURCE => 3
 * GND => GND
 * VCC => 5V
 * 
*/
#include <WiFiNINA.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11

/* WIFI */
char ssid[] = SECRET_SSID;
char pass[] = SECRET_SSID;
int status = WL_IDLE_STATUS;

int counter = 0;
const server = "https://api.groupe5.fr";
float temp[5] = {0,0,0,0,0};
float hum[5] = {0,0,0,0,0};

WiFiSSLClient client;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F,20,4);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  dht.begin();
  while (status != WL_CONNECTED) {
    lcd.print("Connecting to network");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected to network");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP: " + WiFi.localIP());
  Serial.begin(9600);
}


void loop() {
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
    client.connect(server+"/upload?temp="+String(avgTemp/5,1)+"&hum="+String(avgHum/5,1), 5010);
  }
  delay(1000);
}
