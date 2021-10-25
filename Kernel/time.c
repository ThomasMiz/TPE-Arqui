#include <stdint.h>
#include <time.h>
#include <naiveConsole.h>

static uint64_t ticks;

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