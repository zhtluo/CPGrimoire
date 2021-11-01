#include <bits/stdc++.h>

/**
 * 64-bit integer.
 */
typedef long long llong;

/**
 * Infinity for int.
 */
const int INF = 1E9;

/**
 * Infinity for llong.
 */
const llong INFLL = 2E18;

namespace allocator {

/**
 * Array allocator.
 */
template <typename T, int MAXSIZE>
struct array {
  T v[MAXSIZE], *top;
  array() : top(v) {}
  T *alloc(const T &val = T()) { return &(*top++ = val); }
  void dealloc(T *p) {}
};

/**
 * Stack-based array allocator.
 */
template <typename T, int MAXSIZE>
struct stack {
  T v[MAXSIZE];
  T *spot[MAXSIZE], **top;
  stack() {
    for (int i = 0; i < MAXSIZE; ++i) spot[i] = v + i;
    top = spot + MAXSIZE;
  }
  T *alloc(const T &val = T()) { return &(**--top = val); }
  void dealloc(T *p) { *top++ = p; }
};

}  // namespace allocator

namespace graph {

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

}  // namespace graph

namespace splay {

/**
 * Abstract node struct.
 */
template <typename T>
struct node {
  T *f, *c[2];
  int size;
  node() {
    f = c[0] = c[1] = nullptr;
    size = 1;
  }
  void push_down() {}
  void update() {
    size = 1;
    for (int t = 0; t < 2; ++t)
      if (c[t]) size += c[t]->size;
  }
};

/**
 * Abstract reversible node struct.
 */
template <typename T>
struct reversible_node : node<T> {
  int r;
  reversible_node() : node<T>() { r = 0; }
  void push_down() {
    node<T>::push_down();
    if (r) {
      for (int t = 0; t < 2; ++t)
        if (node<T>::c[t]) node<T>::c[t]->reverse();
      r = 0;
    }
  }
  void update() { node<T>::update(); }
  /**
   * Reverse the range of this node.
   */
  void reverse() {
    std::swap(node<T>::c[0], node<T>::c[1]);
    r = r ^ 1;
  }
};

template <typename T, int MAXSIZE = 500000,
          typename alloc = allocator::array<T, MAXSIZE + 2>>
struct tree {
  alloc pool;
  T *root;
  /**
   * Get a new node from the pool.
   */
  T *new_node(const T &val = T()) { return pool.alloc(val); }
  tree() {
    root = new_node(), root->c[1] = new_node(), root->size = 2;
    root->c[1]->f = root;
  }
  /**
   * Helper function to rotate node.
   */
  void rotate(T *n) {
    int v = n->f->c[0] == n;
    T *p = n->f, *m = n->c[v];
    if (p->f) p->f->c[p->f->c[1] == p] = n;
    n->f = p->f, n->c[v] = p;
    p->f = n, p->c[v ^ 1] = m;
    if (m) m->f = p;
    p->update(), n->update();
  }
  /**
   * Splay n so that it is under s (or to root if s is null).
   */
  void splay(T *n, T *s = nullptr) {
    while (n->f != s) {
      T *m = n->f, *l = m->f;
      if (l == s)
        rotate(n);
      else if ((l->c[0] == m) == (m->c[0] == n))
        rotate(m), rotate(n);
      else
        rotate(n), rotate(n);
    }
    if (!s) root = n;
  }
  /**
   * Get the size of the tree.
   */
  int size() { return root->size - 2; }
  /**
   * Helper function to walk down the tree.
   */
  int walk(T *n, int &v, int &pos) {
    n->push_down();
    int s = n->c[0] ? n->c[0]->size : 0;
    (v = s < pos) && (pos -= s + 1);
    return s;
  }
  /**
   * Insert node n to position pos.
   */
  void insert(T *n, int pos) {
    T *c = root;
    int v;
    ++pos;
    while (walk(c, v, pos), c->c[v] && (c = c->c[v]))
      ;
    c->c[v] = n, n->f = c, splay(n);
  }
  /**
   * Find the node at position pos. If sp is true, splay it.
   */
  T *find(int pos, int sp = true) {
    T *c = root;
    int v;
    ++pos;
    while ((pos < walk(c, v, pos) || v) && (c = c->c[v]))
      ;
    if (sp) splay(c);
    return c;
  }
  /**
   * Find the range [posl, posr) on the splay tree.
   */
  T *find_range(int posl, int posr) {
    T *r = find(posr), *l = find(posl - 1, false);
    splay(l, r);
    if (l->c[1]) l->c[1]->push_down();
    return l->c[1];
  }
  /**
   * Insert nn of size nn_size to position pos.
   */
  void insert_range(T **nn, int nn_size, int pos) {
    T *r = find(pos), *l = find(pos - 1, false), *c = l;
    splay(l, r);
    for (int i = 0; i < nn_size; ++i) c->c[1] = nn[i], nn[i]->f = c, c = nn[i];
    for (int i = nn_size - 1; i >= 0; --i) nn[i]->update();
    l->update(), r->update(), splay(nn[nn_size - 1]);
  }
  /**
   * Helper function to dealloc a subtree.
   */
  void dealloc(T *n) {
    if (!n) return;
    dealloc(n->c[0]);
    dealloc(n->c[1]);
    pool.dealloc(n);
  }
  /**
   * Remove from position [posl, posr).
   */
  void erase_range(int posl, int posr) {
    T *n = find_range(posl, posr);
    n->f->c[1] = nullptr, n->f->update(), n->f->f->update(), n->f = nullptr;
    dealloc(n);
  }
};

}  // namespace splay