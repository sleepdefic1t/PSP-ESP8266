
#include <AUnit.h>

#include "psp_time.h"

test(time_epoch) {
    // test parsing of epoch string.
    // "2017-03-21T13:00:00.000Z" should be '1490101200' in seconds
    assertEqual(1490101200ull, Ark::Platform::Time::Epoch("2017-03-21T13:00:00.000Z"));
}

test(time_now) {
    // if system time configures and updates successfully
    if (Ark::Platform::Time::Configure() == 1) {
        // Now is more recent than 2-years after ARK Epoch 
        assertMore(
                Ark::Platform::Time::Now(),
                1548725761ull); // (28 Jan 2019(in seconds))
    } else { // system not connected
         // Now is less than 1-day 
        assertLess(Ark::Platform::Time::Now(), 86400ull);
    }
}
