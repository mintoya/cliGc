#include "template.h"
#include "../layers/leyers2.h"
#include "../list/list.h"
typedef enum { DEFINED, DYNAMIC } DomDynamicType;
typedef union {
  float defined;
  int dynamic;
} DynamicData;
typedef struct Number {
  DynamicData self;
  DomDynamicType currentType;
} Number;
typedef struct Node {
  Box Self;
  List *Children;
  Number width;
  Number heith;
  Number minwidth;
  Number minheith;
  Number colth;
  Number rowth;
} Node;
