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
#include "util/hash_table.hpp"
#include "bots/bot_pvs.hpp"
#include "book/book.hpp"

class bot_ali:
  public bot_pvs
{
  /* This strongly influences the behaviour of ali, should be constant
     during the entire game, but can be changed from default before the
     game starts with set_location_values_from_file */
  int location_values[10];
  
  
public:
  
  bot_ali();  
  
  ~bot_ali();
  
  // does something when a new game starts
  virtual void on_new_game();
  
  // calculates the heuristic for this->inspected
  int heuristic();
    
  bool set_location_values_from_file(const std::string& file);
  
  void set_location_values(const int* v);

  const int* get_location_values() const;
  
};

