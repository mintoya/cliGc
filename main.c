#include "draw/draw.h"
#include "input/input.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#define $runOnce(a)                                                            \
  static char has_run = 0;                                                     \
  if (!has_run) {                                                              \
    a has_run = 1;                                                             \
  }

#define $columns 10
#define $rows 10
typedef struct vector {
  int dr, dc;
} vector;
vector vec(int r, int c) {
  vector v = {r, c};
  return v;
}
vector vadd(vector a, vector b) { return vec(a.dr + b.dr, a.dc + b.dc); }
typedef struct snakePart {
  vector positon;
  Line *look;
  struct snakePart *nextPart;
} snakePart;

vector apple = {1, 1};
snakePart Head = {{0, 0}, NULL, NULL};
static int a = 0;
List *tick() {
  static Box *b = NULL;
  static List *result = NULL;
  if (!b || !result) {
    b = calloc(1, sizeof(Box));
    *b = Box_new(0, 0, 20, 20, hexC("#FFFFFF", "#000001"), L' ');
    Box_set(*b, 0, 0, hexC("#FFFFFF", "#FFFFFF"), L' ');
    result = List_new(sizeof(List));

    Line tline =
        Line_new(5, 5, hexC("#F00000", "#FFF000"), Vertical, L"hello world");
    List *tlist = List_new(sizeof(Line));
    List_append(tlist, &tline);

    List_append(result, (b->lines));
    List_append(result, tlist);
  }
  Box_set(*b, a, a, hexC("#FFFFFF", "#FFFFFF"), L' ');
  return result;
}

void export() {
  List *l = tick();
  draw(l);
  fflush(stdout);
  a++;
}
int main(void) {
  begin(export);
  return (0);
}
