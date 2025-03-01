#include "draw/draw.h"
#include "layers/leyers2.h"
#include "list/list.h"
#include "wprint/wprint.h"
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

List *square(int fpses) {
  List *layer = List_new(sizeof(Line));
  wchar_t *l1 = L"hello";
  wchar_t *l2 = L"world";
  wchar_t fps[12] = {0};
  swprintf(fps, 12, L"%d", fpses);
  int offset = rand() % 100;
  Line l1l = Line_new(5, 5 + offset, RED, BG_BLUE, Vertical, l1);
  Line l2l = Line_new(5, 6 + offset, RESET, RESET, Horizontal, l2);
  Line fpsline = Line_new(0, 5, RESET, RESET, Horizontal, fps);
  List_append(layer, &l1l);
  List_append(layer, &fpsline);
  List_append(layer, &l2l);
  return layer;
}

void export() {
  List *l = square(0);
  draw(l);
  Layer_delete(l);
  l = NULL;
}
int main(void) {
  begin(export);
  return (0);
}
