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
  int search_depth;
  int perfect_depth;
  std::ostream* output_stream;
  int last_move_heur;
  
public: 
  
  static const int NO_HEUR_AVAILABLE = 1000000;
  
  stat_t stats;
  book_t* book;
  
  // ctor
  bot_base();
    
  // dtor
  virtual ~bot_base();

  // perform a move on board in, put result in out
  virtual void do_move(const board* in,board* out) = 0;
  
  // perform some action when a new game starts
  virtual void on_new_game() = 0;
  
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

inline bot_base::~bot_base()
{
  if(book){
    delete book;
  }  
}

inline bot_base::bot_base():
  name("base"),
  output_stream(&std::cout),
  book(nullptr)
{
}

inline void bot_base::set_last_move_heur(int heur)
{
  last_move_heur = heur;
}


inline int bot_base::get_last_move_heur() const
{
  return last_move_heur;
}

inline int bot_base::rough_prediction(const board* b) const
{
  (void)b;
  std::cout << "warning: bot_" << get_name() << " did not implement ";
  std::cout << " rough_prediction()\n";
  return 0;
}



inline int bot_base::get_search_depth() const
{
  return search_depth; 
}

inline int bot_base::get_perfect_depth() const
{
  return perfect_depth;
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

inline void bot_base::disable_book()
{
  if(book){
    delete book;
    book = nullptr;
  }
}

inline void bot_base::disable_shell_output()
{
  output_stream = new dummystream;
}

inline void bot_base::set_name(const std::string& _name)
{
  name = _name;
}

inline std::string bot_base::get_name() const
{
  return name;
}

inline std::ostream& bot_base::output()
{
  return *output_stream;
}

inline bool bot_base::get_use_book() const
{
  return book!=nullptr;
}




