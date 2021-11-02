#include <stdint.h>
#include <time.h>

extern uint8_t getCurrentHours();
extern uint8_t getCurrentMinutes();
extern uint8_t getCurrentSeconds();

static uint64_t ticks;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} Time;

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

Time getActualTime() {
    Time time = {getCurrentSeconds(), getCurrentMinutes(), getCurrentHours()};
    return time;
}
