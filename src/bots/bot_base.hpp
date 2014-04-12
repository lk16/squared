#pragma once

#include <algorithm>
#include <ctime>
#include <string>
#include <sys/time.h>

#include "util/const.hpp"
#include "util/math.hpp"
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
  
  virtual ~bot_base();
  
  /// calculate best move of b and put it in res
  virtual void do_move(const board* b,board* res);
  
  int get_search_depth() const;
  int get_perfect_depth() const;
  int get_nodes() const;
  int get_color() const;
  
  
  /// WARNING: this changes the color of this bot
  void swap_color();
  
  void set_search_depth(int sd,int pd);
  
};

inline bot_base::~bot_base()
{}


inline bot_base::bot_base(int _c, int sd,int pd):
  name("bot_base"),
  search_depth(sd),
  perfect_depth(pd),
  my_color(_c),
  prev_move_time(std::time(NULL))
{
  assert(my_color==BLACK || my_color==WHITE || my_color==COLOR_UNDEFINED);
}

inline int bot_base::get_search_depth() const
{
  return search_depth; 
}

inline int bot_base::get_perfect_depth() const
{
  return perfect_depth;
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
  (void) b;
  (void) res;
  CRASH;
}

inline void bot_base::swap_color()
{
  my_color = (my_color == -1 ? 1 : -1);
}

inline void bot_base::set_search_depth(int sd, int psd)
{
  search_depth = sd;
  perfect_depth = psd;
}

