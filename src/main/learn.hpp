#pragma once

#include <iostream>
#include <map>

#include "game/board.hpp"
#include "bots/bot_ali.hpp"
#include "util/csv.hpp"

struct book_value{
  int best_move,depth;
};

typedef std::map<std::string,book_value> book_t;

void learn_book();

book_t read_book();