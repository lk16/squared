#pragma once

#include <bitset>
#include <cassert>
#include <string.h>
#include <functional>
#include <string>

#include "util/const.hpp"
#include "util/csv.hpp"
#include "util/math.hpp"
#include "util/macros.hpp"
#include "util/string.hpp"
#include "bots/bot_pvs.hpp"
#include "book/book.hpp"

struct bot_ali:
  public bot_pvs
{  
  // these values pretty much define the behaviour of bot_ali
  static const int location_values[10];
  
  // ctor
  bot_ali();

  // dtor
  ~bot_ali() = default;
  
  // does something when a new game starts
  virtual void on_new_game();
  
  // calculates the heuristic for this->inspected
  int heuristic();

  
};

