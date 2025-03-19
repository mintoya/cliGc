#ifndef TEMPLATE_H
#define TEMPLATE_H
#include "../layers/leyers2.h"
#include "../list/list.h"

#include <sys/types.h>
typedef struct {
  int numerator;
  int denominator;
} Rational;
typedef struct Number {
  Rational n;
  char isDefined; // version of the % in css
} Number;
typedef struct Offset {
  Number dcol, drow;
} Offset;
typedef struct definedOffset {
  int dcol, drow;
} definedOffset ;
typedef struct Node {
  Offset position;
  Offset size;
  definedOffset recentPosition;
  definedOffset recentSize;
  rgbColor color;
  List* children;
  Box self;
} Node;

List *renderNodes(Node *start) ;
#endif // !TEMPLATE_H
