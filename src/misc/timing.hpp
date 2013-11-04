#ifndef SQUARED_MISC_TIMING_HPP
#define SQUARED_MISC_TIMING_HPP

#include <sys/time.h>

#include "game/board.hpp"
#include "bots/bot_ali.hpp"

void print_diff(timeval start,timeval end,int runs,const char* name);

void timing_area();

#endif