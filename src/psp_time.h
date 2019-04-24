/**
 * This file is part of ARK Cpp Platform Support Packages.
 *
 * (c) Ark Ecosystem <info@ark.io>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 **/

#ifndef PSP_TIME_H
#define PSP_TIME_H

#include <cstdint>

namespace Ark {
namespace Platform {
namespace Time {

/**
 * int Configure(const int timezone, const int dstOffsetSeconds, const char* server)
 * 
 * Many platforms (namely IoT devices) require a means of configuring it's system time.
 * Without this, it's not possible for those platforms to accurately calculate a TimeStamp.
 * It is typically not necessary on desktop/os platforms.
 * 
 *  1: system connected and time configured sucessfully
 *  0: configuration failed
 * -1: system not connected.
 **/
int Configure(
    const int timezone = 0,
    const int dstOffsetSeconds = 0,
    const char* server = "pool.ntp.org");
/**/

/**
 * uint64_t Epoch(const char* networkEpochStr)
 * 
 * Parses ISO8601-formated TimeStamp String
 **/
uint64_t Epoch(const char* networkEpochStr);
/**/

/**
 * uint64_t Now()
 * 
 * returns the current actual UTC Time in seconds.
 **/
uint64_t Now();
/**/

}; // namespace Time
}; // namespace Platform
}; // namespace Ark

#endif
