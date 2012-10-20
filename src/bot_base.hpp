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
  
  /// for timing purposes
  int nodes,start_time;
  
  
  /// overridable function that should return positive heuristic if the move is better for color c
  virtual int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining);
  
  /// internal alpha beta function, returns index of best move
  int alpha_beta(std::list<board>& vec,int moves_remaining);
  
public:
  color c;
  
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// function called by other classes to calculate and do best move
  board* do_move(const board* b);
  
  
};

#endif