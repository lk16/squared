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



struct board{
 
  // location on board, as displayed below
  static const bits64 location[10];    
  
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
  void switch_turn();
  
  // checks whether field_id is a valid move
  // WARNING not efficient
  bool is_valid_move(int field_id) const;
    
  // returns a bitset of valid moves
  bits64 get_valid_moves() const;
  
  // helper function of get_valid_moves()
  static bits64 get_some_moves(const bits64 me, const bits64 opp_mask, const int dir);
  
  // returns a copy of *this after count random moves
  board do_random_moves(int count) const;
  
  // gets all children from this board
  // returns a pointer to the last found child
  board* get_children(board* out) const;
  
  // returns whether this board has any valid moves
  bool has_valid_moves() const;
  
  // returns the number of valid moves
  int count_valid_moves() const;
  
  // returns the number of opponent moves  
  int count_opponent_moves() const;
  
  // counts frontier discs
  void count_frontier_discs(int* me,int* opp) const;
  
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
  
  // returns html code displaying *this
  // adjusts colors for the right turn
  std::string to_html_table(int turn) const;
  
  // returns disc count difference 
  // positive means me has more than opp
  int get_disc_diff() const;
  
  // does move field_id
  // returns flipped discs
  bits64 do_move(int field_id);
  
  // experimental code for making specific move functions for each field
  bits64 do_move_experimental(const int field_id);
  
  // converts for example "a1" to 0
  // converts "--" to -1
  // converts anything else to -2
  static int position_to_index(const std::string& str);
  
  // reverses position_to_index
  static std::string index_to_position(int index);
  
  // does named move
  bits64 do_move_A1();
  bits64 do_move_A2();
  bits64 do_move_A3();
  bits64 do_move_A4();
  bits64 do_move_A5();
  bits64 do_move_A6();
  bits64 do_move_A7();
  bits64 do_move_A8();
  bits64 do_move_B1();
  bits64 do_move_B2();
  bits64 do_move_B3();
  bits64 do_move_B4();
  bits64 do_move_B5();
  bits64 do_move_B6();
  bits64 do_move_B7();
  bits64 do_move_B8();
  bits64 do_move_C1();
  bits64 do_move_C2();
  bits64 do_move_C3();
  bits64 do_move_C4();
  bits64 do_move_C5();
  bits64 do_move_C6();
  bits64 do_move_C7();
  bits64 do_move_C8();
  bits64 do_move_D1();
  bits64 do_move_D2();
  bits64 do_move_D3();
  bits64 do_move_D4();
  bits64 do_move_D5();
  bits64 do_move_D6();
  bits64 do_move_D7();
  bits64 do_move_D8();
  bits64 do_move_E1();
  bits64 do_move_E2();
  bits64 do_move_E3();
  bits64 do_move_E4();
  bits64 do_move_E5();
  bits64 do_move_E6();
  bits64 do_move_E7();
  bits64 do_move_E8();
  bits64 do_move_F1();
  bits64 do_move_F2();
  bits64 do_move_F3();
  bits64 do_move_F4();
  bits64 do_move_F5();
  bits64 do_move_F6();
  bits64 do_move_F7();
  bits64 do_move_F8();
  bits64 do_move_G1();
  bits64 do_move_G2();
  bits64 do_move_G3();
  bits64 do_move_G4();
  bits64 do_move_G5();
  bits64 do_move_G6();
  bits64 do_move_G7();
  bits64 do_move_G8();
  bits64 do_move_H1();
  bits64 do_move_H2();
  bits64 do_move_H3();
  bits64 do_move_H4();
  bits64 do_move_H5();
  bits64 do_move_H6();
  bits64 do_move_H7();
  bits64 do_move_H8();
  
  // undoes move field_id, flips back all discs represented by undo_data
  void undo_move(int field_id,bits64 undo_data); 
  
  // returns whether all children are the same modulo rotation/mirroring
  static bool only_similar_siblings(const board* siblings,int n);
  
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
  return bits64_find_first(get_non_empty_fields() ^ after->get_non_empty_fields());
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
  me = bits64_set[28] | bits64_set[35];
  opp = bits64_set[27] | bits64_set[36];
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
  return me==b.me && opp==b.opp;
}

inline bool board::operator!=(const board& b) const
{
  return !(b == *this);
}


inline void board::switch_turn()
{
  std::swap<bits64>(me,opp);
}

inline bool board::operator<(const board& b) const
{
  return this->to_database_string() < b.to_database_string();
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
  return get_valid_moves() != 0ull;
}

inline int board::count_valid_moves() const
{
  return bits64_count(get_valid_moves());  
}

inline bool board::is_valid_move(int field_id) const
{ 
  return (get_valid_moves() & bits64_set[field_id]) != 0ull;
}

inline void board::undo_move(int field_id,bits64 undo_data)
{
  bits64 tmp = me;
  me = opp & ~(undo_data | bits64_set[field_id]);
  opp = tmp | undo_data;
}

inline bits64 board::get_some_moves(const bits64 me, const bits64 opp_mask, const int dir) 
{
  // this code was copied from Edax
  
  // 1-stage Parallel Prefix (intermediate between kogge stone & sequential) 
  // 6 << + 6 >> + 7 | + 10 &
  bits64 flip_l, flip_r,mask_l, mask_r;
  const bits64 dir2 = dir + dir;

  flip_l  = opp_mask & (me << dir);
  flip_r  = opp_mask & (me >> dir);
  flip_l |= opp_mask & (flip_l << dir);
  flip_r |= opp_mask & (flip_r >> dir);
  mask_l  = opp_mask & (opp_mask << dir);
  mask_r  = opp_mask & (opp_mask >> dir);
  flip_l |= mask_l & (flip_l << dir2);
  flip_r |= mask_r & (flip_r >> dir2);
  flip_l |= mask_l & (flip_l << dir2);
  flip_r |= mask_r & (flip_r >> dir2);

  return (flip_l << dir) | (flip_r >> dir);
}

inline bits64 board::get_valid_moves() const
{
  // this code was copied from Edax
  const bits64 mask = opp & 0x7E7E7E7E7E7E7E7Eull;

  return (0ull
    | get_some_moves(me,mask,1) // horizontal
    | get_some_moves(me,opp, 8) // vertical
    | get_some_moves(me,mask,7) // diagonals
    | get_some_moves(me,mask,9)
  )
  & get_empty_fields(); // mask with empties
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
  return bits64_count(get_non_empty_fields());
}

inline int board::count_empty_fields() const
{
  return bits64_count(get_empty_fields());
}

inline int board::count_opponent_moves() const
{
  board copy(*this);
  copy.switch_turn();
  return copy.count_valid_moves();
}