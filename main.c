#include "draw/draw.h"
#include "input/input.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
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
  struct snakePart *nextPart;
} snakePart;

vector apple = {1, 1};
snakePart Head = {{0, 0}, NULL};
static int a = 0;
Box *b = NULL;
List *tick() {
  if (b == NULL) {
    Box *box = calloc(1, sizeof(Box));
    *box = Box_new(2, 1, 10, 10, BLUE, RESET, L'S');
    b = box;
  }
  Box_set(*b, 0, a, L'A');
  return b->lines;
}

void export() {
  $sleep(200);
  a++;
  List *l = tick();
  draw(l);
  /* Layer_delete(l); */
  l = NULL;
}
int main(void) {
  begin(export);
  return (0);
}
