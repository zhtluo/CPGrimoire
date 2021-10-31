#include "../template.h"

struct node : splay::reversible_node<node> {
  int val;
  void push_down() { splay::reversible_node<node>::push_down(); }
  void update() { splay::reversible_node<node>::update(); }
};

splay::tree<node> t;

int N, M;

void inorder(node *n) {
  static int f = 0;
  if (!n) return;
  n->push_down();
  inorder(n->c[0]);
  if (n->val) {
    if (f) printf(" ");
    f = 1;
    printf("%d", n->val);
  }
  inorder(n->c[1]);
}

int main() {
  scanf("%d%d", &N, &M);
  for (int i = 0; i < N; ++i) {
    node *n = t.new_node();
    n->val = i + 1;
    t.insert(n, i);
  }
  for (int i = 0, u, v; i < M; ++i) {
    scanf("%d%d", &u, &v);
    node *n = t.find_range(u - 1, v);
    n->reverse();
  }
  inorder(t.root);
}