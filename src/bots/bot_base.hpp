#pragma once

#include <algorithm>
#include <ctime>
#include <string>
#include <sys/time.h>

#include "util/const.hpp"
#include "util/math.hpp"
#include "game/board.hpp"

struct bot_base{

  class stat_t{
    timeval start_time,stop_time;
    unsigned long long nodes;
    
  public:
    stat_t();
    void start_timer();
    void stop_timer();
    double get_seconds() const;
    unsigned long long get_nodes_per_second() const;
    unsigned long long get_nodes() const;
    void inc_nodes();
  };
  
  std::string name;
  int search_depth,perfect_depth;
  stat_t stats;
  
  
  
  /// ctor
  bot_base(int _search_depth,int _perfect_depth);
  
  /// dtor
  virtual ~bot_base();

  /// calculate best move of b and put it in res
  virtual void do_move(const board* in,board* out);
  
  virtual void disable_shell_output() = 0;
  virtual void disable_book() = 0;
  virtual void on_new_game() = 0                        ;
  
  int get_search_depth() const;
  int get_perfect_depth() const;
  
  void set_search_depth(int _search_depth,int _perfect_depth);
  
};

inline bot_base::~bot_base()
{}


inline bot_base::bot_base(int _search_depth,int _perfect_depth):
  name("bot_base")
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

inline void bot_base::stat_t::start_timer()
{
  nodes = 0ull;
  gettimeofday(&start_time,NULL);
}


inline void bot_base::stat_t::stop_timer()
{
  gettimeofday(&stop_time,NULL);
}

inline double bot_base::stat_t::get_seconds() const
{
  return (stop_time.tv_sec + (stop_time.tv_usec / 1000000.0)) - 
  (start_time.tv_sec + (start_time.tv_usec / 1000000.0)); 
}

inline unsigned long long bot_base::stat_t::get_nodes_per_second() const
{
  if(get_seconds() < 0.00000001){
    return 0ull;
  }
  return (unsigned long long)(nodes / get_seconds());
}

inline bot_base::stat_t::stat_t()
{
  nodes = 0;
}

inline long long unsigned int bot_base::stat_t::get_nodes() const
{
  return nodes;
}

inline void bot_base::stat_t::inc_nodes()
{
  nodes++;
}



