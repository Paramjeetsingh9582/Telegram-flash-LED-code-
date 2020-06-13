/*******************************************************************
* Telegram app based home automation using NodeMCU  
 *******************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "Param";     // your network SSID (name)
char password[] = "9582@paramsingh"; // your network key

// Initialize Telegram BOT
#define BOTtoken "1266830632:AAERyTwRnSXRZRkE48zXi6OgOyhT_M8cVwg"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
//client.setInsecure();

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

const int led1Pin = 12;
const int led2Pin = 13;
int led1Status = 0;
int led2Status = 0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/Device1on") {
      digitalWrite(led1Pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      led1Status = 1;
      bot.sendMessage(chat_id, "Device 1 is ON", "");
    }

    if (text == "/Device1off") {
      led1Status = 0;
      digitalWrite(led1Pin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Device 1 is OFF", "");
    }

    if (text == "/Device2on") {
      digitalWrite(led2Pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      led2Status = 1;
      bot.sendMessage(chat_id, "Device 2 is ON", "");
    }

    if (text == "/Device2off") {
      led2Status = 0;
      digitalWrite(led2Pin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Device 2 is OFF", "");
    }

    if (text == "/status") 
    {
      if(led1Status)
      {
        bot.sendMessage(chat_id, "Device 1 is ON", "");
      } 
      else 
      {
        bot.sendMessage(chat_id, "Device 1 is OFF", "");
      }
      if(led2Status)
      {
        bot.sendMessage(chat_id, "Device 2 is ON", "");
      } 
      else 
      {
        bot.sendMessage(chat_id, "Device 2 is OFF", "");
      }
    }

    if (text == "/start") {
      String welcome = "Welcome to Universal Arduino Telegram Bot, " + from_name + ".\n";
      welcome += "/Device1on : To switch the Device 1 ON\n";
      welcome += "/Device1off : To switch the Device 1 OFF\n";
      welcome += "/Device2on : To switch the Device 2 ON\n";
      welcome += "/Device2off : To switch the Device 2 OFF\n";
      welcome += "/status : Gives you current status of Devices\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
void setup() {
  client.setInsecure();
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
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

  pinMode(led1Pin, OUTPUT); // initialize digital ledPin as an output.
  pinMode(led2Pin, OUTPUT);
  delay(10);
  digitalWrite(led1Pin, LOW); // initialize pin as off
  digitalWrite(led2Pin, LOW);
 
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
