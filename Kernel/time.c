#include <stdint.h>
#include <time.h>
#include <naiveConsole.h>

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

    static int lastSecondPrinted = 4;
    int currentSeconds = getElapsedSeconds();
    if (currentSeconds % 5 == 0 && currentSeconds > lastSecondPrinted) {
        ncClear();
        ncPrintDec(currentSeconds);
        ncPrint(" seconds have passed.");
        lastSecondPrinted = currentSeconds;
    }
}

uint64_t getElapsedTicks() {
    return ticks;
}

uint64_t getElapsedSeconds() {
    return ticks * 5 / 91; // 18.2 ticks per second
}

Time getActualTime() {
    Time time = {getCurrentSeconds(), getCurrentMinutes(), getCurrentHours()};
    return time;
}