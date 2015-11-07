#pragma once

#include <algorithm>
#include <atomic>
#include <ctime>
#include <string>
#include <sys/time.h>
#include <thread>

#include "bots/bot_register.hpp"
#include "game/board.hpp"
#include "util/const.hpp"
#include "util/math.hpp"
#include "util/dummystream.hpp"
#include "util/string.hpp"

class bot_base{

public:
  
  enum state_t{
    BOT_NOT_STARTED,
    BOT_THINKING,
    BOT_DONE
  };

protected:
  
  std::string name;
  int search_depth;
  int perfect_depth;
  std::ostream* output_stream;
  
  // perform a move on board in, put result in out
  virtual void do_move(const board* in,board* out) = 0;
  
  void do_move_thread_func(const board* in);
  
  std::atomic<state_t> state;
  
  // result of launch_do_move_thread
  board move_thread_result;
 
  std::thread* thinking_thread;
  
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

  // launch thread with do move, or do nothing depending on state
  state_t launch_do_move_thread(const board* in);
  
  // get result from move thread, WARNING only valid info if state == BOT_DONE
  board get_move_thread_result() const;
  
  // run do_move in same thread
  void do_move_no_thread(const board* in,board* out);
  
  // sets state to BOT_NOT_STARTED, but only if state is BOT_DONE
  void reset_state();
  
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

struct ht_item{
  // ctor
  ht_item(int u,int l): upper_bound(u),lower_bound(l){}
  
  // ctor
  ht_item(int u,int l,int pv): upper_bound(u),lower_bound(l),pv_move(pv){}
  
  // bounds
  int upper_bound,lower_bound;
  
  // principle variation
  int pv_move;
};

