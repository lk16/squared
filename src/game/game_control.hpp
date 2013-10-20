#ifndef SQUARED_GAMECONTROL_HPP
#define SQUARED_GAMECONTROL_HPP

#include <stack>
#include <thread>

#include <glibmm/main.h>

#include "game/board.hpp"
#include "bots/bot_base.hpp"
#include "bots/bot_ali.hpp"

class main_window;

struct game_control{
  
  /// NULL pointer means human player 
  bot_base* bot[2]; 
  
  main_window* mw;
  board current;
  
  std::stack<board> undo_stack,redo_stack;
  
  game_control(main_window* mw);
  ~game_control();
  
  void add_bot(int color, int _depth, int _perfect_depth);
  void remove_bot(int color);
  bot_base* get_bot_to_move();
  bool is_bot(int color); // color should be -1 or 1
  
  void on_new_game();
  void on_game_ended();
  
  void on_undo();
  void on_redo();
  
  void on_human_do_move(int field_id);
  void on_bot_do_move();
  void on_any_move();
  
  bool timeout_handler();
  
  int turn_to_index() const;
  int turn() const;
  
};



#endif
