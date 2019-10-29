/**
 * Copyright (c) 2018 Bruno Freitas Tissei
 *
 * Distributed under the MIT software licenser. For the full copyright and
 * license information, please view the LICENSE file distributed with this
 * source code. 
 */

#include "monte_carlo.h"

State::State(Board *board) : board(board) {
  reset();
}

// Applies movement.
void State::apply_move(int color) {
  num_moves++;
  backup.push_back(color);
  actions = board->apply_color(color);
}

// Applies random movements until board is complete.
void State::rollout() {

  // Apply random movements until board is complete
  while (actions.size() > 0) {

    // Generate random movement with greater probability to colors that
    // yields a greater area
    int sum = 0;
    for (auto i : actions)
      sum += i.first;

    int move = 0;
    int move_cnt = rand() % sum;

    for (auto i : actions) {
      move_cnt -= i.first;
      move = i.second;

      if (move_cnt <= 0)
        break;
    }

    // Apply randomly selected movement
    apply_move(move);
  }
}

// Resets state, board and backup.
void State::reset() {
  num_moves = 0;
  board->reset();
  backup.clear();
  actions = board->get_first_actions();
}

// Gets score obtained by sequence of movements taken by state.
double State::get_result(double bound) {

  // The score is bound - num_moves, that way the score is inversely
  // proportional to number of movements, resulting in a minimized number
  // of movements given by a greater score
  return (bound - num_moves);
}


// Creates new node and builds list of untried moves.
Node::Node(tuple2 move, const State &state, Node *parent, double C, double D) {
  this->move = move;
  this->parent = parent;
  this->points = this->visits = this->sq_points = 0.0;
  this->C = C;
  this->D = D;

  this->remaining_actions = state.actions;
}

// Creates new node, adds it to children vector and returns it.
Node *Node::add_child(int move_pos, State &state) {
  Node *n = new Node(remaining_actions[move_pos], state, this, C, D);

  // Fast vector element erasing, since order does not matter
  swap(remaining_actions[move_pos],
       remaining_actions[remaining_actions.size() - 1]);

  remaining_actions.erase(--(remaining_actions.end()));

  children.push_back(n);
  return n;
}

// Calculates UCT (Upper Confidence Bound 1 applied to trees) of a child node.
double Node::calc_uct(const Node *child) {

  // Control exploitation
  double fi = child->points / child->visits;

  // Control exploration
  double se = C * sqrt(log(this->visits) / child->visits);

  // Third term of UCT proposed by Schadd et al. for single player MCTS
  double th = sqrt((child->sq_points - child->visits * fi * fi + D) / child->visits);

  return fi + se + th;
}

// Gets child with the greatest UCT value.
Node *Node::uct_child() {
  return *(max_element(children.begin(), children.end(),
    [this](const Node *a, const Node *b) {
      return calc_uct(a) < calc_uct(b);
    }
  ));
}

// Updates node's statistics (visits, points and sum of squared points).
void Node::update(double result) {
  this->visits++;
  this->points += result;

  // Sum of squared points to be used by third term of UCT
  this->sq_points += result * result;
}


// Specifies random seed and associates board to be used by state.
MonteCarloTS::MonteCarloTS(int seed, Board *board) : board(board) {
  srand(seed);

  // Calculate maximum number of movements needed (i.e. upper bound) - Clifford et al.
  int N = std::max(board->n, board->m);
  this->moves_upper = (2*N + sqrt(2 * board->c) * N + board->c);
}

// Applies Monte Carlo Tree Search.
std::vector<int> MonteCarloTS::run(int num_iter, double C, double D) {
  State state(board);

  std::vector<int> best_backup;
  Node *root = new Node(tuple2(-1, -1), state, nullptr, C, D);

  for (int iter = 0; iter < num_iter; ++iter) {
    Node *node = root;

    // Select
    while (node->remaining_actions.size() == 0 and node->children.size() != 0) {
      node = node->uct_child();
      state.apply_move(node->move.second);
    }

    // Expand
    if (node->remaining_actions.size() != 0) {
      int move_pos = rand() % node->remaining_actions.size();
      int move = node->remaining_actions[move_pos].second;

      state.apply_move(move);
      node = node->add_child(move_pos, state);
    }

    // Rollout
    state.rollout();

    // Backpropagate
    while (node != nullptr) {
      node->update(state.get_result(moves_upper));
      node = node->parent;
    }

    // Use state's best rollout result as solution
    if (best_backup.size() == 0 or state.backup.size() < best_backup.size())
      best_backup = state.backup;

    state.reset();
  }

  return best_backup;
}
