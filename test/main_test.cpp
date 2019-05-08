
#include <Arduino.h>
#include <AUnit.h>

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

  setDummyTime();
};

void loop() {
  aunit::TestRunner::run();
};
