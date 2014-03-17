#ifndef SQUARED_MISC_TESTING_HPP
#define SQUARED_MISC_TESTING_HPP

#include <sys/time.h>
#include "game/board.hpp"

void print_time_diff(timeval start,timeval end,int runs,const char* name);
 
void testing_area();

#endif