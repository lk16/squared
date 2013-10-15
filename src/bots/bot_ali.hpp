#ifndef SQUARED_BOT_ALI_HPP
#define SQUARED_BOT_ALI_HPP

#include <bitset>
#include <cassert>
#include <string.h>
#include <functional>
#include <unordered_map>
#include <vector>

#include "game/util.hpp"
#include "bots/bot_base.hpp"

#define BOT_ALI_USE_HASHTABLE            0
#define BOT_ALI_ALWAYS_SHOW_HEUR         0
#define BOT_ALI_MIN_SEARCH_DEPTH_TO_SORT 5
#define BOT_ALI_MIN_HASH_TABLE_DEPTH     6
#define BOT_ALI_MAX_HASH_TABLE_DEPTH     12

struct bot_ali:
  public bot_base
{
  
  enum eval_mode{
    NORMAL_MODE,
    PERFECT_MODE
  };
  
  
  std::bitset<64> location_bitsets[10];  
  bool shell_output;
  
private:
  // this board will be used by negamax(), negamax_exact(), heuristic(), and do_move()
  // it represents the currently inspected move
  board inspected;
  
public:
#if BOT_ALI_USE_HASHTABLE 
  std::unordered_map<board,int> table;
#endif

  bot_ali(color c, int _search_depth, int _wld_depth,int _perfect_depth);
  ~bot_ali();
  
  /// picks a move!
  virtual void do_move(const board* b,board* res);
  
  /// calculates the heuristic for this->inspected
  /// positive is good for WHITE 
  int heuristic();
  
  /// calculates the heuristic for this->inspected using negamax
  /// positive result is good for white
  int negamax(int alpha, int beta,int depth_remaining);
  
  /// calculates the result for perfect play of this->inspected, 
  /// positive result is good for white
  /// this is NOT multiplied with EXACT_SCORE_FACTOR
  int negamax_exact(int alpha, int beta);
  
  /// calculates the heuristic using the hill climbing algorithm recursively
  /// positive result is good for white
  int hill_climbing(board* b,int look_ahead,int depth);
  
  /// calculates the heuristic using the hill climbing algorithm recursively
  /// until no moves can be done anymore
  /// positive result is good for white
  /// WARNING: not perfect
  int hill_climbing_infinite(board* b,int look_ahead);
  
  /// sort boards descending according to heurs
  void sort_boards(board *boards,int* heurs, int count);
  
  /// sort boards descending according to heurs
  void sort_board_int_pairs(std::vector<std::pair<board,int> >& boards);
  
  
  void disable_shell_output();
};



#endif