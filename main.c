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
static Box *b = NULL;
List *tick() {
  // clang-format off
  $runOnce(
      Box *box = calloc(1, sizeof(Box));
      *box = Box_new(2, 1, 10, 10, RESET, BG_BLUE, L' '); b = box;
       Head.look = malloc(sizeof(Line));
       *(Head.look) = Line_new(0, 0, RESET, BG_RED, Horizontal, L" ");
       List_prettyPrint(box->lines); $sleep(3000);
  );
  // clang-format on
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
