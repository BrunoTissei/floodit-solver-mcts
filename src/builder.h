/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#pragma once

#include <map>
#include <set>
#include <vector>

#include "types.h"
#include "board.h"
#include "graph.h"

class Builder {

private:

  Board *board;
  int num_groups;

  std::vector<tuple2> groups;
  std::set<tuple2> neighbors;
  std::map<int, std::set<tuple2>> nmap;

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

  /**
   * Initializes builder.
   *
   * @param board board being used.
   */
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
