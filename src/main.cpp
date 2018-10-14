/**
 * FloodIt Solver MCTS
 * main.cpp
 * Purpose: Main program.
 *
 * @author Bruno Freitas Tissei
 * @version 1.0
 */
#include <vector>
#include <iostream>

#include "builder.h"
#include "board.h"
#include "types.h"
#include "monte_carlo.h"

using namespace std;


class Solver {

private:

  int n, m, c;

public:

  void read_input() {
    cin >> n >> m >> c;
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
    vector<int> solution = mcts.run(35000, 4, 53);

    // Print solution
    cout << solution.size() << ende;
    for (auto i : solution)
      cout << i << " ";
    cout << ende;
  }

};


int main() {
  Solver solver;
  solver.run();

  return 0;
}
