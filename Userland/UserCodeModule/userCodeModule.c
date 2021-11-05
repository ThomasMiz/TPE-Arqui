#include <stdint.h>
#include <syscalls.h>
#include <timeUtil.h>
#include <coreUtil.h>
#include <string.h>
#include <stopwatch.h>
#include <timer.h>
#include <hangman.h>
#include <sudoku.h>
#include <color.h>
#define READBUF_LENGTH 32
#define COMMANDS_LENGTH 7

uint32_t width, height;
uint32_t penpos = 0;

void getConsoleSize(uint32_t* width, uint32_t* height) {
	uint64_t size = sys_screensize();
	*width = (size & 0xFFFFFFFF);
	*height = (size >> 32);
}

void help() {
	penpos = sys_writeat(
	"DIVIDEANDCONQUER              The screen will be divided into four windows with the following functions:\n"
	".                             1) The time will be displayed in hh:mm:ss format permanently updating.\n"
	".                             2) In another window there will be a stopwatch with tenths of a second to be\n"
	".                             operated from the keyboard.\n"
	".                             3) In a third window user can play Sudoku by detecting the game completion and\n"
	".                             possible invalid moves.\n"
	".                             4) In the fourth window user can play Hangman.\n"
	"HELP                          Provides help information for commands.\n"
	"INFOREG                       Prints on screen the value of all registers.\n"
	"PRINTMEM                      Receives as argument a pointer and performs a memory dump of 32 bytes from the\n"
	".                             address received as an argument.\n"
	"TIME                          Command to display the system day and time.\n"
	"WHATIFIDIVIDEBY0              Command to verify the operation of the exception routine \"Divide by zero\"\n"
	"WHATIFISEEANINVALIDOPCODE     Command to verify the operation of the exception routine \"Invalid Opcode\"\n",
	1210,(penpos & 0x0000FFFF), (penpos >> 16), gray);
}

void divideByZero() {

}

void invalidOPCode() {

}

void time() {
	char buffer[DATE_AND_TIME_LENGTH+1];
	getDateAndTime(buffer);
	penpos = sys_writeat(buffer, DATE_AND_TIME_LENGTH+1, (penpos & 0xFFFF), (penpos >> 16), green);
	penpos = sys_writeat("\n", 1, (penpos & 0xFFFF), (penpos >> 16), gray);
}

void inforeg() {

}

void printMem() {

}

void divideAndConquer() {
	sys_clearscreen();
	sys_drawline(width/2, 0, width/2, height, magenta); //vertical line
	sys_drawline(0, height/3, width, height/3, magenta); //horizontal line
	initStopwatch();
	initSudoku();
	initHangman();

	char readbuf[1];
	uint64_t lastmillis = 0;

	while(1) {
		uint64_t readlen = sys_pollread(KBDIN, readbuf, 1, 1);
		uint64_t millis = sys_millis();

		if(millis != lastmillis) {
			lastmillis = millis;
			updateTimer(millis);
			updateStopwatch(millis);
		}

		if(readlen != 0) {	
			char ascii = scancodeToAscii(readbuf[0]);
			if(ascii == ' ') {
				changeStatusStopwatch(millis);
			}
			else if(ascii == '\t') {
				stopStopwatch();
			}
			else if(readbuf[0] == UP || readbuf[0] == DOWN || readbuf[0] == LEFT || readbuf[0] == RIGHT) {
				changeCellSudoku(readbuf[0]);
			}
			else if(ascii>48 && ascii<58) {
				updateSudoku(ascii);
			}
		}
	}
}

static char* commands[] = {"divideandconquer", "help", "inforeg", "printmem", "time","whatifidivideby0","whatifiseeaninvalidopcode"};
static void (*commands_functions[])(void) = {divideAndConquer, help, inforeg, printMem, time, divideByZero, invalidOPCode};

char indexCommand(char* readbuf) {
	char *p = readbuf;
	for(int i=0; i<COMMANDS_LENGTH; i++) {
		int cmp;
		if((cmp = strcmp(p,commands[i])) < 0){
			return -1;
		}	
		if(cmp == 0){
			return i;
		}
	}
	return -1;
}

void scanCommand(char* readbuf, uint8_t maxLen) {
	char * p = readbuf;
	char i = 0;
	char maxLenReached = 0;

	while(1) {
		if(i == maxLen) {
			i = 0;
			maxLenReached = 1;
		}

		uint64_t readlen = sys_pollread(STDIN, &p[i], maxLen-i, 1);

		if(readlen != 0) {
			penpos = sys_writeat(&p[i], readlen, (penpos & 0xFFFF), (penpos >> 16), gray);
		}
		
		i += readlen;

		for(int j=i-readlen; j<i; j++) {
			if(p[j] =='\n') {
				if(maxLenReached == 0) {
					p[j] = 0;
				}
				else {
					p[0] = 0;
				}
				return;
			}
		}
	}
}

int main() {
	getConsoleSize(&width, &height);

	sys_clearscreen();	
	
	penpos = sys_writeat("$ ",2,(penpos & 0xFFFF), (penpos >> 16), magenta);

	while(1) {
		char readbuf[READBUF_LENGTH] = {0};
		scanCommand(readbuf,READBUF_LENGTH);
		char index;
		if((index = indexCommand(readbuf))>=0) {
			commands_functions[index]();
		}
		else {
			penpos = sys_writeat("No se encontro el comando.\n",27,(penpos & 0xFFFF), (penpos >> 16), gray);
		}
		penpos = sys_writeat("$ ",2,(penpos & 0xFFFF), (penpos >> 16), magenta);
	}
	return 420;
}