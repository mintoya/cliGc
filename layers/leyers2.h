#ifndef LEYERS2_H
#include "../list/list.h"
#include "../tSize/terminal_size.h"
#include "../wprint/wprint.h"
#include <locale.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define BENCHMARK 0 // turns off the diff functino

#define $eq(a, b) !memcmp(&a, &b, sizeof(a))

void setCursorPosition(int row, int col);

typedef enum {
  RESET = 0,
  RED = 31,
  GREEN = 32,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
  WHITE = 37,
  BRIGHT_RED = 91,
  BRIGHT_GREEN = 92,
  BRIGHT_YELLOW = 93,
  BRIGHT_BLUE = 94,
  BRIGHT_MAGENTA = 95,
  BRIGHT_CYAN = 96,
  BRIGHT_WHITE = 97,
  BG_BLACK = 40,
  BG_RED = 41,
  BG_GREEN = 42,
  BG_YELLOW = 43,
  BG_BLUE = 44,
  BG_MAGENTA = 45,
  BG_CYAN = 46,
  BG_WHITE = 47,
  BG_BRIGHT_BLACK = 100,
  BG_BRIGHT_RED = 101,
  BG_BRIGHT_GREEN = 102,
  BG_BRIGHT_YELLOW = 103,
  BG_BRIGHT_BLUE = 104,
  BG_BRIGHT_MAGENTA = 105,
  BG_BRIGHT_CYAN = 106,
  BG_BRIGHT_WHITE = 107
} Color;
typedef enum {
  Horizontal = 1,
  Vertical = 0,
} Direction;

typedef struct {
  uint8_t row;
  uint8_t col;
  Color fgColor;
  Color bgColor;
  Direction ori;
  wchar_t *contents; // cant contain escape sequences
} Line;
typedef struct {
  wchar_t g;
  Color fg;
  Color bg;
} Cell;

const wchar_t *getColors(Color foreground, Color background) ;
Line Line_new(int row, int col, Color fg, Color bg, Direction orientation,
              wchar_t *content);
void Layer_delete(List *l) ;
void stringAppend(List *l, const wchar_t *nTString);
void rasterset(Cell *c, TerminalSize ts, int row, int col, Cell element);
Cell *Leyer_rasterize(List *l, TerminalSize ts);
void cellArrMerge(Cell *completeLeyer, Cell *incompleteLeyer) ;

Cell *bottomLayer(TerminalSize ts) ;
void printCells(Cell *cellLayer) ;

void printCellDiff(Cell *cellLayer) ;

void box(List *content) ;
#endif // !LEYERS2_H
