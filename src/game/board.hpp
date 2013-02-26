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
  
  /// create board based on oneliner output
  board(const char* in);
  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// resets the board to starting position
  void reset();
  
  /// sets all bitset values. used for disc stability check
  void set_all();
  
  /// does move (field_id) for current turn
  /// returns true if valid, false otherwise
  bool do_move(int field_id,board* result) const;
  
  /// counts the number of valid moves for color c
  int count_moves() const;
  
  /// gets all children from this board
  void get_children(board* array,int* move_count) const;
  
  /// counts number of discs of a given player
  int count_discs(color c) const;

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

  /// returns the color if disc (field_id)
  color get_color(int field_id) const;
  
  /// tests whether disc (field_id) has color c
  bool has_color(int field_id,color c) const;
  
  /// sets disc (field_id) to color c
  void set_color(int field_id,color c);

  /// tests whether  0 <= coord < FIELD_SIZE
  static bool valid_coord(int coord);
    
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference, positive means white has more
  int get_disc_diff() const;
  
  /// convert to one line for readable 
  void oneliner(char* out) const;
  

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
  discs[BLACK].set(FIELD_SIZE*((FIELD_SIZE/2)  ) + (FIELD_SIZE/2)-1);
  discs[BLACK].set(FIELD_SIZE*((FIELD_SIZE/2)-1) + (FIELD_SIZE/2)  );
  discs[WHITE].set(FIELD_SIZE*((FIELD_SIZE/2)-1) + (FIELD_SIZE/2)-1);
  discs[WHITE].set(FIELD_SIZE*((FIELD_SIZE/2)  ) + (FIELD_SIZE/2)  );
  
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


inline void board::set_color(int field_id,color c)
{
  assert(on_board(field_id));
  switch(c){
    case BLACK:
      discs[WHITE].set(field_id,false);
      discs[BLACK].set(field_id,true);
      break;
    case WHITE:
      discs[BLACK].set(field_id,false);
      discs[WHITE].set(field_id,true);
      break;
    case EMPTY:
      discs[BLACK].set(field_id,false);
      discs[WHITE].set(field_id,false);
      break;
    default:
      /* can never happen */
      CRASH;
      break;
  }
}

inline color board::get_color(int field_id) const
{
  return discs[BLACK].test(field_id) ? BLACK : discs[WHITE].test(field_id) ? WHITE : EMPTY;
}

inline bool board::has_color(int field_id, color c) const
{
  return get_color(field_id)==c;
}

inline bool board::valid_coord(int coord)
{
  return coord>=0 && coord<FIELD_SIZE;
}

inline bool board::test_game_ended() const
{
  board copy;
  
  if(std::bitset<TOTAL_FIELDS>(discs[WHITE] | discs[BLACK]).count() == (unsigned int)TOTAL_FIELDS){
    return true;
  }
  if(has_moves()){
    return false;
  }
  
  copy = board(*this);
  copy.turn = opponent(copy.turn);
  return !has_moves();
}

inline int board::count_discs(color c) const
{
  assert(c != EMPTY);
  return discs[c].count();
}

inline int board::max_moves_left() const
{
  return TOTAL_FIELDS - std::bitset<TOTAL_FIELDS>(discs[BLACK] | discs[WHITE]).count();
}


#endif
