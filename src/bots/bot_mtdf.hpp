#pragma once

#include "bots/bot_base.hpp"

class bot_mtdf:
  public bot_base
{
  int search_max_sort_depth;
  
  int moves_left;
  
  int discs_on_searched_board;
  
  bool last_search_exact;
  
  static const int NORMAL_MOVE_SORT_DEPTH = 4;
  static const int PERFECT_MOVE_SORT_DEPTH = 11;
  
  struct ht_data{
    int lower_bound,upper_bound,best_move,uses;
  };
 
  typedef std::unordered_map<board,ht_data,board_hash> hash_table_t;
  hash_table_t hash_table;
  
  
public:
  board inspected;
  
  
  bot_mtdf();  
  
  ~bot_mtdf();
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game();
  
  // calculates the heuristic for this->inspected
  virtual int heuristic() = 0;
  
private:
  
  // sort children
  void do_sorting(board* children,int n);
  
  
   // does the only move, no evaluation necessary
  void do_move_one_possibility(const board* b,board* res);

  // performs best move if found in book, returns whether successful
  bool do_move_book(const board* b,board* res);

  // evaluates and performs a move normally
  template<bool exact>
  void do_move_search(const board* b,board* res);
  
  // performs mtdf search
  template<bool sort,bool exact>
  int mtdf(int f,int lower_bound);
  
  // performs null window search
  template<bool sort,bool exact>
  int null_window(int alpha);
  
  // returns res
  template<bool exact>
  int update_hash_table(int alpha,int res,int move);
  
  
  template<bool exact>
  bool suitable_hashtable_entry();

};