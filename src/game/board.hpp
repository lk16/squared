#ifndef SQUARED_BOARD_HPP
#define SQUARED_BOARD_HPP

#include <bitset>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>
#include <cstring>
#include <set>
#include <unordered_map>

#include "game/util.hpp"





struct board{
  
  // static constants
  static const unsigned int border[64]; // border flags
  static const int direction[8];        // index difference of walking directions
  static std::bitset<64> bit[64];       // (1 << x) for x in [0,63]
  static std::bitset<64> location[10];  // location on board as shown below
  
  // contains bitsets of which bits are set when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const std::bitset<64> walk_possible[8][7];
  
  
  // 0,1,2,3,3,2,1,0,
  // 1,4,5,6,6,5,4,1,
  // 2,5,7,8,8,7,5,2,
  // 3,6,8,9,9,8,6,3,
  // 3,6,8,9,9,8,6,3,
  // 2,5,7,8,8,7,5,2,
  // 1,4,5,6,6,5,4,1,
  // 0,1,2,3,3,2,1,0
  
  
  std::bitset<64> discs[2];
  color turn;
  
  
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
  
  /// applies binary and on both discs[BLACK] and discs[WHITE]
  board& operator&=(const board& b);
  
  /// resets the board to starting position
  void reset();
  
  /// initializes constants
  static void init_constants();
  
  
  /// sets all bits in discs[BLACK] and discs[WHITE]
  void set_all();
  
  /// switches the turn member
  void switch_turn();
  
  
  
  /// checks whether for *this and this->turn, field_id is a valid move
  /// WARNING: NOT EFFICIENT
  bool is_valid_move(int field_id) const;
  
  /// performs move for *this and this->turn
  /// might crash if fed an invalid move
  /// WARNING: NOT EFFICIENT
  void do_move(int field_id,board* out) const;
  
  /// out will represent a bitset of which each set bit represents a square
  /// that COULD BE a valid move
  void get_possible_moves(std::bitset<64> *out) const;
  
  
  
  /// gets all children from this board
  /// returns a possibly increased out pointer
  board* get_children(board* out) const;
  
  /// gets all descendants at a certain depth
  std::set<board> get_descendants(int depth) const;
  
  /// gets whatever name says
  std::bitset<64> get_empty_fields() const;
  std::bitset<64> get_non_empty_fields() const;
  
  
  
  /// get hash
  unsigned long hash() const;
  
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means white has more
  int get_disc_diff() const;
  
  /// tries a move, if not valid, returns false
  bool try_move(int field_id,std::bitset<64>* undo_data);
  
  /// recovers a board state before move field_id, with flipped discs in undo_data 
  void undo_move(int field_id,std::bitset<64>* undo_data); 
  
  /// estimate number of stable discs, without going deeper than max_depth
  int get_stable_disc_count_diff(int max_depth) const;
};


namespace std{
  template<>
  struct hash<board>{
    size_t operator()(board b) const{
      return b.hash();
    }
  };
}

inline board::board()
{
}

inline void board::reset(){
  
  /* wipe all discs of the board */
  discs[BLACK].reset();
  discs[WHITE].reset();
  
  /* put starting pieces on board */
  discs[BLACK].set(28);
  discs[BLACK].set(35);
  discs[WHITE].set(27);
  discs[WHITE].set(36);
  
  turn = BLACK;
  

}

inline board::board(const board& b)
{
  *this = b;
}

inline board& board::operator=(const board& b)
{
  discs[BLACK] = b.discs[BLACK];
  discs[WHITE] = b.discs[WHITE];
  turn = b.turn;
  return *this;
}

inline bool board::operator==(const board& b) const
{
  return b.discs[BLACK]==discs[BLACK] && b.discs[WHITE]==discs[WHITE] && b.turn==turn;
}

inline board& board::operator&=(const board& b)
{
  discs[BLACK] &= b.discs[BLACK];
  discs[WHITE] &= b.discs[WHITE];
  return *this;
}


inline void board::switch_turn()
{
  turn = opponent(turn);
}

inline void board::set_all()
{
  discs[BLACK].set();
  discs[WHITE].set(); 
}

inline bool board::operator<(const board& b) const
{
  if(discs[BLACK] != b.discs[BLACK]){
    return discs[BLACK].to_ulong() < b.discs[BLACK].to_ulong();
  }
  if(discs[WHITE] != b.discs[WHITE]){
    return discs[WHITE].to_ulong() < b.discs[WHITE].to_ulong();
  }
  return (int)turn < (int)b.turn;
}


inline long unsigned int board::hash() const
{
  return 3*discs[BLACK].to_ulong() + 5*discs[WHITE].to_ulong() + 7*turn;
}

inline std::bitset<64> board::get_empty_fields() const
{
  return ~get_non_empty_fields();
}

inline std::bitset<64> board::get_non_empty_fields() const
{
  return discs[WHITE] | discs[BLACK];
}





#endif
