#include "layers/leyers2.c"
#include "list/list.h"
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

void cursorOff() { WFPRINT(L"\033[?25l"); }
void cursorOn() { WFPRINT(L"\033[?25h"); }

static pthread_t threadIds[2];
static int fpsInt = 0;
static int storedFps = 0;
static pthread_mutex_t lock;
volatile sig_atomic_t terminate = 0;
void handle_sigint(int sig) { terminate = 1; }

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
void boxy() {
  List *l = square(storedFps);
  box(l);
  Layer_delete(l);
  l = NULL;
}

void *counter(void *vargp) {
  int myid = getpid();
  while (!terminate) {
    $sleep(1000);
    pthread_mutex_lock(&lock);
    storedFps = fpsInt;
    fpsInt = 0;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
/* void *frame(void) { */
void *frame(void *vargp) {
  int myid = getpid();
  cursorOff();
  while (!terminate) {
    pthread_mutex_lock(&lock);
    boxy();
    fpsInt++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}
int main(void) {
  setlocale(LC_ALL, "");
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  setlocale(LC_ALL, ".UTF-8");
#endif
  if (pthread_mutex_init(&lock, NULL)) {
    printf("\n mutex init has failed\n");
    return 1;
  }
  signal(SIGINT, handle_sigint);
  /* $sleep(3000); */
  setvbuf(stdout, NULL, _IOFBF, 16384);
  pthread_create(threadIds, NULL, frame, NULL);
  pthread_create(threadIds + 1, NULL, counter, NULL);

  while (!terminate) {
    $sleep(1000);
  }

  // Optionally join threads here if needed
  pthread_join(threadIds[0], NULL);
  pthread_join(threadIds[1], NULL);

  pthread_mutex_destroy(&lock);
  WFPRINT(L"\nThreads joined, everything done mayhaps\n");
  pthread_exit(NULL);
  return (0);
}
