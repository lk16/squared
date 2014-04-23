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
  long long nodes;
  double prev_move_time;
 
public:
  
  /// ctor
  bot_base(int _search_depth,int _perfect_depth);
  
  /// dtor
  virtual ~bot_base();
  
  /// calculate best move of b and put it in res
  virtual void do_move(const board* in,board* out);
  
  int get_search_depth() const;
  int get_perfect_depth() const;
  int get_nodes() const;
  
  
  void set_search_depth(int _search_depth,int _perfect_depth);
  
};

inline bot_base::~bot_base()
{}


inline bot_base::bot_base(int _search_depth,int _perfect_depth):
  name("bot_base"),
  prev_move_time(std::time(NULL))
{
  set_search_depth(_search_depth,_perfect_depth);
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

inline void bot_base::do_move(const board* b,board* res){
  (void) b;
  (void) res;
  CRASH;
}


inline void bot_base::set_search_depth(int _search_depth, int _perfect_depth)
{
  search_depth = _search_depth;
  perfect_depth = _perfect_depth;
}

