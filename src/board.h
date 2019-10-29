/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#pragma once

#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>

#include "types.h"
#include "graph.h"

class Board {

private:

  int turn;
  Graph graph;
  bool first_move;

  std::vector<int> next_moves;
  const std::vector<int> full_dx = {0, 1,  0, -1, 1,  1, -1, -1};
  const std::vector<int> full_dy = {1, 0, -1,  0, 1, -1, -1,  1};

public:

  int n, m, c;

  std::vector<int> dx, dy;
  std::vector<std::queue<int>> frontier;
  matrix<int> group_map, board_map;


  /**
   * Initializes board and define neighborhood.
   *
   * @param (n, m) size of the board.
   * @param c number of colors
   * @param all_neighbors whether to use 4 (false) or 8 (true) neighborhood.
   */
  Board(int n, int m, int c, bool all_neighbors);

  /**
   * Associates graph built by Builder to board (the graph is a different
   * representation, other than a matrix, to the same board).
   *
   * @param graph the graph where each vertex is a group of adjacent tiles of
   * the same color in the board.
   */
  void set_graph(Graph graph);

  /**
   * Reads only the board itself (matrix of colors).
   */
  void read_input();

  /**
   * Resets board's internal state.
   */
  void reset();

  /**
   * Gets possible colors to choose as the first move (i.e. colors adjacent to
   * the corner tile).
   *
   * @return available actions (colors), where first is the area that the color
   * yields and second is the color itself.
   */
  std::vector<tuple2> get_first_actions();

  /**
   * Allows board[i] to return board_map[i] (better readability).
   *
   * @param i index of board_map.
   * @return board_map[i].
   */
  std::vector<int> &operator[](int i);

  /**
   * Applies a movement (color) and returns every possible movement i where
   * i.second is the movement itself (color) and i.first is the area that the
   * movement yields.
   *
   * This method works by applying a BFS single step; multiple queues are used,
   * each one stores vertices of different colors and a BFS step uses the
   * corresponding color's queue, while filling the other queues with the new
   * frontier (i.e. neighbors of explored vertices).
   *
   * @param color movement (color) to be applied to the board.
   * @return available actions (colors) after move was made, where first is the
   * area that the color yields and second is the color itself.
   */
  std::vector<tuple2> apply_color(int color);

};
