
#include "psp_net.h"

#include <ESP8266HTTPClient.h>

#include <cstdio>
#include <cstring>
#include <string>

std::string Ark::Platform::Network::HTTP::Get(
    const char* request,
    int apiVersion) {
  HTTPClient httpClient;
  httpClient.setReuse(true);
  httpClient.setTimeout(3000);

  char version[sizeof(apiVersion)];
  snprintf(version, sizeof(apiVersion), "%d", apiVersion);
  httpClient.addHeader("API-Version", version);
  httpClient.addHeader("Content-Type", "application/json");

  httpClient.begin(request);

  delay(1000);

  httpClient.GET();

  return httpClient.getString().c_str();
}

/**/

std::string Ark::Platform::Network::HTTP::Post(
    const char* request,
    const char* body) {
  HTTPClient httpClient;
  httpClient.setReuse(true);
  httpClient.setTimeout(3000);

  (body[0] == '{') // set the header content-type
      ? httpClient.addHeader("Content-Type", "application/json")
      : httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");

  httpClient.begin(request);

  delay(1000);

  httpClient.POST(body);
  
  return httpClient.getString().c_str();
}