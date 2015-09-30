#pragma once

#include <unordered_map>

#include "bots/bot_register.hpp"
#include "bots/bot_base.hpp"

class bot_pvs:
  public bot_base
{
  
  
  int ESTIMATE_DEPTH = 4;
  int MIN_SORT_DEPTH = ESTIMATE_DEPTH + 3;
  
  int moves_left;
  
  // search for the best move
  template<bool exact>
  void search(const board* b,board* res);

  template<bool exact,bool sorted>
  int pvs(int alpha,int beta,const board* b);

  template<bool exact>
  int pvs_null_window(int alpha,const board* b);

  
  int look_ahead(const board* b);
public:
  
  // TODO make private when stable
  typedef std::unordered_map<board,ht_item> hash_table_t;
  typedef hash_table_t::iterator hash_table_iter;
  typedef hash_table_t::const_iterator hash_table_citer;
 
  hash_table_t hash_table;
  
  // ctor
  bot_pvs() = default;  
  
  // dtor
  ~bot_pvs() = default;
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game() = 0;
  
  // calculates the heuristic for this->inspected
  virtual int heuristic(const board* b) = 0;

  

};