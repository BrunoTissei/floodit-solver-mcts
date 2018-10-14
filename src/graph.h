/**
 * FloodIt Solver MCTS
 * graph.h
 * Purpose: Generic graph.
 *
 * @author Bruno Freitas Tissei
 * @version 1.0
 */
#pragma once

#include <vector>

#include "types.h"

using namespace std;


/**
 * Vertex structure containing necessary metadata for MCTS
 */
struct vertex {
  int id, color, area, marker;
  vector<int> neighbors;

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

  vector<int>::iterator begin() {
    return neighbors.begin();
  }

  vector<int>::iterator end() {
    return neighbors.end();
  }
};


class Graph {

private:

  vector<vertex> vertices;

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
