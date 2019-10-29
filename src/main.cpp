/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#include <vector>
#include <iostream>

#include "builder.h"
#include "board.h"
#include "types.h"
#include "monte_carlo.h"

class Solver {

private:

  int n, m, c;

public:

  void read_input() {
    std::cin >> n >> m >> c;
  }

  void run() {
    read_input();

    // Construct board (false = 4 neighbors, true = 8 neighbors)
    Board board(n, m, c, true);
    board.read_input();

    // Build graph of groups from board
    Builder builder(&board);
    Graph graph = builder.build_graph();
    board.set_graph(graph);

    // Run Monte Carlo Search Tree
    MonteCarloTS mcts(123, &board);
    std::vector<int> solution = mcts.run(35000, 4, 53);

    // Print solution
    std::cout << solution.size() << ende;
    for (auto i : solution)
      std::cout << i << " ";
    std::cout << ende;
  }

};


int main() {
  Solver solver;
  solver.run();

  return 0;
}
