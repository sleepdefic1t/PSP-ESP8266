
#include "psp_time.h"

#include <time.h>
#include <stdlib.h> /* strtol */
#include <string>

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
