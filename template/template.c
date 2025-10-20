#include "template.h"
#include "../tSize/terminal_size.h"
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#define $max(a, b) (a > b) ? (a) : (b)
#define $min(a, b) (a < b) ? (a) : (b)
void solidify(Node parent, Node *n) {
  // Position (columns)
  if (n->position.dcol.isDefined) {
    n->recentPosition.dcol =
        (int)(n->position.dcol.n + parent.recentPosition.dcol);
  } else {
    n->recentPosition.dcol = (int)(parent.recentSize.dcol * n->position.dcol.n +
                                   parent.recentPosition.dcol);
  }

  // Position (rows)
  if (n->position.drow.isDefined) {
    n->recentPosition.drow =
        (int)(n->position.drow.n + parent.recentPosition.drow);
  } else {
    n->recentPosition.drow = (int)(parent.recentSize.drow * n->position.drow.n +
                                   parent.recentPosition.drow);
  }

  // Size (columns)
  if (n->size.dcol.isDefined) {
    n->recentSize.dcol = (int)n->size.dcol.n;
  } else {
    n->recentSize.dcol = (int)(parent.recentSize.dcol * n->size.dcol.n);
  }

  // Size (rows)
  if (n->size.drow.isDefined) {
    n->recentSize.drow = (int)n->size.drow.n;
  } else {
    n->recentSize.drow = (int)(parent.recentSize.drow * n->size.drow.n);
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
      recursiverenderer(usedList, (Node *)List_getRef(start->children, i),
                        start);
    }
  }
  return;
}

List *renderNodes(Node *root) {
  static List *mainList = NULL;
  if (mainList == NULL)
    mainList = List_new(sizeof(List));
  mainList->length = 0;
  recursiverenderer(mainList, root, NULL);
  return mainList;
}
