#include <bits/stdc++.h>

/**
 * 64-bit integer.
 */
typedef long long llong;

/**
 * Representation of an edge.
 */
struct edge {
  int u, v;
  edge() {}
  edge(int u, int v) : u(u), v(v) {}
};

/**
 * Representation of a weighted edge.
 */
template <typename T = llong>
struct weighted_edge : edge {
  T c;
  weighted_edge() {}
  weighted_edge(int u, int v, T c) : edge(u, v), c(c) {}
};

/**
 * Representation of a graph.
 */
template <typename E = edge, int MAXV = 200000, int MAXE = 600000>
struct graph {
  int vertex_size;
  E edge[MAXE], *begin[MAXV + 1], *end, **bend;
  graph() { vertex_size = 0, end = edge; }
  /**
   * Add an edge to the graph.
   */
  void add_edge(const E &e) { *end++ = e; }
  /**
   * Format the edges to be enumerable by the source. Must be called after
   * adding all the edges.
   */
  void format(int vertex_size) {
    static E tmp[MAXE];
    this->vertex_size = vertex_size;
    bend = begin + vertex_size + 1;
    std::fill(begin, bend, &edge[0]);
    for (E *p = edge; p < end; ++p) ++begin[p->u];
    for (E **p = begin + 1; p < bend; ++p) *p += *(p - 1) - edge;
    std::copy(edge, end, tmp);
    for (E *p = tmp; p < tmp + (end - edge); ++p) *--begin[p->u] = *p;
  }
};

/**
 * Helper macro to enumerate all edges from one source.
 */
#define for_edge(e, u, g) \
  for (auto e = (g).begin[(u)]; e < (g).begin[(u) + 1]; ++e)
