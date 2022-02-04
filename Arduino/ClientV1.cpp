/*
    AUTHOR: itsy-pandacrp & ClG-DI21
    
    HARDWARE: Arduino MKR WiFi 1010
    
    WIRING:
      DHT_DATA => 7
      
      LCD_SCL => 12
      LCD_SDA => 11
*/

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <DHT.h>
#include <SPI.h>

#include "config.h"

#define DHT_DATA 7
#define DHTTYPE DHT11

#define SENSORID CONFIG_SENSOR

#define lcd_Adress 0x3F
#define lcd_column 16
#define lcd_row 2

char ssid[] = CONFIG_SSID;
char pass[] = CONFIG_PASS;

int counter = 0;
float temp[5] = {0,0,0,0,0};
float hum[5] = {0,0,0,0,0};


const char serverAddress[] = CONFIG_SERVER;
int port = CONFIG_PORT;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
DHT dht(DHT_DATA, DHTTYPE);
LiquidCrystal_I2C lcd(lcd_Adress,lcd_column,lcd_row);

int status = WL_IDLE_STATUS;

void sendData(String postData){
  String path = "/api/v1/data";
  String contentType = "application/json";
  client.post(path, contentType, postData);
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

void lcdPrint(int col, int row, String message){
  lcd.setCursor(col, row);
  lcd.print(String(message));
  for (int i = 0;String(message).length()+i<16;i++){
    lcd.print(" ");
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcdPrint(0,0,"LCD initialized");
  lcdPrint(0,1,"DHT connecting");
  dht.begin();
  lcdPrint(0,1,"DHT connected");

  delay(1000);
  lcd.clear();
  int TRY = 0;
  while ( status != WL_CONNECTED) {
    TRY++;
    lcdPrint(0,0,"Connecting to");
    lcdPrint(0,1,String(ssid));

    for (int i=0;i<=5;i++){
      lcdPrint(15,1,String(5-i));
      status = WiFi.begin(ssid, pass);
      if (status == WL_CONNECTED) {
        break;
      }
    }
    delay(1000);
  }
  
  lcdPrint(0,0,"Connected");
  delay(1000);
  
  lcd.clear();
  IPAddress ip = WiFi.localIP();
  lcd.setCursor(0,0);
  lcd.print(ip);
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
    sendData("{\"sensorID\":"+String(SENSORID)+",\"temp\":"+String(avgTemp/5,1)+",\"humidity\":"+String(avgHum/5,1)+"}");
    Serial.println("{sensorID:"+String(SENSORID)+",temp:"+String(avgTemp/5,1)+",humidity:"+String(avgHum/5,1)+"}");
  }
  delay(1000);
}
