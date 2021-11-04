#include <timeUtil.h>
#include <syscalls.h>

extern uint32_t width, height;
extern Color blackColor, timeColor;

#define POSX width/4-(TIME_LENGTH/2)*CHAR_WIDTH
#define POSY height/6-CHAR_HEIGHT/2
#define WIDTH TIME_LENGTH*CHAR_WIDTH
#define HEIGHT CHAR_HEIGHT
#define LENGTH TIME_LENGTH+1

void updateTimer(uint64_t millis) {
    char timebuf[TIME_LENGTH+1];
    getTime(timebuf);
    sys_drawrect(POSX, POSY, WIDTH, HEIGHT, blackColor);
    sys_writeat(timebuf, LENGTH, POSX, POSY, timeColor);
}