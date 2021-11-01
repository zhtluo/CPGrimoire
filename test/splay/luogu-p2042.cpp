/** Brief description:
 * Maintain a sequence on 6 operations:
 * 1. INSERT posi tot c1 c2 ... ctot: insert (c1, c2, ..., ctot) to posi.
 * 2. DELETE posi tot: delete tot numbers starting at posi.
 * 3. MAKE-SAME posi tot c: set [posi, posi + tot) to be c.
 * 4. REVERSE posi tot: reverse [posi, posi + tot).
 * 5. GET-SUM posi tot: find the sum of [posi, posi + tot).
 * 6. MAX-SUM: find the maximum subarray.
 */

#include "../../template.h"

const int MAXSIZE = 500000;

struct node : splay::reversible_node<node> {
  llong val, sum, left_max_sum, right_max_sum, max_sum, lazy;
  int is_lazy;
  node(llong val = 0) : splay::reversible_node<node>(), val(val), sum(val) {
    left_max_sum = right_max_sum = val > 0 ? val : 0;
    max_sum = val;
    is_lazy = lazy = 0;
  }
  void set(llong v) {
    is_lazy = 1, lazy = val = v;
    sum = v * size;
    left_max_sum = right_max_sum = v > 0 ? v * size : 0;
    max_sum = val > 0 ? sum : val;
  }
  void reverse() {
    splay::reversible_node<node>::reverse();
    std::swap(left_max_sum, right_max_sum);
  }
  void push_down() {
    splay::reversible_node<node>::push_down();
    if (is_lazy) {
      for (int t = 0; t < 2; ++t)
        if (c[t]) c[t]->set(lazy);
      is_lazy = 0;
    }
  }
  void update() {
    splay::reversible_node<node>::update();
    static node empty = node();
    empty.max_sum = -INFLL;
    node *p[2];
    for (int t = 0; t < 2; ++t) p[t] = c[t] ? c[t] : &empty;
    sum = val + p[0]->sum + p[1]->sum;
    left_max_sum =
        std::max(p[0]->sum + val + p[1]->left_max_sum, p[0]->left_max_sum);
    right_max_sum =
        std::max(p[1]->sum + val + p[0]->right_max_sum, p[1]->right_max_sum);
    max_sum = std::max(std::max(p[0]->max_sum, p[1]->max_sum),
                       p[0]->right_max_sum + val + p[1]->left_max_sum);
  }
};

splay::tree<node, MAXSIZE, allocator::stack<node, MAXSIZE + 2>> t;

void insert(int posi, int tot) {
  static node *addr[MAXSIZE];
  for (int i = 0, val; i < tot; ++i) {
    scanf("%d", &val);
    addr[i] = t.new_node(node(val));
  }
  t.insert_range(addr, tot, posi);
}

int main() {
  int N, M;
  scanf("%d%d", &N, &M);
  insert(0, N);
  for (int q = 0; q < M; ++q) {
    static char s[20];
    scanf(" %s", s);
    if (strcmp(s, "INSERT") == 0) {
      int posi, tot;
      scanf("%d%d", &posi, &tot);
      insert(posi, tot);
    } else if (strcmp(s, "DELETE") == 0) {
      int posi, tot;
      scanf("%d%d", &posi, &tot);
      if (tot > 0) t.erase_range(posi - 1, posi + tot - 1);
    } else if (strcmp(s, "MAKE-SAME") == 0) {
      int posi, tot, c;
      scanf("%d%d%d", &posi, &tot, &c);
      if (tot > 0) t.find_range(posi - 1, posi + tot - 1)->set(c);
    } else if (strcmp(s, "REVERSE") == 0) {
      int posi, tot;
      scanf("%d%d", &posi, &tot);
      if (tot > 0) t.find_range(posi - 1, posi + tot - 1)->reverse();
    } else if (strcmp(s, "GET-SUM") == 0) {
      int posi, tot;
      scanf("%d%d", &posi, &tot);
      if (tot > 0)
        printf("%lld\n", t.find_range(posi - 1, posi + tot - 1)->sum);
      else
        puts("0");
    } else if (strcmp(s, "MAX-SUM") == 0) {
      printf("%lld\n", t.find_range(0, t.size())->max_sum);
    }
  }
}