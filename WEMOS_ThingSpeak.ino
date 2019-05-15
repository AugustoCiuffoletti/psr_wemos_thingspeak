#include "secret.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;

int n=0;

int joinAP(int retry){
  Serial.print("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(MYSSID, MYPASSWD);
  while ( ( WiFi.status() != WL_CONNECTED ) && ((retry--) > 0) )
  {
    delay(500);
    Serial.print(".");
  }
  if ( WiFi.status() != WL_CONNECTED ) {
    Serial.println("Can't connect to AP");
    return 1;
  }
  Serial.println();
  Serial.print("Connected as ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN,LOW);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(57600);
  WiFi.mode(WIFI_STA);
  while ( ! joinAP(200) ) {
    Serial.println("Can't connect..., retrying");
  };
}

void loop() {
  WiFiClient client;
  HTTPClient http;
  char payload[100]; // Buffer HTTP
  digitalWrite(LED_BUILTIN,LOW);
  sprintf(payload, "api_key=%s&field1=%d", CHANNEL_KEY,n++);
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, "http://api.thingspeak.com/update")) {
    http.addHeader("Host","api.thingspeak.com");
    int x=http.POST(payload);
    if(x == 200) {
      Serial.println("Channel update successful.");
      http.end();
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }
  digitalWrite(LED_BUILTIN,HIGH);
  delay(60000);
}
