#include <stdint.h>
#include <time.h>

extern uint8_t getCurrentHours();
extern uint8_t getCurrentMinutes();
extern uint8_t getCurrentSeconds();
extern uint8_t getCurrentDay();
extern uint8_t getCurrentMonth();
extern uint8_t getCurrentYear();

static uint64_t ticks;

void timerIntHandler(void) {
    ticks++;
}

uint64_t getElapsedTicks() {
    return ticks;
}

uint64_t getElapsedSeconds() {
    return TICKS_TO_SECONDS(ticks);
}

uint64_t getElapsedMilliseconds() {
    return TICKS_TO_MILLISECONDS(ticks);
}

uint64_t getCurrentTime() {
    return getCurrentHours() | ((uint64_t)getCurrentMinutes() << 8) | ((uint64_t)getCurrentSeconds() << 16);
}

uint64_t getCurrentDate() {
    return getCurrentDay() | ((uint64_t)getCurrentMonth() << 8) | ((uint64_t)getCurrentYear() << 16);
}
