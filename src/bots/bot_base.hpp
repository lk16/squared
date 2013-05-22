#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <list>
#include <vector>
#include <sys/time.h>

#include "game/util.hpp"
#include "game/board.hpp"

class bot_base{
  
  int max_depth,max_endgame_depth,look_ahead;
 
  
private:

  /// overridable function that should return positive heuristic if the move is better for WHITE
  virtual int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining);
  
  
  void sort_boards(board* moves,int move_count,int depth);  
  
public:
  color c;
  
  int get_max_depth() const;
  
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// calculate best move of b and put it in res
  void do_move(const board* b,board* res);
  
  /// for timing purposes
  long long nodes;
  double prev_move_time;
};

#endif