// Time - Version: Latest
#include <Wire.h> 
#include <TimeLib.h>
#include <Adafruit_INA219.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_Si7021.h>
#include <WiFiNINA_Generic.h>
#include <LiquidCrystal.h>

float SI7021_hum;                                        // relative humidity Si7021
float SI7021_temp;                                       // temperature Si7021

Adafruit_Si7021 sensor = Adafruit_Si7021();

char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;// the Wifi radio's status


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");

  Serial.println ();
  Serial.println ();
  Serial.println ("+ ----------------------------- +");


  if (!sensor.begin ())
  {
    Serial.println ("Did not find Si7021 sensor!");
    while (true);
  }
  delay(50);
}

void loop() {
  // check the network connection once every 10 seconds:

  delay(10000);

  printData();

  Serial.println("----------------------------------------");
  double T;
  SI7021_hum = sensor.readHumidity();                      // read Si7021 data
  SI7021_temp = sensor.readTemperature();

  Serial.print ("relative humidity:   ");
  Serial.print (SI7021_hum, 0);
  Serial.println (" %");

  Serial.print ("Si7021 temperature:  ");
  Serial.print (SI7021_temp, 1);
  Serial.println (" *C");

  Serial.println ();
  delay (5000);

}

void printData() {

  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();


}
