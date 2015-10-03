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

#include "SQLiteCpp/SQLiteCpp.h"

class bot_base{


  
  std::string name;
  int search_depth;
  int perfect_depth;
  std::ostream* output_stream;
  SQLite::Database* book;
  
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
  virtual ~bot_base();

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
  
  void open_book();
  
  // add entry to book
  // may do nothing if entry with larger depth exists
  void add_to_book(const board* b,int depth,int heuristic,int pv);
  
  // assign heuristic and pv values for board b with depth at least depth
  // returns whether succesful
  bool lookup_book(const board* b,int depth,int* heuristic,int* pv);
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

