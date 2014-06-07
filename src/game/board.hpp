#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "util/bitset.hpp"
#include "util/const.hpp"
#include "util/bitset.hpp"
#include "util/macros.hpp"
#include "util/math.hpp"

  
struct board;
//typedef bits64(board::*move_func_t)();

extern bits64 (board::* const move_funcs[65])();



struct board{
  
  
  
  // static constants
  static const unsigned int border[64]; // border flags
  
  // contains bitsets of which bits are set when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const bits64 walk_possible[8][7];
  
  // contains index differences when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const int walk_diff[8][7];       
  
  // location on board, for table see source file 
  static const bits64 location[10];    
  

  
  enum fields{
    A1,A2,A3,A4,A5,A6,A7,A8,
    B1,B2,B3,B4,B5,B6,B7,B8,
    C1,C2,C3,C4,C5,C6,C7,C8,
    D1,D2,D3,D4,D5,D6,D7,D8,
    E1,E2,E3,E4,E5,E6,E7,E8,
    F1,F2,F3,F4,F5,F6,F7,F8,
    G1,G2,G3,G4,G5,G6,G7,G8,
    H1,H2,H3,H4,H5,H6,H7,H8,
    PASS
  };


  // me = discs of player to move
  // opp = opponent
  bits64 me,opp;
  
  // represents which player is to move:
  // false means black, true means white
  /// bool turn;
    
  /// does NOTHING; call reset() to initialize
  board();
  
  /// copy ctor
  board(const board& b);

  /// move ctor
  board(const board&& b);
  
  /// ctor from string of board::to_string()
  board(const std::string& in);

  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// checks for board equality
  bool operator==(const board& b) const;
  
  /// checks for ordering
  bool operator<(const board& b) const;
 
  /// resets the board to starting position
  void reset();
  
  /// switches me and opp
  void switch_turn();
  
  /// checks whether for *this and this->turn, field_id is a valid move
  bool is_valid_move(int field_id) const;
    
  /// out will represent a bitset in which each set bit represents a valid move
  bits64 get_valid_moves() const;

  bits64 get_valid_moves_superset() const;
  
  board do_random_moves(int count) const;
  
  /// gets all children from this board
  /// returns a possibly increased out pointer
  board* get_children(board* out) const;
  
  /// returns whether this board has any children for this->turn
  bool has_valid_moves() const;
  
  /// returns the number of children, without calculating the actual children
  int count_valid_moves() const;
  
  bits64 get_empty_fields() const;
  bits64 get_non_empty_fields() const;
  int count_discs() const;
  int count_empty_fields() const;
  
  /// get hash
  long long unsigned hash() const;
  
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means this->turn has more
  int get_disc_diff() const;
  
  /// does a move, returns flipped discs
  bits64 do_move(int field_id);
  
  /// experimental code for making specific move functions for each field
  bits64 do_move_experimental(const int field_id);
  
  /// does named move
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
  bits64 do_move_pass();
  
  /// recovers a board state before move field_id, with flipped discs in undo_data 
  void undo_move(int field_id,bits64 undo_data); 
  
  /// returns string representation
  std::string to_string() const;
  
  /// returns string representation modulo rotation
  std::string to_database_string() const;
  
  /// rotate/mirror the board, 0 <= n <= 7
  board rotate(int n) const;
  
  /// returns how *this was rotated to get b or -1 if no matches are found
  int get_rotation(const board* b) const;

  
};


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

inline void board::switch_turn()
{
  std::swap<bits64>(me,opp);
}

inline bool board::operator<(const board& b) const
{
  return this->to_database_string() < b.to_database_string();
}

inline long long unsigned board::hash() const
{
  return 3*me + 5*opp;
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
  return get_valid_moves() != 0;
}

inline int board::count_valid_moves() const
{
  return count_64(get_valid_moves());  
}

inline bool board::is_valid_move(int field_id) const
{ 
  return (get_valid_moves() & bits64_set[field_id]) != 0ull;
}

inline void board::undo_move(int field_id,bits64 undo_data)
{
  switch_turn();
  
  me &= ~(undo_data | bits64_set[field_id]);
  opp |= (undo_data);  
  
  
  assert((me & undo_data) == 0ull);
  assert((opp & undo_data) == undo_data);
  assert((get_non_empty_fields() & bits64_set[field_id]) == 0ull);
}

inline bits64 board::get_valid_moves_superset() const{
  bits64 res = 0ull;
  bits64 non_empty = get_non_empty_fields();
  
  res |= ((opp << 9) & (non_empty << 18) & 0xFEFEFEFEFEFEFEFE);
  res |= ((opp << 8) & (non_empty << 16) & 0xFEFEFEFEFEFEFEFE);
  res |= ((opp << 7) & (non_empty << 14) & 0xFEFEFEFEFEFEFEFE);
  res |= ((opp << 1) & (non_empty << 1) & 0xFEFEFEFEFEFEFEFE);
  
  res |= ((opp >> 9) & (non_empty >> 18) & 0x7F7F7F7F7F7F7F7F);
  res |= ((opp >> 8) & (non_empty >> 16) & 0x7F7F7F7F7F7F7F7F);
  res |= ((opp >> 7) & (non_empty >> 14) & 0x7F7F7F7F7F7F7F7F);
  res |= ((opp >> 1) & (non_empty >> 1) & 0x7F7F7F7F7F7F7F7F);
  
  res &= ~non_empty;
  return res;
}

inline bits64 board::get_valid_moves() const
{
  bits64 res = 0ull;
  
  for(int d=4;d<8;d++){
    
    assert(board::walk_diff[d][0] > 0);
    
    res |= 
    (
    ((opp >> board::walk_diff[d][0]) & board::walk_possible[d][0]) 
    & 
    (
    ((me >> board::walk_diff[d][1]) & board::walk_possible[d][1])
    |
    (
    ((opp >> board::walk_diff[d][1]) & board::walk_possible[d][1])
    &
    (
    ((me >> board::walk_diff[d][2]) & board::walk_possible[d][2])
    |
    (
    ((opp >> board::walk_diff[d][2]) & board::walk_possible[d][2])
    &
    (
    ((me >> board::walk_diff[d][3]) & board::walk_possible[d][3])
    |
    (
    ((opp >> board::walk_diff[d][3]) & board::walk_possible[d][3])
    &
    (
    ((me >> board::walk_diff[d][4]) & board::walk_possible[d][4])
    |
    (
    ((opp >> board::walk_diff[d][4]) & board::walk_possible[d][4])
    &
    (
    ((me >> board::walk_diff[d][5]) & board::walk_possible[d][5])
    |
    (
    ((opp >> board::walk_diff[d][5]) & board::walk_possible[d][5])
    &
    ((me >> board::walk_diff[d][6]) & board::walk_possible[d][6])
    )
    )
    )
    )  
    )
    )
    )
    )
    )
    )
    );
    
    res |= 
    (
    ((opp << board::walk_diff[d][0]) & board::walk_possible[7-d][0]) 
    & 
    (
    ((me << board::walk_diff[d][1]) & board::walk_possible[7-d][1])
    |
    (
    ((opp << board::walk_diff[d][1]) & board::walk_possible[7-d][1])
    &
    (
    ((me << board::walk_diff[d][2]) & board::walk_possible[7-d][2])
    |
    (
    ((opp << board::walk_diff[d][2]) & board::walk_possible[7-d][2])
    &
    (
    ((me << board::walk_diff[d][3]) & board::walk_possible[7-d][3])
    |
    (
    ((opp << board::walk_diff[d][3]) & board::walk_possible[7-d][3])
    &
    (
    ((me << board::walk_diff[d][4]) & board::walk_possible[7-d][4])
    |
    (
    ((opp << board::walk_diff[d][4]) & board::walk_possible[7-d][4])
    &
    (
    ((me << board::walk_diff[d][5]) & board::walk_possible[7-d][5])
    |
    (
    ((opp << board::walk_diff[d][5]) & board::walk_possible[7-d][5])
    &
    ((me << board::walk_diff[d][6]) & board::walk_possible[7-d][6])
    )
    )
    )
    )  
    )
    )
    )
    )
    )
    )
    );
  }
  
  res &= get_empty_fields();
  return res;
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
  return count_64(get_non_empty_fields());
}

inline int board::count_empty_fields() const
{
  return count_64(get_empty_fields());
}

