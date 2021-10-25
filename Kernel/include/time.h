#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

/* Gets the total amount of ticks elapsed since startup. */
uint64_t getElapsedTicks();

/* Gets the total amount of seconds elapsed since startup. */
uint64_t getElapsedSeconds();

#endif