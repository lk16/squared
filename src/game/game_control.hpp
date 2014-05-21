#pragma once

#include <stack>

#include <glibmm/main.h>

#include "game/board.hpp"
#include "bots/bot_base.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"

class main_window;

struct game_control{
  
  /// NULL pointer means human player 
  bot_base* bot[2]; 
  
  main_window* mw;
  
  struct board_state_t{
    board b;
    int turn;
    
    void switch_turn(){
      turn = (turn==WHITE ? BLACK : WHITE);  
    }
  };
  
  board_state_t board_state;
  
  std::stack<board_state_t> undo_stack,redo_stack;
  
  game_control();
  game_control(const game_control& gc);
  ~game_control();
  
  void add_bot(int color, int _depth, int _perfect_depth);
  void remove_bot(int color);
  bot_base* get_bot_to_move();
  
  void on_new_game();
  void on_game_ended();
  
  void on_undo();
  void on_redo();
  
  void on_human_do_move(int field_id);
  void on_bot_do_move();
  void on_any_move();
  
  bool timeout_handler();
    
};


