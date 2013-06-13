#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <list>
#include <string>
#include <sys/time.h>

#include "game/util.hpp"
#include "game/board.hpp"

class bot_base{

protected:
  std::string name;
  int search_depth,wdl_depth,perfect_depth;
  color c;
  long long nodes;
  double prev_move_time;
 
public:
  
  /// constructor
  bot_base(color _c, int sd, int wdl, int pd):
    name("bot_base"),
    search_depth(sd),
    wdl_depth(wdl),
    perfect_depth(pd),
    c(_c),
    prev_move_time(std::time(NULL))
  {}
  
  /// calculate best move of b and put it in res
  virtual void do_move(const board* b,board* res){
    // prevent compiler complaints
    b = NULL;
    res = NULL;
    CRASH;
  }
  
  int get_search_depth() const{ return search_depth; }
  int get_nodes()        const{ return nodes; }
  int get_color()        const{ return c; }
  
};

#endif