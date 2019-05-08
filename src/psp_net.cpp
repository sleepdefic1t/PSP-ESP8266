
#include "psp_net.h"

#include <ESP8266HTTPClient.h>

#include <cstdio>
#include <cstring>
#include <string>

int tryConnection(HTTPClient &httpClient, const char* const request) {
  if (!httpClient.begin(request)) {
    // Bad HTTP begin
    return -1;
  };
  int code = httpClient.GET();
  int count = 0;
  while (code != HTTP_CODE_OK) {
    // error
    httpClient.end();
    if (count >=2) {
      // Bad connection. Try another peer
      return code;
    };
    // Bad HTTP GET.\nRetrying connection..
    delay(1000);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("API-Version", "2");
    httpClient.begin(request);
    code = httpClient.GET();
    count++;
  };
  return code;
}

/**/

// Arduino's HTTPClient requires that a single-line HTTP request string begins with 'http://'.
// This is only a consideration on IoT platforms.
inline std::string toHttpStr(const char* const request) {
  // char[7 (size of char string "http://") + 'request' string-length + 1 (for the null terminator '\0')]
  char httpRequest[7 + std::strlen(request) + 1];
  snprintf(httpRequest, sizeof(httpRequest), "http://%s%c", request, '\0');
  return httpRequest;
}

/**/

std::string Ark::Platform::Network::HTTP::Get(const char* request) {
  #ifndef UNIT_TEST
    HTTPClient httpClient;
    httpClient.setReuse(false);
    httpClient.setTimeout(3000);
    if (int code = tryConnection(httpClient, toHttpStr(request).c_str()) != 200) {
      // error
      return httpClient.errorToString(-code).c_str(); // <- note `-` symbol.
    };
    return httpClient.getString().c_str();
  #else
    return "{\"statusCode\":404,\"error\":\"Not Found\",\"message\":\"Not Found\"}";
  #endif
}

/**/

std::string Ark::Platform::Network::HTTP::Post(
    const char* request,
    const char* body) {
  #ifndef UNIT_TEST
    HTTPClient httpClient;
    httpClient.setReuse(true);
    httpClient.setTimeout(3000);
    httpClient.begin(toHttpStr(request).c_str());
    (body[0] == '{') // set the header content-type
        ? httpClient.addHeader("Content-Type", "application/json")
        : httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpClient.POST(body);
  return httpClient.getString().c_str();
  #else
    return "{\"statusCode\":422,\"error\":\"Unprocessable Entity\",\"message\":\"should NOT have fewer than 1 items\"}";
  #endif
}
