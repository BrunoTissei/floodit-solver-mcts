/**
 * FloodIt Solver MCTS
 * builder.h
 * Purpose: Builds graph from board.
 *
 * @author Bruno Freitas Tissei
 * @version 1.0
 */
#pragma once

#include <map>
#include <set>
#include <vector>

#include "types.h"
#include "board.h"
#include "graph.h"

using namespace std;

class Builder {

private:

  Board *board;
  int num_groups;

  vector<tuple2> groups;
  set<tuple2> neighbors;
  map<int, set<tuple2>> nmap;

  /**
   * Recusively goes through every tile in a group, while populating the vector
   * 'group' with the coordinates of the found tiles and 'neighbors' set with
   * the coordinates of the group's border tiles (neighbor tiles).
   *
   * @param (x, y) coordinates of tile in the current group.
   * @param color color of every tile in the group.
   * @return area of group (i.e. number of tiles).
   */
  int get_group(int x, int y, int color);

public:

  Builder(Board *board);

  /**
   * Builds graph where each vertex is a group of tiles of the same color
   * in the initial board.
   *
   * @return the graph where each vertex is a group of adjacent tiles of
   * the same color in the board.
   */
  Graph build_graph();

};
