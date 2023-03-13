#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include <dht.h>
#define dataPin 12 // Defines pin number to which the sensor is connected D6 nodemcu
dht DHT; // Creats a DHT object
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hallo, FI KK I ");
  lcd.clear();
}
void loop() {
  int readData = DHT.read22(dataPin); // Reads the data from the sensor jika menggunakan dht 11 tinggal ganti angka 22 menjadi 11
  float t = DHT.temperature; // Gets the values of the temperature
  float h = DHT.humidity; // Gets the values of the humidity
  
  // Printing the results on the serial monitor
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("    Humidity = ");
  Serial.print(h);
  Serial.println(" % ");

 lcd.setCursor(0,0);
 lcd.print("Suhu      : ");
 lcd.setCursor(12,0);
 lcd.print(t);
 lcd.setCursor(18,0);
 lcd.print("*C");
 lcd.setCursor(0,1);
 lcd.print("Kelembaban: ");
 lcd.setCursor(12,1);
 lcd.print(h);
 lcd.setCursor(19,1);
 lcd.println("%");
 lcd.setCursor(0,2);
 lcd.print(" LIKE AND SUBSCRIBE");
 lcd.setCursor(2,3);
 lcd.print("--------->>>>SHARE");
  delay(2000); // Delays 2 secods, as the DHT22 sampling rate is 0.5Hz
}
