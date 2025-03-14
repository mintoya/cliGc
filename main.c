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

static int a = 0;
List *tick() {
  static Box *b = NULL;
  static List *result = NULL;
  if (!b || !result) {
    b = calloc(1, sizeof(Box));
    *b = Box_new(10, 10, 20, 20, hexC("#FFFFFF", "#000001"), L' ');
    result = List_new(sizeof(List));

    /* Line tline = */
    /*     Line_new(5, 5, hexC("#F00000", "#FFF000"), Vertical, L"hello world");
     */
    /* List *tlist = List_new(sizeof(Line)); */
    /* List_append(tlist, &tline); */

    List_append(result, (b->lines));
    /* List_append(result, tlist); */
  }
  Box_set(*b, a + 2, a + 2, hexC("#FFFFFF", "#FF8F0F"), L' ');
  return result;
}

int main(void) {
  begin();
  while (1) {
    draw(tick());
    a++;
  }
  return (0);
}
