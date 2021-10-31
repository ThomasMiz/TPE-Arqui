#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

#define TICKS_TO_SECONDS(x) ((x) * 5 / 91)
#define TICKS_TO_MILLISECONDS(x) ((x) * 5000 / 91);

/* Gets the total amount of ticks elapsed since startup. */
uint64_t getElapsedTicks();

/* Gets the total amount of seconds elapsed since startup. */
uint64_t getElapsedSeconds();

/* Gets the total amount of milliseconds elapsed since startup. */
uint64_t getElapsedMilliseconds();

#endif
