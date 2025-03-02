#include "draw/draw.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include <unistd.h>
#include <wchar.h>
#define $columns 10
#define $rows 10
#ifdef _WIN32
#include <windows.h>
void wsleep(useconds_t usec) {
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative
                              // value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#define $sleep(a) wsleep(a * 1000)
#else
#define $sleep(a) usleep(a * 1000)
#endif

typedef struct snakePart {
  int row, column;
  struct snakePart *nextPart, *prevPart;
} snakePart;

typedef enum direction {
  up = 1,
  down = 2,
  right = 3,
  left = 4,
} direction;

static snakePart Head = {0, 0, NULL, NULL};
static direction HeadDirection = 3;
List *tick() {
  List *rows = List_new(sizeof(Line));
  wchar_t template[$columns + 1] = {0};
  for (int i = 0; i < $columns; i++) {
    template[i] = L' ';
  }
  for (int i = 0; i < $rows; i++) {
    Line tline = Line_new(i, 0, RESET, BG_BLUE, Horizontal, template);
    List_append(rows, &tline);
  }
  snakePart *csnake = &Head;
  while (csnake) {
    Line tline = Line_new(csnake->row, csnake->column, RESET, BG_RED,
                          Horizontal, L"");
    List_append(rows, &tline);
    csnake = csnake->nextPart;
  }
  int rowMove = 0;
  int colMove = 0;
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

  csnake = Head.nextPart;
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

  return rows;
}

void export() {
  List *l = tick();
  $sleep(1000);
  draw(l);
  Layer_delete(l);
  l = NULL;
}
int main(void) {
  begin(export);
  return (0);
}
