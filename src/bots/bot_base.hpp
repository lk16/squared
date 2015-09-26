#pragma once

#include <algorithm>
#include <ctime>
#include <string>
#include <sys/time.h>

#include "bots/bot_register.hpp"
#include "game/board.hpp"
#include "util/const.hpp"
#include "util/math.hpp"
#include "util/dummystream.hpp"
#include "util/string.hpp"

class bot_base{


  
  std::string name;
  int search_depth;
  int perfect_depth;
  std::ostream* output_stream;
  
public: 
  
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
    void reset();
  };
  
  stat_t stats;
  
  // ctor
  bot_base();
    
  // dtor
  virtual ~bot_base() = default;

  // perform a move on board in, put result in out
  virtual void do_move(const board* in,board* out) = 0;
  
  // perform some action when a new game starts
  virtual void on_new_game();
  
  // output stream to be used instead of std::cout
  // may for example return reference to dummystream
  std::ostream& output();
  
  int get_search_depth() const;
  
  int get_perfect_depth() const;
  
  std::string get_name() const;
  
  void set_search_depth(int _search_depth,int _perfect_depth);
  
  void set_name(const std::string& _name);
    
  void disable_shell_output();
  
};