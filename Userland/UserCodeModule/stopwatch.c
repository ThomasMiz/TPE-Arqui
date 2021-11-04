#include <syscalls.h>
#include <timeUtil.h>

extern uint32_t width, height;
extern Color blackColor, timeColor, color;

#define POSX 3*width/4-5*CHAR_WIDTH
#define POSY height/6-CHAR_HEIGHT/2
#define WIDTH TIME_WITH_TENTHS_LENGTH*CHAR_WIDTH
#define HEIGHT CHAR_HEIGHT
#define LENGTH TIME_WITH_TENTHS_LENGTH+1

static uint8_t isRunning = 0;
static uint64_t millisRunning;

void updateStopwatch(uint64_t millis) {
    if(isRunning) {
        char stopwatchBuff[TIME_WITH_TENTHS_LENGTH+1];
        getTimeWithTenths(stopwatchBuff, millis-millisRunning);
        sys_drawrect(POSX, POSY, WIDTH, HEIGHT, blackColor);
        sys_writeat(stopwatchBuff, LENGTH, POSX, POSY, timeColor);
    }
}

void changeStatusStopwatch(uint64_t millis) {
    millisRunning = millis-millisRunning;
    if(isRunning) {
        isRunning = 0;   
    }
    else {
        isRunning = 1;
    }  
}

void initStopwatch() {
    sys_writeat("00:00:00:0", LENGTH, POSX, POSY, timeColor);
    sys_writeat("Space to stop or resume the timer.\n",35,width/2+CHAR_WIDTH,height/3-CHAR_HEIGHT*2, color);
    sys_writeat("Tab to restart it.",18,width/2+CHAR_WIDTH,height/3-CHAR_HEIGHT, color);
}

void stopStopwatch() {
    isRunning = 0;
    millisRunning = 0;
    sys_drawrect(POSX, POSY, WIDTH, HEIGHT, blackColor);
    initStopwatch();
}
