#include <sudoku.h>
#include <syscalls.h>
#include <coreUtil.h>
#include <color.h>

extern uint32_t width, height;

#define VERTICALX (width/2 + (width-width/2-CHAR_WIDTH*21)/2)
#define VERTICALFROMY (height/3 +(height-height/3-CHAR_HEIGHT*21)/2)
#define VERTICALTOY (height - (height-height/3-CHAR_HEIGHT*21)/2)

#define HORIZONTALY VERTICALFROMY
#define HORIZONTALFROMX VERTICALX
#define HORIZONTALTOX (VERTICALX+CHAR_WIDTH*21)

#define SUDOKU_SIZE 9

typedef struct {
    char value;
    uint8_t isBlocked;
    Color color;
} Cell;

static uint8_t isRunning = 0;
static int8_t selectedColumn, selectedRow;
static int8_t lastSelectedColumn, lastSelectedRow;
static Cell currentSudoku[SUDOKU_SIZE][SUDOKU_SIZE];

static char emptySudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'},
    {'0','0','0','0','0','0','0','0','0'}
    };

static char easySudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
    {'0','8','3','4','7','0','0','2','6'},
    {'2','0','7','0','3','0','8','4','0'},
    {'9','4','0','8','2','6','0','3','0'},
    {'0','0','4','7','0','8','2','0','3'},
    {'0','7','8','6','4','0','0','0','0'},
    {'6','0','0','0','0','0','0','7','0'},
    {'0','0','0','0','8','7','0','5','9'},
    {'7','2','0','5','0','0','0','8','4'},
    {'8','5','6','3','0','0','0','0','2'}
};

static char normalSudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
    {'0','0','8','1','0','2','5','0','0'},
    {'0','0','0','8','0','0','3','0','0'},
    {'4','0','0','5','7','0','0','0','6'},
    {'6','0','0','4','9','0','8','0','3'},
    {'0','0','0','0','0','0','9','5','2'},
    {'0','5','0','3','2','0','0','0','7'},
    {'0','8','0','0','0','7','0','0','0'},
    {'0','4','2','0','0','0','7','0','0'},
    {'0','9','3','0','0','0','0','6','8'}
};

static char extremeSudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {
    {'0','0','3','0','0','2','0','0','0'},
    {'4','0','0','0','8','0','0','3','6'},
    {'0','0','8','0','0','0','1','0','0'},
    {'0','4','0','0','6','0','0','7','3'},
    {'0','0','0','9','0','0','0','0','0'},
    {'0','0','6','0','0','0','0','0','0'},
    {'0','0','4','0','7','0','0','6','8'},
    {'6','0','0','0','0','0','0','0','0'},
    {'7','0','0','0','0','0','5','9','0'}
};

static uint8_t check(Cell board[SUDOKU_SIZE][SUDOKU_SIZE], int boardSize, uint8_t i, uint8_t j) {
    char elem = board[i][j].value;
    for (int col = 0;col < boardSize;col++)
        if (col != j && board[i][col].value == elem)
            return 0;
    for (int row = 0; row < boardSize; row++)
        if (row != i && board[row][j].value == elem)
            return 0;
    for (int m = i-i%3, count=0; count < 3; m++, count++)
        for (int n = j-j%3, count1=0; count1 < 3; n++, count1++)
            if (board[m][n].value == elem)
                if (m != i || n != j)
                    return 0;
    return 1;
}

static uint8_t isValidSudoku(Cell board[SUDOKU_SIZE][SUDOKU_SIZE], int boardSize) {
    uint8_t checked = 1;
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++){
            if(board[i][j].value=='0') {
                checked = 0;
            }
            if (board[i][j].value>='1' && board[i][j].value <='9')
                if (!board[i][j].isBlocked) {
                    if(!check(board, boardSize, i, j)) {
                        checked = 0;
                        board[i][j].color = red;
                    }
                    else {
                        board[i][j].color = green;
                    }
                    
                }
        } 
                    
    return checked;
}

static void updateCellSudoku(uint8_t row, uint8_t column, Color color) {
    sys_drawrect(VERTICALX+CHAR_WIDTH*(column*2+1+(int)column/3),HORIZONTALY+CHAR_HEIGHT*(row*2+1+(int)row/3),CHAR_WIDTH,CHAR_HEIGHT,black);
    sys_writeat(&currentSudoku[row][column].value, 1, 
                VERTICALX+CHAR_WIDTH*(column*2+1+(int)column/3), 
                HORIZONTALY+CHAR_HEIGHT*(row*2+1+(int)row/3),color);
}

static void drawFrameSudoku() {
    sys_drawrect(VERTICALX, VERTICALFROMY,CHAR_WIDTH*21, CHAR_HEIGHT*21, black);
    for(int i=0; i<4; i++) {
        sys_drawline(VERTICALX+(CHAR_WIDTH*i*7), VERTICALFROMY, VERTICALX+(CHAR_WIDTH*i*7), VERTICALTOY, blue);
        sys_drawline(HORIZONTALFROMX, HORIZONTALY+(CHAR_HEIGHT*i*7), HORIZONTALTOX, HORIZONTALY+(CHAR_HEIGHT*i*7), blue);
    }
}

static void drawNumbersSudoku(Cell sudoku[9][9]) {
    for(int i=0; i<SUDOKU_SIZE; i++) {
        for(int j=0; j<SUDOKU_SIZE; j++) {
            updateCellSudoku(i,j,sudoku[i][j].color);    
        }
    }
}

void initSudoku() {
    for(int i=0; i<SUDOKU_SIZE; i++) {
        for(int j=0; j<SUDOKU_SIZE; j++) {
            currentSudoku[i][j].value = emptySudoku[i][j];
            currentSudoku[i][j].isBlocked = 1;
            currentSudoku[i][j].color = white;
        }
    }
    
    drawFrameSudoku();
    drawNumbersSudoku(currentSudoku);

    sys_writeat("Press 1 for easy, 2 for normal or 3 for extreme mode.", 53, width/2+CHAR_WIDTH, height-CHAR_HEIGHT*3, gray);
    sys_writeat("Press arrows to move around the board.", 38, width/2+CHAR_WIDTH, height-CHAR_HEIGHT*2, gray);
    sys_writeat("Press numbers to put them in the selected cell.", 47, width/2+CHAR_WIDTH, height-CHAR_HEIGHT, gray);

    selectedRow = 0;
    selectedColumn = 0;
    lastSelectedRow = 0;
    lastSelectedColumn = 0;
}

static void youWinSudoku() {
    for(int i=0; i<SUDOKU_SIZE; i++) {
        for(int j=0; j<SUDOKU_SIZE; j++) {
            currentSudoku[i][j].color = green;
            updateCellSudoku(i,j,currentSudoku[i][j].color);
        }
    }
    initSudoku();
    isRunning = 0;
}

void updateSudoku(char number) {
    if(isRunning==0 && number>='1' && number<='3') {
        isRunning = 1;
        char* p;
        if(number=='1') {
            p = &easySudoku[0][0];
        }
        else if(number=='2') {
            p = &normalSudoku[0][0];
        }
        else {
            p = &extremeSudoku[0][0];
        }
        for(int i=0; i<SUDOKU_SIZE; i++) {
            for(int j=0; j<SUDOKU_SIZE; j++) {
                currentSudoku[i][j].value = p[j+SUDOKU_SIZE*i];
                currentSudoku[i][j].isBlocked = p[j+SUDOKU_SIZE*i]!='0';
                if(currentSudoku[i][j].isBlocked){
                    currentSudoku[i][j].color=white;
                }
                else{
                    currentSudoku[i][j].color=blue;
                }
                    
            }
        }
        drawNumbersSudoku(currentSudoku);
    }
    else{
        if(!currentSudoku[selectedRow][selectedColumn].isBlocked) {
            currentSudoku[selectedRow][selectedColumn].value = number;
            if(isValidSudoku(currentSudoku,SUDOKU_SIZE))
                 youWinSudoku(); 
            else
                drawNumbersSudoku(currentSudoku);
        }
    }
}


void changeCellSudoku(uint8_t scancode) {
    lastSelectedRow = selectedRow;
    lastSelectedColumn = selectedColumn;

    if(scancode == UP) {
        selectedRow = MAX(selectedRow-1,0); 
    }
    if(scancode == DOWN) {
        selectedRow = MIN(8,selectedRow+1);
    }
    if(scancode == LEFT) {
        selectedColumn = MAX(0,selectedColumn-1);
    }
    if(scancode == RIGHT) {
        selectedColumn = MIN(8, selectedColumn+1);
    } 
    updateCellSudoku(lastSelectedRow, lastSelectedColumn, currentSudoku[lastSelectedRow][lastSelectedColumn].color);

    updateCellSudoku(selectedRow, selectedColumn, yellow);
}