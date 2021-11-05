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
#include <libasm.h>
#include <fractal.h>
#define READBUF_LENGTH 32

uint32_t width, height;
static uint32_t penpos = 0;

static void getScreenSize(uint32_t* width, uint32_t* height) {
	uint64_t size = sys_screensize();
	*width = (size & 0xFFFFFFFF);
	*height = (size >> 32);
}

static void print(const char* buf, uint64_t count, Color color) {
	penpos = sys_writeat(buf, count, (penpos & 0xFFFF), (penpos >> 16), color);
}

static void help() {
	const char* helpstring = 
	"DIVIDEANDCONQUER     The screen will be divided into four windows with the following functions:\n"
	".                    1) The time will be displayed in hh:mm:ss format permanently updating.\n"
	".                    2) In another window there will be a stopwatch with tenths of a second to be\n"
	".                    operated from the keyboard.\n"
	".                    3) In a third window user can play Sudoku by detecting the game completion and\n"
	".                    possible invalid moves.\n"
	".                    4) In the fourth window user can play Hangman.\n"
	"HELP                 Provides help information for commands.\n"
	"INFOREG              Prints on screen the value of all registers.\n"
	"PRINTMEM             Receives as argument a pointer and performs a memory dump of 32 bytes from the\n"
	".                    address received as an argument.\n"
	"TIME                 Command to display the system day and time.\n"
	"DIVIDEBYZERO         Command to verify the operation of the exception routine \"Divide by zero\"\n"
	"INVALIDOPCODE        Command to verify the operation of the exception routine \"Invalid Opcode\"\n"
	"FRACTAL              Draws a pretty fractal on the screen. Warning: high CPU usage.\n";
	print(helpstring, strlen(helpstring), gray);
}

static void divideByZero() {
	int i = 5 / 0;
}

static void invalidOPCode() {
	print("Running invalid opcode...", 25, gray);
	runInvalidOpcode();
}

static void time() {
	char buffer[DATE_AND_TIME_LENGTH+1];
	getDateAndTime(buffer);
	print(buffer, DATE_AND_TIME_LENGTH+1, green);
	print("\n", 1, gray);
}

static const char* registerNames[18] = {
    "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"
};

static void inforeg() {
	const uint64_t* regdata = dumpRegisters();
	char buf[18];
	buf[0] = '0';
	buf[1] = 'x';

	for (int i = 0; i < 16; i++) {
        print(registerNames[i], 3, gray);
        print(": ", 2, gray);
    	uint64ToHex(regdata[i], buf+2);
        print(buf, sizeof(buf), gray);
		if ((i % 4) == 3)
			print("\n", 1, gray);
		else
			print("   ", 3, gray);
	}
}

static void printMem() {

}

void divideAndConquer() {
	sys_clearscreen();
	sys_drawline(width/2, 0, width/2, height, magenta); //vertical line
	sys_drawline(0, height/3, width, height/3, magenta); //horizontal line
	sys_writeat("Press ESC to exit.",18,CHAR_WIDTH,0,gray);
	stw_init();
	sdk_init();
	hang_init();

	char readbuf[1];
	uint64_t lastmillis = 0;

	while(1) {
		uint64_t readlen = sys_pollread(KBDIN, readbuf, 1, 1);
		uint64_t millis = sys_millis();

		if(millis != lastmillis) {
			lastmillis = millis;
			timer_update(millis);
			stw_update(millis);
		}

		if(readlen != 0) {	
			char ascii = scancodeToAscii(readbuf[0]);
			if(ascii == ' ') {
				stw_changeStatus(millis);
			}
			else if(ascii == '\t') {
				stw_stop();
			}
			else if(readbuf[0] == UP || readbuf[0] == DOWN || readbuf[0] == LEFT || readbuf[0] == RIGHT) {
				sdk_move(readbuf[0]);
			}
			else if(ascii>48 && ascii<58) {
				sdk_update(ascii);
			}
			else if(ascii>='a' && ascii<='z') {
				hang_update(ascii);
			}
			else if(ascii==27) {
				sys_clearscreen();
				penpos = 0;
				break;
			}
		}
	}
}

static void fractal() {
	frc_run();
	sys_clearscreen();
	penpos = 0;
}

static const char* commands[] = {"divideandconquer", "dividebyzero", "fractal", "help", "inforeg", "invalidopcode", "printmem", "time"};
static void (*commands_functions[])(void) = {divideAndConquer, divideByZero, fractal, help, inforeg, invalidOPCode, printMem, time};
#define COMMANDS_LENGTH (sizeof(commands)/sizeof(commands[0]))

static char indexCommand(char* readbuf) {
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

static void scanCommand(char* readbuf, uint8_t maxLen) {
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
			print(&p[i], readlen, gray);
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
	getScreenSize(&width, &height);

	sys_clearscreen();	
	
	print("$ ", 2, magenta);

	while(1) {
		char readbuf[READBUF_LENGTH] = {0};
		scanCommand(readbuf,READBUF_LENGTH);
		char index;
		if((index = indexCommand(readbuf))>=0) {
			commands_functions[index]();
		}
		else {
			print("No se encontro el comando.\n", 27, gray);
		}
		print("$ ", 2, magenta);
	}

	return 420;
}
