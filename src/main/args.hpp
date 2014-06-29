#pragma once

#include "bots/bot_ali_trainer.hpp"
#include "game/game_control.hpp"
#include "util/arg_parser.hpp"
#include "main/testing.hpp"

struct squared_arg_t
{
  
  arg_parser<squared_arg_t>* parser;
  
  game_control gc;
  bool show_flag;
  bool start_windowed_game;
  bool use_book;
  std::string bot_type;
  
  squared_arg_t();
  
  void init_map();
  
  
  int set_valuation(int color);
  
  // modifiers
  int show_help();
  int show_board();
  int testing_area_mask();
  int learn();
  int set_board();
  int randomize_board();
  int set_white_level();
  int set_black_level();
  int compress_book();
  int minus_q_flag();
  int no_book();
  int set_white_valuation();
  int set_black_valuation();
  int train_bot_ali();
  int set_bot_type();
};