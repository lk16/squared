#pragma once

#include "bots/bot_base.hpp"

class bot_mtdf:
  public bot_base
{
  int search_max_sort_depth;
  int moves_left;
  
  static const int NORMAL_MOVE_SORT_DEPTH = 4;
  static const int PERFECT_MOVE_SORT_DEPTH = 7;
  
  struct tpt_value{
    int lower_bound,upper_bound,best_move;
    
    tpt_value(){
      lower_bound = MIN_HEURISTIC;
      upper_bound = MAX_HEURISTIC;
      best_move = -1;
    }
    
    tpt_value(int lb,int ub,int bm){
       lower_bound = lb,upper_bound = ub,best_move = bm;
    }
  };
 
  std::unordered_map<board,tpt_value,board_hash> hash_table;
  
public:
  board inspected;
  
  
  bot_mtdf();  
  
  ~bot_mtdf();
  
  // picks a move!
  virtual void do_move(const board* b,board* res);
    
  // does something when a new game starts
  virtual void on_new_game() = 0;
  
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
  
};