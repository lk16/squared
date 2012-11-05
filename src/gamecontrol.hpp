#ifndef SQUARED_GAMECONTROLL_HPP
#define SQUARED_GAMECONTROLL_HPP

#include <stack>

#include <glibmm/main.h>

#include "bot_base.hpp"
#include "bot_random.hpp"
#include "bot_ali.hpp"
#include "board.hpp"


class main_window;

struct game_control{
  
  game_control(main_window* mw);
  ~game_control();

  void on_new_game();
  void on_game_ended();
  
  void on_undo();
  void on_redo();
  
  void on_human_do_move(int x,int y);
  void on_bot_do_move();
  void on_any_move(board* next);
  
  bool timeout_handler();
  
  color turn() const;
  
  /* NULL pointer means human player */
  bot_base* bot[2]; 
  
  main_window* mw;
  board* current;
  
  /* used as stack */
  std::stack<board*> undo_stack,redo_stack;
  
  // to give gui time to update when two bots play vs each other
  int last_move;
};

inline color game_control::turn() const
{
  return current->turn;
}


#endif
