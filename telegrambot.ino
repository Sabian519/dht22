#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN D1
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Initialize Wifi connection to the router
char ssid[] = "hotspot";     // diisi nama wifi
char password[] = "newbie12"; // diisi password wifi

// Initialize Telegram BOT
#define BOTtoken "6020879752:AAFDEbkLvA703-hayclpC5aPZozGCyiz6hI" // diisi Token Bot (Dapat dari Telegram Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
 
    //Cek Pembacaan Sensor DHT11
    if (text == "/statussuhu") {
      int t = dht.readTemperature()-2;
       String temp = "Suhu saat ini : ";
       temp += int(t);
       temp +=" *C\n";
      
      bot.sendMessage(chat_id,temp, "");
    }
    if (text == "/statuskelembapan") {
      int h = dht.readHumidity();
       String temp = "Kelembaban: ";
       temp += int(h);
       temp += " %";
     
      bot.sendMessage(chat_id,temp, "");
    }
    
    //Cek Command untuk setiap aksi
    if (text == "/start") {
      String welcome = "Welcome  " + from_name + ".\n";
      welcome += "/statussuhu : Status Suhu\n";
      welcome += "/statuskelembapan : Status Kelembapan\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void setup() {

  Serial.begin(115200);
  dht.begin();
  client.setInsecure();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.begin(115200);
  lcd.begin(0, 2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hallo, Moch Gilbert! ");
  lcd.clear();

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int t = dht.readTemperature()-2;
  int h = dht.readHumidity();
  
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
 delay(2000); // Delays 2 secods, as the DHT22 sampling rate is 0.5Hz
  
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}