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

static void help() {
	static int helpCounter = 0;
	if (helpCounter++ >= 3 && ((sys_millis() / 100) % 4) == 0) {
		print("I need somebody!\n", 17, yellow);
		helpCounter = 0;
		return;
	}

	const char* helpstring = 
	"CLEAR                Clears the console.\n"
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
	clearscreen();
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
				clearscreen();
				break;
			}
		}
	}

	clearscreen();
}

static void fractal() {
	frc_run();
	clearscreen();
}

static void clear() {
	clearscreen();
}

static const char* commands[] = {"clear", "divideandconquer", "dividebyzero", "fractal", "help", "inforeg", "invalidopcode", "printmem", "time"};
static void (*commands_functions[])(void) = {clear, divideAndConquer, divideByZero, fractal, help, inforeg, invalidOPCode, printMem, time};
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

int main() {
	getScreenSize(&width, &height);

	clearscreen();	
	print("Welcome to Shell! Type \"HELP\" for a list of commands.\n\n", 55, green);

	while(1) {
		print("$ ", 2, magenta);
		char readbuf[READBUF_LENGTH] = {0};
		scanf(readbuf,READBUF_LENGTH);
		char index;
		if((index = indexCommand(readbuf))>=0) {
			commands_functions[index]();
		}
		else {
			print("No se encontro el comando.\n", 27, gray);
		}
	}

	return 420;
}
