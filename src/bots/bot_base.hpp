#ifndef SQUARED_BOT_BASE_HPP
#define SQUARED_BOT_BASE_HPP

#include <algorithm>
#include <ctime>
#include <list>
#include <vector>
#include <sys/time.h>

#include "game/util.hpp"
#include "game/board.hpp"

class bot_base{
  
  int max_depth,max_endgame_depth,look_ahead;
 
  struct hash_table_entry{
    hash_table_entry* next;
    board* b;
    int heur;
    
    hash_table_entry(){}
    
    hash_table_entry(hash_table_entry* _next,board* _b,int _heur):
      next(_next),b(_b),heur(_heur){}
  };
  
public:
  const static int HASH_TABLE_SIZE = 1021;
  
private:
  hash_table_entry* hash_table[HASH_TABLE_SIZE];
  
  void init_hash_table();
  void clear_hash_table();
  void add_hash_table_entry(const board* b,int heur);
  
  
  
  /// overridable function that should return positive heuristic if the move is better for color c
  virtual int heuristic(const board* b);
  
  /// calculates the heuristic for b using alpha beta pruning
  int alpha_beta(const board* b,int alpha, int beta,int depth_remaining);
  
  
  void sort_boards(board* moves,int move_count,int depth);  
  
  board stable;
  
public:
  color c;
  
  int get_max_depth() const;
  
  /// constructor
  bot_base(color c,int max_depth,int max_endgame_depth);
  
  /// destructor
  ~bot_base();
  
  /// calculate best move of b and put it in res
  void do_move(const board* b,board* res);
  
  /// for timing purposes
  long long nodes;
  int prev_move_time;  
  
};

#endif