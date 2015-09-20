#pragma once

#include <glibmm/main.h>

#include "game/board.hpp"
#include "bots/bot_base.hpp"
#include "util/pgn.hpp"
#include "unittesting/unittesting.hpp"
#include "util/tournament.hpp"

class main_window;

struct game_control{
  

  struct board_state_t{
    board b;
    int turn;
    
    void switch_turn(){
      turn = (turn==WHITE ? BLACK : WHITE);  
    }
  };
  
  
  // robots (NULL means human)
  bot_base* bot[2]; 
  
  // window
  main_window* mw;
  
  // current board state
  board_state_t *board_states,*current_state,*last_redo;
 
  
  // game modifiers  
  bool quit_if_game_over;
  bool loop_game;
  bool show_board_flag;
  bool use_xot;
  std::string board_string;
  
  // behaviour modifiers
  bool run_unit_test;
  bool run_speed_test;
  bool run_windowed_game;
  pgn_task_t* pgn_task;
  int random_moves;
  int learn_threads;
  tournament_t* tournament;
  
  
  
  // bot modifiers
  std::string bot_type;
  int search_depth,perfect_depth;
  
  
  game_control();
  ~game_control();
  
  void add_bot(int color);
  void remove_bot(int color);
  bot_base* get_bot_to_move();
  
  void on_new_game();
  void on_game_ended();
  
  void on_undo();
  void on_redo();
  
  // called when a human does move with id field_id
  void on_human_do_move(int field_id);
  
  // called when a bot is supposed to move
  void on_bot_do_move();
  
  // called at the end of on_human_do_move and on_any_move
  void on_any_move();
  
  // lets bot start thinking if any is to move
  bool timeout_handler();
  
  // main method: run special tasks and/or windowed game
  void run();
  
  // does special tasks, if any 
  // returns true if one is run
  bool do_special_tasks();
};


