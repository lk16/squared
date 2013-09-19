#ifndef SQUARED_BOARD_HPP
#define SQUARED_BOARD_HPP

#include <bitset>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>
#include <cstring>

#include "game/util.hpp"



static const unsigned int board_border[64] = {
  0x2f,0x07,0x07,0x07,0x07,0x07,0x07,0x97,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x94,
  0xe9,0xe0,0xe0,0xe0,0xe0,0xe0,0xe0,0xf4
};

const int board_direction[8] = {
  -9,-8,-7,-1,1,7,8,9
};



struct board{
  
  std::bitset<TOTAL_FIELDS> discs[2];
  color turn;
    
  /// does NOTHING; call reset() to initialize
  board();
  
  /// copies a board
  board(const board& b);
  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// checks for board equality
  bool operator==(const board& b) const;
  
  /// resets the board to starting position
  void reset();
  
  /// checks whether for *this and this->turn, field_id is a valid move
  /// WARNING: NOT EFFICIENT
  bool is_valid_move(int field_id) const;
  
  /// performs move for *this and this->turn
  /// will crash if fed an invalid move
  /// WARNING: NOT EFFICIENT
  void do_move(int field_id,board* out) const;
  
  
  /// out will represent a bitset of which each bit represents a square
  /// that COULD BE a valid move
  void get_possible_moves(std::bitset<64> *out) const;
  
  
  /// gets all children from this board
  /// if out==NULL it is unchanged
  void get_children(board* out,int* move_count) const;
  
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means white has more
  int get_disc_diff() const;
};



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

/*namespace std{
  template<>
  struct hash<board>{
    size_t operator()(const board& b) const{
      return 3*b.discs[BLACK].to_ulong() + 5*b.discs[WHITE].to_ulong() + 7*b.turn;
    }
  };
  
}*/

#endif
