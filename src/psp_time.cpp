
#include "psp_time.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <time.h>
#include <stdlib.h> /* strtol */
#include <string>

int Ark::Platform::Time::Configure(
    const int timezone,
    const int dstOffsetSeconds,
    const char* server) {

  // if WiFi is connected, configure Time.
  if (WiFi.status() == 3) {
    configTime(timezone, dstOffsetSeconds, server);
  } else { return -1; }; // system not connected

  const unsigned long currentSystemTime = time(0);
  // if time > 28 Jan 2019(in seconds) (2 years after Ark Epoch)
  // and time value < near uint64_t max container size (guards potential overflow)
  return (currentSystemTime > 1548725761ull && currentSystemTime < 1e20);
}

/***/

uint64_t Ark::Platform::Time::Epoch(const char* networkEpochStr) {
  constexpr const size_t expectedLength = sizeof("2017-03-21T13:00:00.000Z") - 1;
  if (expectedLength != 24) { return 0; } //  Unexpected ISO 8601 date/time length

  std::string input(networkEpochStr);

  if (input.length() < expectedLength) {
    return 0;
  };

  struct tm time;
  time.tm_year = strtol(&input[0], nullptr, 10) - 1900;
  time.tm_mon = strtol(&input[5], nullptr, 10) - 1;
  time.tm_mday = strtol(&input[8], nullptr, 10);
  time.tm_hour = strtol(&input[11], nullptr, 10);
  time.tm_min = strtol(&input[14], nullptr, 10);
  time.tm_sec = strtol(&input[17], nullptr, 10);
  time.tm_isdst = 0;

  return mktime(&time);
}

/***/

uint64_t Ark::Platform::Time::Now() {
  return time(0);
}
