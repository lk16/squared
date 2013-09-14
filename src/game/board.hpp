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
  bool operator==(const board& b);
  
  /// resets the board to starting position
  void reset();
  
  /// does move (field_id) for current turn
  /// if this is a valid move, (result++) is returned, and this->turn is flipped
  /// if not, result is returned and this->turn remains the same
  board* do_move(int field_id,board* result) const;
  
  /// gets all children from this board
  /// if out==NULL it is unchanged
  void get_children(board* out,int* move_count) const;

  /// tests whether both players dont have any moves left
  bool test_game_ended() const;
  
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means white has more
  int get_disc_diff() const;
  
  /// calculate hash
  unsigned long long hash() const;
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
  memcpy(this,&b,sizeof(board));
}


inline board& board::operator=(const board& b)
{
  memcpy(this,&b,sizeof(board));
  return *this;
}

inline bool board::test_game_ended() const
{
  
  if((discs[WHITE] | discs[BLACK]).count() == (unsigned int)TOTAL_FIELDS){
    return true;
  }
  int move_count;
  get_children(NULL,&move_count);
  if(move_count==0){
    board copy = board(*this);
    copy.turn = opponent(copy.turn);
    copy.get_children(NULL,&move_count);
    return move_count==0;
  }
  return false;
}

inline unsigned long long board::hash() const{

  
  unsigned long long black,white;
 
  black = discs[BLACK].to_ulong();
  white = discs[WHITE].to_ulong();

#define hash_64bit(a) \
  a = (a+0x479ab41d) + (a<<8);  \
  a = (a^0xe4aa10ce) ^ (a>>5);  \
  a = (a+0x9942f0a6) - (a<<14); \
  a = (a^0x5aedd67d) ^ (a>>3);  \
  a = (a+0x17bea992) + (a<<7);  
  
  hash_64bit(black);
  hash_64bit(white);

#undef hash_64bit
  
  return black ^ white;
}

inline bool board::operator==(const board& b)
{
  // the ! is because memcmp returns 0 when both memory blocks are equal
  return !memcmp(this,&b,sizeof(board));
}


#endif
