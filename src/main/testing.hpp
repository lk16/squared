#pragma once

#include <sys/time.h>

#include "bots/bot_ali.hpp"
#include "game/board.hpp"
#include "util/math.hpp"


void print_time_diff(timeval start,timeval end,int runs,const std::string& name);
 
void testing_area();
