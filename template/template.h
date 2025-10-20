#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "../layers/leyers2.h"
#include "../my-list/my-list.h"

#include <stdint.h>
#include <sys/types.h>
typedef struct Number {
  float n;
  uint8_t isDefined;
} Number;
typedef struct Offset {
  Number dcol, drow;
} Offset;
typedef struct definedOffset {
  int dcol, drow;
} definedOffset;

typedef struct Node {
  Offset position;
  Offset size;
  definedOffset recentPosition;
  definedOffset recentSize;
  rgbColor color;
  List *children;
  Box self;
} Node;

List *renderNodes(Node *start);
#endif // !TEMPLATE_H
