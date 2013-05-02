#ifndef SQUARED_BOARD_HPP
#define SQUARED_BOARD_HPP

#include <bitset>
#include <cassert>
#include <iostream>
#include <list>
#include <sstream>

#include "game/util.hpp"

enum board_rotation{
  ROTATE_NONE=0,
  ROTATE_90=1,
  ROTATE_180=2,
  ROTATE_270=3,
  ROTATE_FLIP=4,
  ROTATE_FLIP_90=5,
  ROTATE_FLIP_180=6,
  ROTATE_FLIP_270=7
};



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
  
  /// checks for board equality
  bool operator==(const board& b);
  
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
  
  /// rotate the board
  board rotate(board_rotation) const;
  
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
  assert(valid_coord(field_id));
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

inline board board::rotate(board_rotation rot) const
{
  int x,y;
  board res(*this);

  switch(rot){
    case ROTATE_NONE:
      return res;
    case ROTATE_90:
      for(y=0;y<FIELD_SIZE;y++){
        for(x=0;x<FIELD_SIZE;x++){
          res.discs[BLACK].set(y*FIELD_SIZE+x,discs[BLACK].test((y)+(FIELD_SIZE*(FIELD_SIZE-1-x))));
          res.discs[WHITE].set(y*FIELD_SIZE+x,discs[WHITE].test((y)+(FIELD_SIZE*(FIELD_SIZE-1-x))));
        }
      }
      return res;
    case ROTATE_180:
      return res.rotate(ROTATE_90).rotate(ROTATE_90);
    case ROTATE_270:
      return res.rotate(ROTATE_90).rotate(ROTATE_90).rotate(ROTATE_90);
    case ROTATE_FLIP:
      for(y=0;y<FIELD_SIZE;y++){
        for(x=0;x<FIELD_SIZE;x++){
          res.discs[BLACK].set(y*FIELD_SIZE+x,discs[BLACK].test((FIELD_SIZE*y)+(FIELD_SIZE-1-x)));
          res.discs[WHITE].set(y*FIELD_SIZE+x,discs[WHITE].test((FIELD_SIZE*y)+(FIELD_SIZE-1-x)));
        }
      }
      return res;
    case ROTATE_FLIP_90:
      return res.rotate(ROTATE_FLIP).rotate(ROTATE_90);
    case ROTATE_FLIP_180:
      return res.rotate(ROTATE_FLIP).rotate(ROTATE_90).rotate(ROTATE_90);
    case ROTATE_FLIP_270:
      return res.rotate(ROTATE_FLIP).rotate(ROTATE_90).rotate(ROTATE_90).rotate(ROTATE_90);
  }
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
  a = (a+0x17bea992) + (a<<7);  \
  
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
