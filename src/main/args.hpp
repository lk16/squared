#pragma once

#include "game/game_control.hpp"
#include "main/testing.hpp"
#include "util/arg_parser.hpp"
#include "util/pgn.hpp"
#include "util/tournament.hpp"

struct squared_args:
  public arg_parser_base<squared_args>
{
  
  squared_args(int argc,const char **argv);
  
  
  game_control* gc;
  

  
  
  // modifiers
  int show_help();
  int show_board();
  int testing_area_mask();
  int learn();
  int set_board();
  int randomize_board();
  int set_level();
  int add_black_bot();
  int add_white_bot();
  int compress_book();
  int quit_if_game_over();
  int no_book();
  int set_white_valuation();
  int set_black_valuation();
  int train_bot_ali();
  int set_bot_type();
  int loop_game();
  int speed_test();
  int process_pgn();
  int use_xot();
  int tournament_flag();
};
