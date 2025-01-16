#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#define FAIL exit(1)*/
// not using stdlib for now
#define FAIL printf("FAIL was supposed to be called here");

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define RESET "\x1B[0m"

List *List_new(unsigned long bytes) {
  List *l = calloc(1, sizeof(List));
  l->width = bytes;
  l->length = 0;
  l->head = calloc(1, bytes);
  l->size = 1;
  return l;
}
void List_delete(List *l) {
  free(l->head);
  l->head = NULL;
  free(l);
}
void errPrint(char *message) {
  // just prints stuff in red and exits out
  printf(KRED "%s" RESET, message);
  FAIL;
}
void warnPrint(char *message) {
  // prints things in orange
  printf(KYEL "%s" RESET, message);
}
void List_resize(List *l, unsigned int newSize) {
  /*char buffer[60] = {0};*/
  /*sprintf(buffer,"List_resize(%u)\n",newSize);*/
  /*warnPrint(buffer);*/

  if (newSize < 1) {
    List_resize(l, 1);
    return;
  }

  void *newPlace = realloc(l->head, newSize * l->width);
  if (newPlace == NULL) {
    free(l->head);
    errPrint("ran out of space for some realloc, in List_resize\n");
  }
  l->head = newPlace;
  l->size = newSize;
  l->length = (l->length < l->size) ? (l->length) : (l->size);
}
int checkBounds(List *l, unsigned int i) {
  /*printf("chekcing %i, length: %i size: %i\n",i,l->length,l->size);*/
  int inBounds = 1;
  if (i >= l->length) {
    if (i > l->size) {
      errPrint("Out of bounds\n");
    } else {
      warnPrint("Out of range\n");
      inBounds = 0;
    }
  }
  return inBounds;
}
void List_set(List *l, unsigned int i, const void *element) {
  checkBounds(l, i);
  memcpy(l->head + i * l->size, element, l->width);
}
void *List_gst(List *l, unsigned int i) {
  checkBounds(l, i);
  return l->head + i * l->width;
}
void List_append(List *l, void *element) {
  if (l->length + 1 >= l->size) {
    List_resize(l, l->length * 2);
  }
  memcpy(l->head + l->width * l->length, element, l->width);
  l->length++;
}
void List_remove(List *l, unsigned int i) {
  if (checkBounds(l, i)) {
    memcpy(l->head + i * l->width, l->head + (i + 1) * l->width,
           (l->length - i) * l->width);
    l->length--;
  }
}
void List_insert(List *l, unsigned int i, void *element) {
  if (i == l->length) {
    List_append(l, element);
    return;
  }
  if (l->size <= l->length + 1) {
    List_resize(l, l->length + 1);
  }
  l->length++;
  memcpy(l->head + (i + 1) * l->width, l->head + i * l->width,
         l->width * (l->length - i));
  List_set(l, i, element); // seg fault
}
void List_print_as_string(List *l) {
  for (unsigned int i = 0; i < l->length; i++) {
    char c = *(char *)List_gst(l, i); // Access each char
    printf("%c", c);                  // Print the character
  }
  printf("\n");
}
int List_forEach(List *l, int (*function)(void *)) {
  int result = 1;
  for (int i = 0; i < l->length; i++) {
    result = result && function(List_gst(l, i));
  }
  return result;
}
