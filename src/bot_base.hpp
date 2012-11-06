#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <list>
#include <vector>

#include "shared.hpp"
#include "board.hpp"

class bot_base{
  
  int max_depth,max_endgame_depth,look_ahead;
  
  /// overridable function that should return positive heuristic if the move is better for color c
  virtual int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining);
  
  /// internal alpha beta function, returns index of best move
  int alpha_beta(std::list<board>& vec,int moves_remaining);
  
  void sort_boards(board* moves,int move_count,int depth);
  
public:
  color c;
  
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// calculate and do best move
  board* do_move(const board* b);
  
  /// for timing purposes
  int nodes,prev_move_time;  
};

#endif