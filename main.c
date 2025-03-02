#include "draw/draw.h"
#include "input/input.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include <unistd.h>
#include <wchar.h>
#define $columns 10
#define $rows 10
typedef struct snakePart {
  int row, column;
  struct snakePart *nextPart;
} snakePart;

typedef enum direction {
  up = 1,
  down = 2,
  right = 3,
  left = 4,
} direction;

static snakePart nextnext = {1, 1, NULL};
static snakePart next = {1, 1, &nextnext};
static snakePart Head = {1, 1, &next};
static direction HeadDirection = 3;
List *tick() {

  int rowMove = 0;
  int colMove = 0;
  if (_kbhit()) {
    int ch = getch();
    switch (ch) {
    case 'w':
      HeadDirection = up;
      break;
    case 'a':
      HeadDirection = left;
      break;
    case 's':
      HeadDirection = down;
      break;
    case 'd':
      HeadDirection = right;
      break;
    }
  }
  switch (HeadDirection) {
  case right:
    colMove++;
    break;
  case left:
    colMove--;
    break;
  case down:
    rowMove++;
    break;
  case up:
    rowMove--;
    break;
  }

  int headRow = Head.row;
  int headCol = Head.column;
  Head.row += rowMove;
  Head.column += colMove;

  snakePart *csnake = Head.nextPart;
  while (csnake) {
    int tcol, trow;
    tcol = csnake->column;
    trow = csnake->row;
    csnake->column = headCol;
    csnake->row = headRow;
    headCol = tcol;
    headRow = trow;
    csnake = csnake->nextPart;
  }

  List *rows = List_new(sizeof(Line));
  wchar_t template[$columns + 1] = {0};
  for (int i = 0; i < $columns; i++) {
    template[i] = L' ';
  }
  for (int i = 0; i < $rows; i++) {
    Line tline = Line_new(i, 0, RESET, BG_BLUE, Horizontal, template);
    List_append(rows, &tline);
  }
  csnake = &Head;
  while (csnake) {
    Line tline =
        Line_new(csnake->row, csnake->column, RED, RESET, Horizontal, L"");
    List_append(rows, &tline);
    csnake = csnake->nextPart;
  }
  return rows;
}

void export() {
  $sleep(25);
  List *l = tick();
  $sleep(25);
  draw(l);
  Layer_delete(l);
  l = NULL;
}
int main(void) {
  begin(export);
  return (0);
}
