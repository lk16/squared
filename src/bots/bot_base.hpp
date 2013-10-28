#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <string>
#include <sys/time.h>

#include "game/util.hpp"
#include "game/board.hpp"

class bot_base{

protected:
  std::string name;
  int search_depth,perfect_depth;
  int my_color;
  long long nodes;
  double prev_move_time;
 
public:
  /// constructor
  bot_base(int _c, int sd,int pd);
  
  /// calculate best move of b and put it in res
  virtual void do_move(const board* b,board* res);
  
  int get_search_depth() const;
  int get_nodes() const;
  int get_color() const;
  
};



inline bot_base::bot_base(int _c, int sd,int pd):
  name("bot_base"),
  search_depth(sd),
  perfect_depth(pd),
  my_color(_c),
  prev_move_time(std::time(NULL))
{
  assert(my_color==1 || my_color==-1);
}

inline int bot_base::get_search_depth() const
{
  return search_depth; 
}

inline int bot_base::get_nodes() const
{
  return nodes; 
}

inline int bot_base::get_color() const
{
  return my_color;
}

inline void bot_base::do_move(const board* b,board* res){
  // prevent compiler complaints
  (void) b;
  (void) res;
  CRASH;
}

#endif
