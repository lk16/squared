#pragma once

#include "bots/bot_base.hpp"

struct bot_random:
  public bot_base
{
  bot_random();  
  
  virtual void do_move(const board* in, board* out);
  virtual void on_new_game();
  virtual void disable_book();
  virtual void disable_shell_output();
  virtual ~bot_random();
};