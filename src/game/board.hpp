#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "util/const.hpp"
#include "util/bitset.hpp"
#include "util/macros.hpp"
#include "util/math.hpp"


struct board{
  
  
  
  // static constants
  static const unsigned int border[64]; // border flags
  
  // (1 << x) for x in [0,63]
  static const bits bit[64];
  
  // ((~0ull) << (64-x)) for x in [0,63]
  static const bits bits_before[64];
  
  // ((~0ull) >> (64-x)) for x in [0,63]
  static const bits bits_after[64];
  
  
  // contains bitsets of which bits are set when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const bits walk_possible[8][7];
  
  // contains index differences when you can walk
  // in direction (1st index) for number of steps (2nd index)
  static const int walk_diff[8][7];       
  
  // location on board, for table see source file 
  static const bits location[10];    
  


  // me = discs of player to move
  // opp = opponent
  bits me,opp;
  
  // represents which player is to move:
  // false means black, true means white
  bool turn;
  
  // did any player pass yet?
  bool passed;  
  
  /// does NOTHING; call reset() to initialize
  board();
  
  /// copy ctor
  board(const board& b);

  /// move ctor
  board(const board&& b);
  
  /// can be used to create board object from string of board::to_string()
  board(const std::string& in);

  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// checks for board equality
  bool operator==(const board& b) const;
  
  /// checks for ordering
  bool operator<(const board& b) const;
 
  /// resets the board to starting position
  void reset();
  
  /// switches me and opp and inverses turn
  void switch_turn();
  
  /// checks whether for *this and this->turn, field_id is a valid move
  bool is_valid_move(int field_id) const;
  
  
  /// out will represent a bitset in which each set bit represents a valid move
  bits get_valid_moves() const;
  
  board do_random_moves(int count) const;
  
  /// gets all children from this board
  /// returns a possibly increased out pointer
  board* get_children(board* out) const;
  
  /// returns whether this board has any children for this->turn
  bool has_valid_moves() const;
  
  /// returns the number of children, without calculating the actual children
  int count_valid_moves() const;
  

  bits get_empty_fields() const;
  bits get_non_empty_fields() const;
  int count_discs() const;
  int count_empty_fields() const;
  
  /// get hash
  long long unsigned hash() const;
  
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference positive means this->turn has more
  int get_disc_diff() const;
  
  /// does a move, returns flipped discs
  bits do_move(int field_id);
  
  /// experimental code for making specific move functions for each field
  bits do_move_field_id(int field_id);
  
  
  /// recovers a board state before move field_id, with flipped discs in undo_data 
  void undo_move(int field_id,bits undo_data); 
  
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
  opp(std::move(b.opp)),
  turn(std::move(b.turn)),
  passed(std::move(b.passed))
{
}


inline board::board()
{
}

inline void board::reset(){
  
  me = bit[28] | bit[35];
  opp = bit[27] | bit[36];
  
  passed = false;
  turn = false;
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
  turn = !turn;
  std::swap<bits>(me,opp);
}


inline bool board::operator<(const board& b) const
{
  return this->to_database_string() < b.to_database_string();
}


inline long long unsigned board::hash() const
{
  return 3*me + 5*opp + 7*turn + 11*(passed ? 1 : 0);
}

inline bits board::get_empty_fields() const
{
  return ~get_non_empty_fields();
}

inline bits board::get_non_empty_fields() const
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
  return (get_valid_moves() & board::bit[field_id]) != 0ull;
}

inline void board::undo_move(int field_id,bits undo_data)
{
  switch_turn();
  
  me &= ~(undo_data | board::bit[field_id]);
  opp |= (undo_data);  
  
  
  assert((me & undo_data).none());
  assert((opp & undo_data) == undo_data);
  assert((get_non_empty_fields() & board::bit[field_id]) == 0ull);
}

inline bits board::get_valid_moves() const
{
  bits res = 0ull;
  
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

