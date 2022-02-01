#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x3F,16,2);
float data[2] = {24.54, 10.2};
String values[2] = {"Temp", "Humi"};
int counter = 0;
String ip = "127.0.0.1";
void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop(){
  delay(1000);
  counter++;
  if (counter == 5) {
    counter = 0;
  }
  if (counter/2 == 0 || counter/2 == 1) {
   printData(String(values[counter/2]), String(data[counter/2],2)); 
  }
}

void getData(){
  
}

void printData(String value, String message){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(value + " " + message);
  lcd.setCursor(0,0);
  lcd.print("IP: " + ip);
}
