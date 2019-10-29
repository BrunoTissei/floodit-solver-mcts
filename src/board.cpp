/**
* Copyright (c) 2018 Bruno Freitas Tissei
*
* Distributed under the MIT software licenser. For the full copyright and
* license information, please view the LICENSE file distributed with this
* source code. 
*/

#include "board.h"


// Initializes board and define neighborhood.
Board::Board(int n, int m, int c, bool all_neighbors) : n(n), m(m), c(c) {
  board_map = matrix<int>(n, std::vector<int>(m));
  group_map = matrix<int>(n, std::vector<int>(m));

  next_moves = std::vector<int>(c + 1);
  frontier = std::vector<std::queue<int>>(c + 1, std::queue<int>());

  turn = 1;

  // if all_neighbors is true, then the diagonals are included
  if (all_neighbors) {
    dx = full_dx;
    dy = full_dy;

  // Otherwise the dx and dy vectors are cut by half (testing purposes only)
  } else {
    dx = std::vector<int>(full_dx.begin(), full_dx.begin() + 4);
    dy = std::vector<int>(full_dy.begin(), full_dy.begin() + 4);
  }
}


// Associates graph built by Builder to board (the graph is a different
// representation, other than a matrix, to the same board).
void Board::set_graph(Graph graph) {
  this->graph = graph;
}


// Reads only the board itself (matrix of colors).
void Board::read_input() {
  for (auto &i : board_map)
    for (auto &j : i)
      std::cin >> j;
}


// Resets board's internal state.
void Board::reset() {

  // next_moves[i] contains area yielded by color i
  std::fill(next_moves.begin(), next_moves.end(), 0);

  // Swap turn from 1 to 2 or 2 to 1, it is used as a marker to avoid exploring
  // vertices that were already explored in this turn
  turn = 3 - turn;

  // Empty frontier
  for (auto &i : frontier)
    while (!i.empty())
      i.pop();

  // Fill frontier based on upper-left group
  graph[0].marker = turn;
  for (auto i : graph[0]) {
    graph[i].marker = turn;
    frontier[graph[i].color].push(i);
    next_moves[graph[i].color] += graph[i].area;
  }
}


// Gets possible colors to choose as the first move (i.e. colors adjacent to
// the corner tile).
std::vector<tuple2> Board::get_first_actions() {
  std::vector<tuple2> actions;

  // First moves are obtained from initial frontier state
  for (int i = 1; i <= c; ++i)
    if (frontier[i].size())
      actions.push_back(tuple2(next_moves[i], i));

  return actions;
}


// Allows board[i] to return board_map[i] (better readability).
std::vector<int> & Board::operator[](int i) {
  return board_map[i];
}


// Applies a movement (color) and returns every possible movement i where
// i.second is the movement itself (color) and i.first is the area that the
// movement yields.
std::vector<tuple2> Board::apply_color(int color) {

  // Apply BFS step to frontier color only
  while (!frontier[color].empty()) {
    int v = frontier[color].front();
    frontier[color].pop();

    // Update next_moves[v.color] to remove expanded group
    next_moves[graph[v].color] -= graph[v].area;

    for (auto i : graph[v])
      if (graph[i].marker != turn) {
        graph[i].marker = turn;

        // Add neighbors to the correspoding queue based on their colors
        frontier[graph[i].color].push(i);

        // Update next_moves to contain area yielded by neighbors
        next_moves[graph[i].color] += graph[i].area;
      }
  }

  // Build sorted vector of movements by the yielded area to be used in
  // the rollout phase
  std::vector<tuple2> next;
  for (int i = 1; i <= c; ++i)
    if (next_moves[i] > 0)
      next.push_back(tuple2(next_moves[i], i));

  std::sort(next.begin(), next.end());

  return next;
}
