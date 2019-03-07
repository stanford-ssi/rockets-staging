/*********
  Rui Santos
  edited by Tylor Jilk
  Blinks LED on and off with OTA upload capability
  Note that a hotspot must be turned on with ssid and password of that given below.
  ArduinoOTA github: https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
*********/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Replace with your network credentials
const char* ssid = "Staging2K19";
const char* password = "sEWnon6YhY!y7Ul#wwd&";
const int led_pin = 0; // pin 0 connected to LED on h.boson 1.0

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  digitalWrite(led_pin, LOW);
  delay(1000);
  digitalWrite(led_pin, HIGH);
  delay(1000);
}

/*
 * Will want to use the commands
 *    void onStart(OTA_CALLBACK(fn));
 *    void onEnd(OTA_CALLBACK(fn));
 *    void onProgress(OTA_CALLBACK_PROGRESS(fn));
 *    void onError(OTA_CALLBACK_ERROR (fn));
 * to control what the esp does on startup / shutdown so it doesn't do something dumb.
 */
