#include "template.h"
#include "../tSize/terminal_size.h"
#include <string.h>
#include <unistd.h>
#include <wchar.h>
Rational Rational_add(Rational a, Rational b) {
  Rational result;
  if (a.denominator == b.denominator) {
    result.denominator = a.denominator;
    result.numerator = a.numerator + b.numerator;
  } else {
    result.denominator = a.denominator * b.denominator;
    result.numerator =
        a.numerator * b.denominator + b.numerator * a.denominator;
  }
  return result;
}
Rational Rational_subtract(Rational a, Rational b) {
  b.numerator *= -1;
  return Rational_add(a, b);
}
Rational Rational_multiply(Rational a, Rational b) {
  Rational result;
  result.numerator = a.numerator * b.numerator;
  result.denominator = a.denominator * b.denominator;
  return result;
}
Rational Rational_divide(Rational a, Rational b) {
  int temp = b.numerator;
  b.numerator = b.denominator;
  b.denominator = temp;
  return Rational_multiply(a, b);
}
int Rational_toInt(Rational a) { return a.numerator / a.denominator; }
void solidify(Node parent, Node *n) {
  if (n->position.dcol.isDefined) {
    n->recentPosition.dcol =
        Rational_toInt(n->position.dcol.n) + parent.recentPosition.dcol;
  } else {
    n->recentPosition.dcol = parent.recentSize.dcol *
                                 n->position.dcol.n.numerator /
                                 n->position.dcol.n.denominator +
                             parent.recentPosition.dcol;
  }

  if (n->position.drow.isDefined) {
    n->recentPosition.drow =
        Rational_toInt(n->position.drow.n) + parent.recentPosition.drow;
  } else {
    n->recentPosition.drow = parent.recentSize.drow *
                                 n->position.drow.n.numerator /
                                 n->position.drow.n.denominator +
                             parent.recentPosition.drow;
  }
  if (n->size.dcol.isDefined) {
    n->recentSize.dcol = Rational_toInt(n->size.dcol.n);
  } else {
    n->recentSize.dcol = parent.recentSize.dcol * n->size.dcol.n.numerator /
                         n->size.dcol.n.denominator;
  }

  if (n->size.drow.isDefined) {
    n->recentSize.drow = Rational_toInt(n->size.drow.n);
  } else {
    n->recentSize.drow = parent.recentSize.drow * n->size.drow.n.numerator /
                         n->size.drow.n.denominator;
  }
}

void printNode(const Node *node) {
  DEBUG(L"Node: \n");
  DEBUG(L"Children: %p\n", node->children);
  DEBUG(L"Self: %p\n", node->self.lines);
}

static List noline = {
    .head = NULL,
    .length = 0,
    .size = 0,
    .width = 0,
};
List *fromNode(Node *n, Node *parent) {
  static Node rootNode = {
      .recentPosition = {0, 0},
      .recentSize = {0, 0},
  };
  if (!parent) {
    TerminalSize ts = get_terminal_size();
    definedOffset size = {ts.width, ts.height};
    rootNode.recentSize = size;
    parent = &rootNode;
  }
  solidify(*parent, n);
  int col = n->recentPosition.dcol;
  int row = n->recentPosition.drow;
  int dcol = n->recentSize.dcol + n->recentPosition.dcol;
  int drow = n->recentSize.drow + n->recentPosition.drow;
  if (n->self.lines) {
    if (col == n->self.col && row == n->self.row && drow == n->self.brow &&
        dcol == n->self.bcol) {
      return n->self.lines;
    } else {
      Layer_delete(n->self.lines);
      n->self = Box_new(row, col, drow, dcol, n->color, L' ');
    }

  } else {
    n->self = Box_new(row, col, drow, dcol, n->color, L' ');
  }
  return n->self.lines;
}
void recursiverenderer(List *usedList, Node *start, Node *parent) {
  List_append(usedList, fromNode(start, parent));
  if (start->children != NULL) {
    for (int i = 0; i < start->children->length; i++) {
      Node *child = (Node *)List_gst(start->children, i);
      recursiverenderer(usedList, child, start);
    }
  }
  return;
}

List *renderNodes(Node *start) {
  static List *mainList = NULL;
  if (mainList == NULL)
    mainList = List_new(sizeof(List));
  mainList->length = 0;
  recursiverenderer(mainList, start, NULL);
  return mainList;
}
