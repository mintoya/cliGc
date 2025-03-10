#ifndef LEYERS2_H
#define LEYERS2_H
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

typedef struct rgbColor {
  uint8_t fg[3];
  uint8_t bg[3];
} rgbColor;
typedef enum {
  Horizontal = 1,
  Vertical = 0,
} Direction;

typedef struct {
  int row;
  int col;
  rgbColor color;
  Direction ori;
  wchar_t *contents; // cant contain escape sequences
} Line;
typedef struct {
  wchar_t g;
  rgbColor color;
} Cell;
Line Line_new(int row, int col, rgbColor color, Direction orientation,
              wchar_t *content) ;
void Layer_delete(List *l) ;

typedef struct {
  List *lines;
  int row, col, brow, bcol;
  rgbColor defcolor;
  wchar_t defchar;
} Box;
rgbColor hexC(char fgColor[7], char bgColor[7]);
void Box_set(Box box, int row, int col, rgbColor color, wchar_t val);
void pretty_print_box(const Box box);
void Box_reset(Box box, int row, int col) ;
Box Box_new(int row, int col, int brow, int bcol, rgbColor color,wchar_t fill);
void box(List *content) ;
#endif // !LEYERS2_H
