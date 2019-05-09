
#include <AUnit.h>

#include "psp_net.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>

// ArduinoJson Presets
#define ARDUINOJSON_USE_LONG_LONG 1
#define ARDUINOJSON_ENABLE_STD_STRING 1      // Enable 'std::string'
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 0  // disable 'String'
#include "ArduinoJson.h"


test(network, http_get) {
  const auto request = "http://167.114.29.45:4003/api/v2";

  // Get the 'request' response using HTTP
  const auto response = Ark::Platform::Network::HTTP::Get(request).c_str();

  if (WiFi.status() != WL_CONNECTED) {
    assertEqual(response, "connection refused");
  } else {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, response);
    assertEqual(404, doc["statusCode"]);
  };
}

test(network, http_post) {
  const auto request = "http://167.114.29.45:4003/api/v2/transactions";
  const auto txJson = "{\"transactions\":[]}";

  // Post the 'request' and 'txJson' for a response using HTTP
  const auto response = Ark::Platform::Network::HTTP::Post(request, txJson).c_str();

  if (WiFi.status() != WL_CONNECTED) {
    assertEqual(response, "");
  } else {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, response);
    assertEqual(422, doc["statusCode"]);
  };
}
