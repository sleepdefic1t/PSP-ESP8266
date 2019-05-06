
#include "psp_rng.h"

#include "lib/rfc6979/rfc6979.h"

#include <Arduino.h>

int Ark::Platform::RNG::Nonce(
    const uint8_t hash[],
    const uint8_t privateKey[],
    uint8_t nonce32[32]) {
  return nonce_function_rfc6979(&nonce32[0], &hash[0], &privateKey[0], nullptr, nullptr, 0);
}

/**/

#define RANDOM_REG32  ESP8266_DREG(0x20E44)

int Ark::Platform::RNG::RandomBytes(uint8_t *dest, unsigned size) {
  // Use the least-significant bits from the ADC for an unconnected pin (or connected to a source of 
  // random noise). This can take a long time to generate random data if the result of esp_random() 
  // doesn't change very frequently.
  // src: https://github.com/kmackay/micro-ecc/blob/master/examples/ecc_test/ecc_test.ino
  // True cryptographically-secure randomness is not guaranteed in this implementation.
  while (size) {
    uint8_t val = 0;
    for (unsigned i = 0; i < 8; ++i) {
      unsigned int init = RANDOM_REG32;
      int count = 0;
      while (RANDOM_REG32 == init) {
        ++count;
      };
      
      if (count == 0) {
        val = (val << 1) | (init & 0x01);
      } else {
        val = (val << 1) | (count & 0x01);
      };
    };
    *dest = val;
    ++dest;
    --size;
  };
  // NOTE: it would be a good idea to hash the resulting random data using SHA-256 or similar.
  return 1;
}
