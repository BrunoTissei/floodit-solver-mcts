/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#pragma once

#include <vector>

#include "types.h"

/**
 * Vertex structure containing necessary metadata for MCTS
 */
struct vertex {
  int id, color, area, marker;
  std::vector<int> neighbors;

  vertex(int i, int c, int a) : id(i), color(c), area(a), marker(0) {}

  ~vertex() {
    neighbors.clear();
  }

  void add_neighbor(int i) {
    neighbors.push_back(i);
  }

  int &operator[](int i) {
    return neighbors[i];
  }

  std::vector<int>::iterator begin() {
    return neighbors.begin();
  }

  std::vector<int>::iterator end() {
    return neighbors.end();
  }
};


class Graph {

private:
  std::vector<vertex> vertices;

public:
  Graph();

  ~Graph();

  /**
   * Adds edge between v-th and u-th vertex
   *
   * @param v origin vertex
   * @param u destin vertex
   */
  void add_edge(int v, int u);

  /**
   * Adds vertex to vertices vector
   *
   * @param v new vertex
   */
  void add_vertex(vertex v);

  /**
   * Allows graph[i] to return vertices[i] (better readability).
   *
   * @param i index of vertices.
   * @return vectices[i].
   */
  vertex &operator[](int i);
};
