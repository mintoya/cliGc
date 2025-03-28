#include "draw/draw.h"
#include "input/input.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include "template/template.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <unistd.h>
#include <wchar.h>
#define RED "#FF0000"

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

    Line tline =
        Line_new(5, 5, hexC("#F00000", "#FFF000"), Vertical, L"hello world");

    List *tlist = List_new(sizeof(Line));
    List_append(tlist, &tline);

    List_append(result, (b->lines));
    List_append(result, tlist);
  }
  Box_set(*b, a, a + 2, hexC("#FFFFFF", "#FF8F0F"), L' ');
  return result;
}

int main(void) {

  // clang-format off
  Node root = {
      .position = {.dcol = {.n = {0, 1}, .isDefined = 1},
                   .drow = {.n = {0, 1}, .isDefined = 1}},
      .size = {.dcol = {.n = {1, 1}, .isDefined = 0},
               .drow = {.n = {1, 1}, .isDefined = 0}},
      .color = {{0, 0, 0}, {0, 0, 0}},
      .children = NULL,
      .self = NULL,
  };
  Node example = {
      .children = NULL,
      .color = hexC("#111111", "#FF8888"),
      .position ={.dcol = {.n = {1, 2}, .isDefined = 0,},
                  .drow = {.n = {1, 2}, .isDefined = 0,},},
      .size = { .dcol = { .isDefined = 0, .n = {2, 4}, },
                .drow = { .isDefined = 0, .n = {3, 6}, },},
      .self = { .lines = NULL, },
  };
  Node example2 = {
      .children = NULL,
      .color = hexC("#111111", "#FF0000"),
      .position =
          { .dcol = { .isDefined = 0, .n = {1, 2}, },
            .drow = { .isDefined = 0, .n = {1, 2}, },},
      .size =
          { .dcol = { .isDefined = 0, .n = {2, 4}, },
            .drow = { .isDefined = 0, .n = {3, 6}, },},
      .self = { .lines = NULL, },
  };
  Node example3 = {
      .children = NULL,
      .color = hexC("#111111", "#FFFF00"),
      .position =
          { .dcol = { .isDefined = 1, .n = {1, 1}, },
            .drow = { .isDefined = 1, .n = {1, 1}, },},
      .size =
          { .dcol = { .isDefined = 0, .n = {2, 4}, },
            .drow = { .isDefined = 0, .n = {3, 6}, },},
      .self = { .lines = NULL, },
  };
  // clang-format on
  example.children = List_new(sizeof(Node));
  List_append(example.children, &example2);
  List_append(example.children, &example3);
  root.children = List_new(sizeof(Node));
  List_append(root.children, &example);
  Node *ref = (Node *)List_gst(example.children, 1);

  /* begin(); */
  while (1) {
    $sleep(100);
    /* draw(renderNodes(&root)); */
    syncDraw(renderNodes(&root));
    /* draw(tick()); */
    ref->position.dcol.n.numerator = a % 6;
    ref->position.drow.n.numerator = a % 6;
    a++;
  }
  return (0);
}
