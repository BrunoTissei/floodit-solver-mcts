/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#include "builder.h"


// Initializes builder.
Builder::Builder(Board *board) : board(board) {
  num_groups = 1;
}


// Recusively goes through every tile in a group, while populating the vector
// group with the coordinates of the found tiles and neighbors set with the
// coordinates of the group's border tiles (neighbor tiles).
int Builder::get_group(int x, int y, int color) {

  // Mark group map with the group's id in the corresponding cell
  board->group_map[x][y] = num_groups;
  groups.push_back(tuple2(x, y));

  int ans = 0;
  for (uint it = 0; it < board->dx.size(); ++it) {
    int i = board->dx[it], j = board->dy[it];

    // Check if it is inside the board
    if (x + i >= 0 and x + i < board->n and y + j >= 0 and y + j < board->m) {

      // Check if it belongs to this group, if yes, follows recursively
      if (!board->group_map[x + i][y + j] and (*board)[x + i][y + j] == color)
        ans += get_group(x + i, y + j, color) + 1;

      // If it's out of the group then it's a neighbor
      else if ((*board)[x + i][y + j] != color)
        neighbors.insert(tuple2(x + i, y + j));
    }
  }

  // Return area computed so far
  return ans;
}


// Builds graph where each vertex is a group of tiles of the same color
// in the initial board.
Graph Builder::build_graph() {
  Graph graph;

  // Look for tiles that don't belong to any groups yet
  for (int i = 0; i < board->n; ++i) {
    for (int j = 0; j < board->m; ++j) {
      if (!board->group_map[i][j]) {
        groups.clear();
        neighbors.clear();

        // Fill group and neighbors with the group data starting on (i,j)
        int area = get_group(i, j, (*board)[i][j]) + 1;

        graph.add_vertex(vertex(num_groups - 1, (*board)[i][j], area));
        nmap[num_groups++] = neighbors;
      }
    }
  }

  // Build graph
  for (int i = 1; i <= num_groups; ++i) {
    std::set<int> adj;

    // Get ids from neighbors
    for (auto j : nmap[i])
      adj.insert(board->group_map[j.first][j.second]);

    // Add them to the i-th's adjacency list
    for (auto j : adj)
      graph[i - 1].add_neighbor(j - 1);
  }

  nmap.clear();
  return graph;
}
