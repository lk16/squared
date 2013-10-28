#ifndef SQUARED_BOARD_HPP
#define SQUARED_BOARD_HPP

#include <bitset>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "game/util.hpp"


struct board{
  
  // static constants
  static const unsigned int border[64]; // border flags
  
  // (1 << x) for x in [0,63]
  static const std::bitset<64> bit[64];       
  
  // contains bitsets of which bits are set when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const std::bitset<64> walk_possible[8][7];
  
  // contains index differences when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const int walk_diff[8][7];       
  
  // contains bitsets of captured discs
  // in direction (1st index) for number of steps (2nd index)
  // considering move in capture_start
  static const std::bitset<64> capture[8][6];
  static const int capture_start[8][6];
  
  
  // location on board, for table see source file 
  static const std::bitset<64> location[10];    
  
  
  // represents which player is to move:
  // -1 means black, 1 means white
  char turn;

  // discs of player to move
  std::bitset<64> me;
  
  // discs of opponent of player to move
  std::bitset<64> opp;
  
  // did any player pass yet?
  bool passed;
  
  
  /// does NOTHING; call reset() to initialize
  board();
  
  /// copies a board
  board(const board& b);
  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// checks for board equality
  bool operator==(const board& b) const;
  
  /// checks for ordering
  bool operator<(const board& b) const;
 
  /// resets the board to starting position
  void reset();
  
  /// switches the turn member
  void switch_turn();
  
  /// generate random board 
  /// WARNING: no guarantees on number of moves or 
  /// reachability from reset() board can be given
  void randomize();
  
  /// checks whether for *this and this->turn, field_id is a valid move
  bool is_valid_move(int field_id) const;
  
  
  /// out will represent a bitset in which each set bit represents a valid move
  void get_valid_moves(std::bitset<64> *out) const;
  
  /// out will represent a bitset in which each set bit represents a valid direction
  void get_valid_directions(std::bitset<8>* out) const;
  
  
  
  /// gets all children from this board
  /// returns a possibly increased out pointer
  board* get_children(board* out) const;
  
  /// returns whether this board has any children for this->turn
  bool has_children() const;
  
  /// returns the number of children, without calculating the actual children
  int count_children() const;
  
  /// gets whatever name says
  std::bitset<64> get_empty_fields() const;
  std::bitset<64> get_non_empty_fields() const;
  
  
  
  /// get hash
  unsigned long hash() const;
  
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means this->turn has more
  int get_disc_diff() const;
  
  /// does a move
  void do_move(int field_id,std::bitset<64>* undo_data);
  void do_move_experimental(int field_id,std::bitset<64>* undo_data);
 
  /// debug method
  void check_do_move_experimental() const;
  
  /// recovers a board state before move field_id, with flipped discs in undo_data 
  void undo_move(int field_id,std::bitset<64>* undo_data); 
  
  
};


inline board::board()
{
}

inline void board::reset(){
  
  /* wipe all discs of the board */
  me.reset();
  opp.reset();
  
  /* put starting pieces on board */
  me.set(28);
  me.set(35);
  opp.set(27);
  opp.set(36);
  
  passed = false;
  
  turn = -1;
}

inline board::board(const board& b)
{
  *this = b;
}

inline board& board::operator=(const board& b)
{
  me = b.me;
  opp = b.opp;
  turn = b.turn;
  passed = b.passed;
  return *this;
}

inline bool board::operator==(const board& b) const
{
  return me==b.me && opp==b.opp && passed==b.passed && turn==b.turn;
}

inline void board::switch_turn()
{
  turn *= -1;
  std::swap<std::bitset<64> >(me,opp);
}


inline bool board::operator<(const board& b) const
{
  if(me != b.me){
    return me.to_ulong() < b.me.to_ulong();
  }
  if(opp != b.opp){
    return opp.to_ulong() < b.opp.to_ulong();
  }
  if(turn != b.turn){
    return turn < b.turn;
  }
  if(passed != b.passed){
    return passed;
  }
  return false;
}


inline long unsigned int board::hash() const
{
  return 3*me.to_ulong() + 5*opp.to_ulong() + 7*turn + 11*(passed ? 1 : 0);
}

inline std::bitset<64> board::get_empty_fields() const
{
  return ~get_non_empty_fields();
}

inline std::bitset<64> board::get_non_empty_fields() const
{
  return me | opp;
}

inline bool board::has_children() const
{
  std::bitset<64> moves;
  get_valid_moves(&moves);
  return moves.any();
}

inline int board::count_children() const
{
  std::bitset<64> moves;
  get_valid_moves(&moves);
  return moves.count();  
}

inline void board::randomize()
{
  
  me =  rand_64();
  opp = rand_64();
  turn = -1 + 2*(rand() % 2);
  passed = false;
  
  me &= (~opp);
  opp &= (~me);
}



#endif
