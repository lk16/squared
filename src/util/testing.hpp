#pragma once

#include <sys/time.h>

#include "game/board.hpp"

void print_time_diff(timeval start,timeval end,int runs,const char* name);
 
void testing_area();
