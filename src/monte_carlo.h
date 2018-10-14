/**
 * FloodIt Solver MCTS
 * monte_carlo.h
 * Purpose: Executes Monte Carlo Tree Search.
 *
 * @author Bruno Freitas Tissei
 * @version 1.0
 */
#pragma once

#include <cmath>
#include <utility>
#include <algorithm>

#include "graph.h"
#include "board.h"
#include "builder.h"
#include "types.h"

using namespace std;


class State {

private:

  Board *board;
  int num_moves;

public:

  vector<int> backup;
  vector<tuple2> actions;


  State(Board *board);

  /**
   * Applies movement.
   *
   * @param color movement to be applied.
   */
  void apply_move(int color);

  /**
   * Applies random movements until board is complete.
   */
  void rollout();

  /**
   * Resets state, board and backup.
   */
  void reset();

  /**
   * Gets score obtained by sequence of movements taken by state.
   *
   * @param bound maximum possible number of movements to solve this board.
   * @return score given by (bound - num_moves) (i.e. inverse of num_moves).
   */
  double get_result(double bound);

};


class Node {

private:

  double C, D;
  double points, visits, sq_points;

  /**
   * Calculates UCT (Upper Confidence Bound 1 applied to trees) of a child node.
   *
   * @param child child node used to calculate UCT.
   * @return UCT value of child.
   */
  double calc_uct(const Node *child);

public:

  tuple2 move;

  Node *parent;
  vector<Node*> children;
  vector<tuple2> remaining_actions;


  /**
   * Creates new node and builds list of untried moves.
   *
   * @param move movement that generated this node.
   * @param state state of the game when node was created.
   * @param parent parent node.
   * @param (C, D) constants for UCT.
   */
  Node(tuple2 move, const State &state, Node *parent, double C, double D);

  /**
   * Creates new node, adds it to children vector and returns it.
   *
   * @param move_pos index of remaining_actions indicating movement that
   * generated new child.
   * @param state state of the game when child node was created.
   * @return newly created child node.
   */
  Node *add_child(int move_pos, State &state);

  /**
   * Gets child with the greatest UCT value.
   *
   * @return child with the greates UCT value.
   */
  Node *uct_child();

  /**
   * Updates node's statistics (visits, points and sum of squared points).
   *
   * @param result score obtained in rollout.
   */
  void update(double result);

};


class MonteCarloTS {

private:

  Board *board;
  double moves_upper;

public:

  /**
   * Specifies random seed and associates board to be used by state.
   *
   * @param seed random seed.
   * @param board board used in the puzzle.
   */
  MonteCarloTS(int seed, Board *board);


  /**
   * Applies Monte Carlo Tree Search.
   *
   * @param num_iter number of iterations.
   * @param (C, D) constants for UCT.
   * @return result (i.e. sequence of movements to solve game).
   */
  vector<int> run(int num_iter, double C, double D);

};
