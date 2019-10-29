/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#include "graph.h"

Graph::Graph() {}

Graph::~Graph() {
  vertices.clear();
}

// Adds edge between v-th and u-th vertex
void Graph::add_edge(int v, int u) {
  vertices[v].add_neighbor(u);
}

// Adds vertex to vertices vector
void Graph::add_vertex(vertex v) {
  vertices.push_back(v);
}

// Allows graph[i] to return vertices[i] (better readability).
vertex & Graph::operator[](int i) {
  return vertices[i];
}
