#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#include <stdint.h>

void initSudoku();
void changeCellSudoku(uint8_t scancode);
void updateSudoku(char number);

#endif