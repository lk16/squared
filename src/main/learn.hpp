#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <map>

#include "game/board.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"
#include "util/types.hpp"

struct book_value{
  int best_move,depth;
};

void learn_book();

book_t read_book();

int get_move_index(const board* before,const board* after);

int learn_move(bot_base* bot,const std::string& board_str,int depth,int n,int total);