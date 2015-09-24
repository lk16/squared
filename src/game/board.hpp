#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "game/xot.hpp"
#include "util/bitset.hpp"
#include "util/const.hpp"
#include "util/bitset.hpp"
#include "util/macros.hpp"
#include "util/math.hpp"

struct board;

extern bits64 (board::* const move_funcs[64])();

struct board{
 
  // location on board, as displayed below
  static const bits64 location[10];    
  
  static const bits64 ordered_locations[10];
 
  static const bits64 dir_mask[64][8];
  
  
  // 0,1,2,3,3,2,1,0,
  // 1,4,5,6,6,5,4,1,
  // 2,5,7,8,8,7,5,2,
  // 3,6,8,9,9,8,6,3,
  // 3,6,8,9,9,8,6,3,
  // 2,5,7,8,8,7,5,2,
  // 1,4,5,6,6,5,4,1,
  // 0,1,2,3,3,2,1,0
  
  enum square_names{
    X_SQUARES = 0,
    C_SQUARES = 1,
    B_SQUARES = 2,
    A_SQUARES = 3
  };
  
  enum fields{
    A1,A2,A3,A4,A5,A6,A7,A8,
    B1,B2,B3,B4,B5,B6,B7,B8,
    C1,C2,C3,C4,C5,C6,C7,C8,
    D1,D2,D3,D4,D5,D6,D7,D8,
    E1,E2,E3,E4,E5,E6,E7,E8,
    F1,F2,F3,F4,F5,F6,F7,F8,
    G1,G2,G3,G4,G5,G6,G7,G8,
    H1,H2,H3,H4,H5,H6,H7,H8
  };


  // discs of player to move
  bits64 me;
  
  // discs of other player
  bits64 opp;
  
  // does NOTHING; call reset() to initialize
  board();
  
  // copy ctor
  board(const board& b);

  // move ctor
  board(const board&& b);
  
  // ctor from string returned by board::to_string()
  board(const std::string& in);
  
  // assigns a board from b
  board& operator=(const board& b);
  
  // checks for board equality
  bool operator==(const board& b) const;
  
  // checks for board inequality
  bool operator!=(const board& b) const;
  
  // checks for ordering
  bool operator<(const board& b) const;
 
  // resets the board to starting position
  void reset();
  
  // init with random xot board
  // thanks to http://berg.earthlingz.de/xot/download.php?lang=en
  void xot();
  
  // switches me and opp
  board* switch_turn();
  
  // checks whether field_id is a valid move
  // WARNING not efficient
  bool is_valid_move(int field_id) const;
    
  // returns a bitset of valid moves
  bits64 get_valid_moves() const;
    
  // returns a copy of *this after count random moves
  board do_random_moves(int count) const;
  
  // gets all children from this board
  // returns a pointer to the last found child
  board* get_children(board* out) const;
  
  // gets all children from this board
  // returns a pointer to the last found child
  // use this when valid moves are known
  board* get_children(board* out,bits64 moves) const;
  
  // returns whether this board has any valid moves
  bool has_valid_moves() const;
  
  // returns the number of valid moves
  int count_valid_moves() const;
  
  // returns whether the opponent has moves
  bool opponent_has_moves() const;
  
  // returns the number of opponent moves  
  int count_opponent_moves() const;
  
  // counts frontier discs
  void get_frontier_discs(int* me,int* opp) const;
  
  // returns a bitset of empty fields
  bits64 get_empty_fields() const;
  
  // returns a bitset of non empty fields
  bits64 get_non_empty_fields() const;
  
  // returns the amount of discs on the board
  int count_discs() const;
  
  // returns the amount of empty fields on the board
  int count_empty_fields() const;
  
  
    
  // returns string displaying this in a ascii art kind of way
  // adjusts colors for the right turn
  std::string to_ascii_art(int turn) const;
  
  // prints this->ascii_art() to cout
  void show(int turn) const;
  
  // returns disc count difference 
  // positive means me has more than opp
  int get_disc_diff() const;
  
  // does move field_id
  // returns flipped discs
  bits64 do_move(int field_id);
    
  // converts for example "a1" to 0
  // converts "--" to -1
  // converts anything else to -2
  static int position_to_index(const std::string& str);
  
  // reverses position_to_index
  static std::string index_to_position(int index);
  
  template<int field_id>
  bits64 do_move_internally();
  
  // undoes move field_id, flips back all discs represented by undo_data
  void undo_move(bits64 move_bit,bits64 undo_data); 

  // returns string representation of *this
  std::string to_string() const;
  
  // returns string representation of *this modulo rotation
  std::string to_database_string() const;
  
  // returns index of the move done between *this and *after
  // if none found returns 64
  int get_move_index(const board* after) const;
  
  // returns *this modulo rotation
  board to_database_board() const;
  
  // returns a mirrored or rotated version of *this
  // 0 <= n <= 7
  board rotate(int n) const;
  
  // returns how *this was rotated to get *b
  // returns -1 if no matches are found
  int get_rotation(const board* b) const;

  // count sum of flippable discs for all moves
  int get_mobility(bits64 moves) const;
  
};

struct board_hash {
  size_t operator()(const board b) const{
    return b.me ^ ((b.opp << 32) | (b.opp >> 32));
  }
};

extern bits64 (board::* const move_funcs[64])();

inline bits64 board::do_move(int move_id)
{  
  return (this->*move_funcs[move_id])(); 
}


inline int board::get_move_index(const board* after) const
{
  return (get_non_empty_fields() ^ after->get_non_empty_fields()).first_index();
}




inline board::board(const board&& b):
  me(std::move(b.me)),
  opp(std::move(b.opp))
{
}


inline board::board()
{
}

inline void board::reset(){
  me = bits64().set(28).set(35);
  opp = bits64().set(27).set(36);
}

inline board::board(const board& b)
{
  *this = b;
}

inline board& board::operator=(const board& b)
{
  me = b.me;
  opp = b.opp;
  return *this;
}

inline bool board::operator==(const board& b) const
{
  return (me == b.me) && (opp == b.opp);
}

inline bool board::operator!=(const board& b) const
{
  return !(b == *this);
}


inline board* board::switch_turn()
{
  std::swap<bits64>(me,opp);
  return this;
}

inline bool board::operator<(const board& b) const
{
  if(me != b.me){
    return me < b.me;
  }
  return opp < b.opp;
}

inline bits64 board::get_empty_fields() const
{
  return ~get_non_empty_fields();
}

inline bits64 board::get_non_empty_fields() const
{
  return me | opp;
}

inline bool board::has_valid_moves() const
{
  return get_valid_moves().any();
}

inline bool board::opponent_has_moves() const
{
  return board(*this).switch_turn()->get_valid_moves().any();
}

inline int board::count_valid_moves() const
{
  return get_valid_moves().count();  
}

inline bool board::is_valid_move(int field_id) const
{ 
  return get_valid_moves().test(field_id);
}

inline void board::undo_move(bits64 move_bit,bits64 undo_data)
{
  bits64 tmp = me;
  me = opp & ~(undo_data | move_bit);
  opp = tmp | undo_data;
}

inline bits64 board::get_valid_moves() const
{

bits64 flip_l, flip_r,mask_l, mask_r;
bits64 res = 0ull;
  
  // this funtion is a modified version of code from Edax
  const bits64 mask = opp & bits64(0x7E7E7E7E7E7E7E7Eull);

  flip_l = mask & (me << 1);
  flip_l |= mask & (flip_l << 1);
  mask_l = mask & (mask << 1);
  flip_l |= mask_l & (flip_l << 2);
  flip_l |= mask_l & (flip_l << 2);
  flip_r = mask & (me >> 1);
  flip_r |= mask & (flip_r >> 1);
  mask_r = mask & (mask >> 1);
  flip_r |= mask_r & (flip_r >> 2);
  flip_r |= mask_r & (flip_r >> 2);
  res |= (flip_l << 1) | (flip_r >> 1);

  flip_l = opp & (me << 8);
  flip_l |= opp & (flip_l << 8);
  mask_l = opp & (opp << 8);
  flip_l |= mask_l & (flip_l << 16);
  flip_l |= mask_l & (flip_l << 16);
  flip_r = opp & (me >> 8);
  flip_r |= opp & (flip_r >> 8);
  mask_r = opp & (opp >> 8);
  flip_r |= mask_r & (flip_r >> 16);
  flip_r |= mask_r & (flip_r >> 16);
  res |= (flip_l << 8) | (flip_r >> 8);

  flip_l = mask & (me << 7);
  flip_l |= mask & (flip_l << 7);
  mask_l = mask & (mask << 7);
  flip_l |= mask_l & (flip_l << 14);
  flip_l |= mask_l & (flip_l << 14);
  flip_r = mask & (me >> 7);
  flip_r |= mask & (flip_r >> 7);
  mask_r = mask & (mask >> 7);
  flip_r |= mask_r & (flip_r >> 14);
  flip_r |= mask_r & (flip_r >> 14);
  res |= (flip_l << 7) | (flip_r >> 7);

  flip_l = mask & (me << 9);
  flip_l |= mask & (flip_l << 9);
  mask_l = mask & (mask << 9);
  flip_l |= mask_l & (flip_l << 18);
  flip_l |= mask_l & (flip_l << 18);
  flip_r = mask & (me >> 9);
  flip_r |= mask & (flip_r >> 9);
  mask_r = mask & (mask >> 9);
  flip_r |= mask_r & (flip_r >> 18);
  flip_r |= mask_r & (flip_r >> 18);
  res |= (flip_l << 9) | (flip_r >> 9);

  
  return res & get_empty_fields(); // mask with empties
}

inline int board::get_rotation(const board* b) const
{
  for(int i=0;i<8;i++){
    if(rotate(i) == *b){
      return i;
    }
  }
  return -1;
}

inline int board::count_discs() const
{
  return get_non_empty_fields().count();
}

inline int board::count_empty_fields() const
{
  return get_empty_fields().count();
}

inline int board::count_opponent_moves() const
{
  return board(*this).switch_turn()->count_valid_moves();
}

template<int field_id>
inline bits64 board::do_move_internally()
{

  bits64 flipped = 0ull;
  
  const bits64* mask = dir_mask[field_id];
  
  for(int d=0;d<8;++d){
    bits64 line = mask[d];
    if(d<4){
      int end = (line & me).last_index();
      line.reset_before(end);
    }
    else{
      int end = (line & me).first_index();
      line.reset_after(end);
    }
    flipped |= line.is_subset_of_mask(opp) & line;
  }
  
  me.set(field_id) |= flipped;
  opp &= ~me;
  switch_turn(); 
  return flipped;
}
