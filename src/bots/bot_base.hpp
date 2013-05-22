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

protected:
  int max_depth,max_endgame_depth;
 
  /// higher return value indicates a game state is better for WHITE
  virtual int heuristic(const board* b);
  
public:
  /// TODO make private
  color c;
  
  int get_max_depth() const;
  
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// calculate best move of b and put it in res
  virtual void do_move(const board* b,board* res);
  
  /// for timing purposes; TODO, make private
  long long nodes;
  double prev_move_time;
};

#endif