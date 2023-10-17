#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>


DHT dht(D4,DHT11);
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup() {
  Serial.begin (115200);
  dht.begin();
  lcd.begin();
  lcd.display();        
  lcd.backlight();      
  lcd.clear();          
}

void loop() {
  float h = dht.readHumidity();             
  float t = dht.readTemperature();          
  lcd.setCursor(0, 0);
  lcd.print("Humi = "+String(h,1)+" %");   
  lcd.setCursor(0, 1);
  lcd.print("Temp = "+String(t,1)+" C");      
  delay(1000);
  lcd.clear();
}