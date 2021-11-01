#include "../template.h"

graph::graph<> g;

int main() {
  for (int i = 0; i < 20; ++i)
    g.add_edge(graph::edge(rand() % 10, rand() % 10));
  for (int i = 0; i < 20; ++i) printf("%d %d\n", g.edge[i].u, g.edge[i].v);
  puts("");
  g.format(10);
  for (int i = 0; i < 10; ++i) {
    printf("%d\n", i);
    for_edge(e, i, g) printf("%d %d\n", e->u, e->v);
  }
}