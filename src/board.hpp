#ifndef SQUARED_BOARD_HPP
#define SQUARED_BOARD_HPP

#include <bitset>
#include <cassert>
#include <iostream>
#include <list>

#include "shared.hpp"

struct board{
  std::bitset<64> discs[2];
  int id;
  color turn;
  
  /// initializes a board to starting position
  board();
  
  /// copies a board
  board(const board& b);
  
  /// assigns a board from b
  board& operator=(const board& b);
  
  /// resets the board to starting position
  void reset();
  
  /// does move (x,y) for current turn
  board do_move(int x,int y) const;
  
  /// counts the number of valid moves for color c
  int count_moves(color c) const;
  
  /// gets all children from this board
  std::list<board> get_children() const;
  
  /// counts number of discs of a given player
  int count_discs(color c) const;

  /// tests whether move (x,y) is valid for the current turn
  bool is_valid_move(int x,int y) const;
  
  /// tests whether move (x,y) is valid for color c
  bool is_valid_move(int x,int y,color c) const;
  
  /// tests wheter color c has valid moves
  bool has_moves(color c) const;
  
  /// returns maximum number of moves that could be done before the game is finished
  int max_moves_left() const;
  
  /// tests whether both players dont have any moves left
  bool test_game_ended() const;
  
  /// returns the sum of flippable discs for each valid move of player c
  int get_mobility(color c) const;
  
  /// returns number of flipped discs for move (x,y) and player c, if (x,y) is not valid, 0 is returned
  int count_flipped(int x,int y,color c) const;

  /// returns the color if disc (x,y)
  color get_color(int x,int y) const;
  
  /// tests whether disc (x,y) has color c
  bool has_color(int x,int y,color c) const;
  
  /// sets disc (x,y) to color c
  void set_color(int x,int y,color c);

  /// tests whether (x,y) is on the board
  static bool on_board(int x,int y);
    
  /// prints this to standard output, mark moves for current turn with '.'
  void show() const;
  
  /// returns disc count difference, positive means white has more
  int get_disc_diff() const;
  
};








inline board::board()
{
  reset();
}

inline void board::reset(){
  discs[0] = 0ul | (1ul << (4*8+3)) | (1ul << (3*8+4));
  discs[1] = 0ul | (1ul << (4*8+4)) | (1ul << (3*8+3));
  
  turn = BLACK;
  id = -1;
}

inline board::board(const board& b)
{
  discs[0] = b.discs[0];
  discs[1] = b.discs[1];
  id = b.id;
  turn = b.turn;
}

inline board& board::operator=(const board& b)
{
  discs[0] = b.discs[0];
  discs[1] = b.discs[1];
  id = b.id;
  turn = b.turn;
  return *this;
}

inline void board::set_color(int x,int y,color c)
{
  assert(on_board(x,y));
  switch(c){
    case BLACK:
      discs[WHITE].set(8*y+x,false);
      discs[BLACK].set(8*y+x,true);
      break;
    case WHITE:
      discs[BLACK].set(8*y+x,false);
      discs[WHITE].set(8*y+x,true);
      break;
    case EMPTY:
      discs[BLACK].set(8*y+x,false);
      discs[WHITE].set(8*y+x,false);
      break;
    default:
      /* can never happen */
      assert(0);
      break;
  }
}

inline color board::get_color(int x, int y) const
{
  return discs[BLACK].test(8*y+x) ? BLACK : discs[WHITE].test(8*y+x) ? WHITE : EMPTY;
}

inline bool board::has_color(int x, int y, color c) const
{
  return get_color(x,y)==c;
}

inline bool board::on_board(int x, int y)
{
  return x>=0 && x<=7 && y>=0 && y<=7;
}

inline bool board::has_moves(color c) const
{
  int x,y;
  for(x=0;x<8;x++){
    for(y=0;y<8;y++){
      if(is_valid_move(x,y,c)) return true;
    }
  }
  return false;
}

inline bool board::test_game_ended() const
{
  return (!has_moves(WHITE)) && (!has_moves(BLACK));
}

inline int board::count_discs(color c) const
{
  assert(c != EMPTY);
  return discs[c].count();
}

inline int board::max_moves_left() const
{
  return 64 - std::bitset<64>(discs[BLACK] | discs[WHITE]).count();
}


#endif