#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>

#include <unistd.h>

#include "game/board.hpp"
#include "bots/bot_base.hpp"
#include "util/csv.hpp"

struct book_t{

  struct value{
    int best_move,depth;
  };
  
  typedef std::map<std::string,value> data_type;
  typedef data_type::const_iterator citer;
  typedef data_type::iterator iter;

  static const int min_learn_depth = 12;
  static const int entry_max_discs = 24;

  data_type data;
  std::string filename;
  
  book_t(const std::string& _filename);

  
  
  int get_move_index(const board* before,const board* after);
  
  void learn(bot_base* bot);

  int learn_move(bot_base* bot,const std::string& board_str,int depth,int n_left);

  void add_to_book_file(const std::string& b,int depth,int move);
  
  void print_stats() const;
  
  void remove_obsolete_lines() const;
};
