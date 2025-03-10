#include "draw/draw.h"
#include "input/input.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include <assert.h>
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
  if (!b) {
    b = calloc(1, sizeof(Box));
    *b = Box_new(0, 0, 20, 20, hexC("#FFFFFF", "#000001"), L' ');
  }
  return b->lines;
}

void export() {
  $sleep(200);
  List *l = tick();
  draw(l);
  l = NULL;
  a++;
}
int main(void) {
  begin(export);
  return (0);
}
