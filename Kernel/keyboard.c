#include <stdint.h>
#include <naiveConsole.h>

void keyboardIntHandler(uint8_t scancode) {
    ncPrintHex(scancode);
}