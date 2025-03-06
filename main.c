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
static Box *b = NULL;
List *tick() {
  if (b == NULL) {
    Box *box = calloc(1, sizeof(Box));
    *box = Box_new(2, 1, 10, 10, RESET, BG_BLUE, L' ');
    Line templine = {0, 0, RED, BG_BLUE, Horizontal, setAlloc(L"hello")};
    /* List_append(b->lines, &templine); */
    b = box;
  }
  Box_set(*b, a, a, L'S');
  Box_set(*b, a, 0, L'S');
  Box_set(*b, 0, a, L'S');
  Line l = Line_new(6, 5, BLUE, BG_RED, Horizontal, L" ");
  List_append(b->lines, &l);
  return b->lines;
}

void export() {
  $sleep(200);
  a++;
  List *l = tick();
  draw(l);
  l = NULL;
}
int main(void) {
  begin(export);
  return (0);
}
