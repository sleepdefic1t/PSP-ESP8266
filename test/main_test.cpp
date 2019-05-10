
#include <AUnit.h>

#include <Arduino.h>

#include <cstring>

/**
 * Connecting to WiFi is optional for testing.
 * 
 **/
#include <ESP8266WiFi.h>

/* This is the WiFi network you'd like your board to connect to. */
const char* ssid = "yourSSID";
const char* password = "yourWiFiPassword";
/**/

// Set Dummy Time for testing board
#include <sys/time.h>
void setDummyTime() {
  // set board time to: 21 March 2019(in seconds)
  // 2 years after Mainnet launch.
  struct timeval tv;
  tv.tv_sec = 1553173200ull; 
  settimeofday(&tv, NULL);
};

void setup() {
  Serial.begin(115200);
	while (!Serial); // for the Arduino Leonardo/Micro only

  // if ssid and passwd set
  if (strcmp(ssid, "yourSSID") != 0 && strcmp(password, "yourWiFiPassword") != 0) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    };
    Serial.println();
    Serial.println(WiFi.localIP());
  };

  setDummyTime();
};

void loop() {
  aunit::TestRunner::run();
};
