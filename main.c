#include "layers/leyers2.c"
#include "list/list.h"
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>
#ifdef _WIN32
#include <windows.h>
#endif
static int fpsInt = 0;
static int storedFps = 0;
/* i dont know what a mutex is  */
List *square(int fpses) {
  static int isB;
  if (!(rand() % 1000)) {
    isB = !isB;
    /* isB = 1; */
  }
  List *layer = List_new(sizeof(Line));
  wchar_t *l1 = (isB) ? L"hello" : L"Bozo!";
  wchar_t *l2 = (isB) ? L"world" : L"Bozo!";
  wchar_t fps[12] = {0};
  swprintf(fps, 12, L"%d", fpses);
  int offset = isB;
  Line l1l = Line_new(5, 5 + offset, RED, BG_BLUE, Vertical, l1);
  Line l2l = Line_new(5, 6 + offset, RESET, RESET, Horizontal, l2);
  Line fpsline = Line_new(0, 5, RESET, RESET, Horizontal, fps);
  List_append(layer, &l1l);
  List_append(layer, &fpsline);
  List_append(layer, &l2l);
  return layer;
}
void boxy() {
  List *l = square(storedFps);
  box(l);
  Layer_delete(l);
  l = NULL;
  setCursorPosition(0, 0);
  fflush(stdout);
}
void cursorOff() { WFPRINT(L"\033[?25l"); }
void cursorOn() { WFPRINT(L"\033[?25h"); }

void *counter(void *vargp) {
  int myid = getpid();
  cursorOff();
  while (1) {
    $sleep(1000);
    storedFps = fpsInt;
    fpsInt = 0;
  }
}
/* void *frame(void) { */
void *frame(void *vargp) {
  int myid = getpid();
  cursorOff();
  while (1) {
    boxy();
    fpsInt++;
  }
}
int main(void) {
  setlocale(LC_ALL, "");
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  setlocale(LC_ALL, ".UTF-8");
#endif

  $sleep(3000);
  setvbuf(stdout, NULL, _IOFBF, 16384);
  pthread_t threadid;
  pthread_create(&threadid, NULL, counter, NULL);
  pthread_create(&threadid, NULL, frame, NULL);
  pthread_exit(NULL);
  return (0);
}
