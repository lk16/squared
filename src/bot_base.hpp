#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <list>
#include <vector>
#include <sys/time.h>

#include "shared.hpp"
#include "board.hpp"

class bot_base{
  
  int max_depth,max_endgame_depth,look_ahead;
  
  /// overridable function that should return positive heuristic if the move is better for color c
  virtual int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining,board* stability);
  
  
  void sort_boards(board* moves,int move_count,int depth);
  
public:
  color c;
  
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// calculate best move of b and put it in res
  void do_move(const board* b,board* res);
  
  /// for timing purposes
  long long nodes;
  int prev_move_time;  
  
};

#endif