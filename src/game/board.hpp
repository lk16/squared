#ifndef SQUARED_BOARD_HPP
#define SQUARED_BOARD_HPP

#include <bitset>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>

#include "game/util.hpp"


struct board{
  std::bitset<TOTAL_FIELDS> discs[2];
  color turn;
  
  /// initializes a board to starting position
  board();
  
  /// copies a board
  board(const board& b);
  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// checks for board equality
  bool operator==(const board& b);
  
  /// resets the board to starting position
  void reset();
  
  /// sets all bitset values. used for disc stability check
  void set_all();
  
  /// does move (field_id) for current turn
  /// if this is a valid move, (result++) is returned, and this->turn is flipped
  /// if not, result is returned and this->turn remains the same
  board* do_move(int field_id,board* result) const;
  
  /// gets all children from this board
  void get_children(board* array,int* move_count) const;

  /// tests wheter current turn has valid moves
  bool has_moves() const;
  
  /// returns maximum number of moves that could be done before the game is finished
  int max_moves_left() const;
  
  /// tests whether both players dont have any moves left
  bool test_game_ended() const;
  
  /// returns the sum of flippable discs for each valid move of player c
  int get_mobility(color c) const;
  
  /// returns number of flipped discs for move (field_id) and player c, if (field_id) is not valid, 0 is returned
  int count_flipped(int field_id,color c) const;

  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means white has more
  int get_disc_diff() const;
  
  /// calculate hash
  unsigned long long hash() const;
  
};

inline board::board()
{
  reset();
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
  discs[0] = b.discs[0];
  discs[1] = b.discs[1];
  turn = b.turn;
}

inline board& board::operator=(const board& b)
{
  discs[0] = b.discs[0];
  discs[1] = b.discs[1];
  turn = b.turn;
  return *this;
}

inline void board::set_all()
{
  discs[0].set();
  discs[1].set();
}

inline bool board::test_game_ended() const
{
  
  if((discs[WHITE] | discs[BLACK]).count() == (unsigned int)TOTAL_FIELDS){
    return true;
  }
  if(has_moves()){
    return false;
  }
  
  board copy = board(*this);
  copy.turn = opponent(copy.turn);
  return !copy.has_moves();
}

inline int board::max_moves_left() const
{
  return TOTAL_FIELDS - (discs[BLACK] | discs[WHITE]).count();
}


inline unsigned long long board::hash() const{

  
  unsigned long long black,white;
  
  if(FIELD_SIZE != 8){
    return 0ull;  
  }
 
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
  return discs[0] == b.discs[0] 
  && discs[1] == b.discs[1]
  && turn == b.turn;
}


#endif
