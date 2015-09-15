#pragma once

#include <algorithm>
#include <ctime>
#include <string>
#include <sys/time.h>

#include "bots/bot_register.hpp"
#include "book/book.hpp"
#include "game/board.hpp"
#include "util/const.hpp"
#include "util/math.hpp"
#include "util/dummystream.hpp"

class bot_base{


  
  std::string name;
  int search_depth;
  int perfect_depth;
  std::ostream* output_stream;
  int last_move_heur;
  
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
  
  
  static const int NO_HEUR_AVAILABLE = 1000000;
  
  stat_t stats;
  book_t* book;
  
  // ctor
  bot_base();
    
  // dtor
  virtual ~bot_base() = default;

  // perform a move on board in, put result in out
  virtual void do_move(const board* in,board* out) = 0;
  
  // perform some action when a new game starts
  virtual void on_new_game();
  
  // does a very rough prediction on the perfect play
  virtual int rough_prediction(const board* b) const;
  
  
  // output stream to be used instead of std::cout
  // may for example return reference to dummystream
  std::ostream& output();
  
  int get_search_depth() const;
  
  int get_perfect_depth() const;
  
  bool get_use_book() const;
  
  std::string get_name() const;
  
  int get_last_move_heur() const;
  
  void set_last_move_heur(int heur);
  
  void set_search_depth(int _search_depth,int _perfect_depth);
  
  void set_name(const std::string& _name);
  
  void disable_book();
  
  void disable_shell_output();
  
};