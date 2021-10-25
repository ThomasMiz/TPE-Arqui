#include <stdint.h>
#include <keyboard.h>
#include <naiveConsole.h>

void keyboardIntHandler(uint8_t scancode) {
    if ((scancode & 0b10000000) == BIT_SCANCODE_DOWN)
        ncPrintChar(scancodeToAscii(scancode & 0b01111111));
}

static char scancodeToAsciiTable[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'Q', 'W', 'E', 'R',
    'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',
    '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', ',', '.', '/', 0, 0, 0, ' '
};

#define SCANCODE_ARR_LENGTH (sizeof(scancodeToAsciiTable)/sizeof(scancodeToAsciiTable[0]))

char scancodeToAscii(char scancode) {
    return scancode < SCANCODE_ARR_LENGTH ? scancodeToAsciiTable[scancode] : 0;
}