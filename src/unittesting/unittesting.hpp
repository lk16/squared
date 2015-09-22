#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "game/board.hpp"
#include "util/math.hpp"

class squared_unittest{
  void announce(const std::string& name);
  void announce_step();
  
  
public:
  
  // run all unittests
  void run();
};
void squared_unittesting();