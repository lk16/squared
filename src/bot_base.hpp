#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <list>
#include <vector>

#include "shared.hpp"
#include "board_with_id.hpp"

class bot_base{
  
  
  color c;
  int max_depth,max_endgame_depth,look_ahead;
  
  /// for timing purposes
  int nodes,start_time;
  
  
  /// overridable function that should return positive heuristic if the move is better for white
  virtual int heuristic(const board* b);
  
  /// calculates the heuristic for b using negamax
  int negamax(const board* b,int alpha,int beta,int depth_remaining);
  
  /// internal negamax function, returns index of best move
  int negamax_internal_loop(std::list<board_with_id>& vec,int moves_remaining);
  
  
  /// recursively adds moves ahead of b to vec,
  /// id represents the first move after the examined field
  void add_moves_ahead(std::list<board_with_id>& vec,unsigned id,const board* b,int moves_remaining);
  
public:
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// function called by other classes to calculate and do best move
  board* do_move(const board* b);
  
};

#endif