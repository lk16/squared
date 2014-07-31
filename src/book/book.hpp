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

class book_t{

public:
  struct value{
    static const unsigned NEEDED_COLUMNS = 3;
    
    value(const csv::line_t&);
    value(int bm,int d);
    value(){} // needed by STL
    
    int best_move,depth;
  };
  
  typedef std::map<std::string,value> data_type;
  typedef data_type::const_iterator citer;
  typedef data_type::iterator iter;

  static const int MIN_LEARN_DEPTH = 9;
  static const int ENTRY_MAX_DISCS = 24;
  static const int NOT_FOUND = -1;

private:
  
  data_type data;
  csv csv_file;

public:
  book_t();
  book_t(const std::string& _filename);

  void add(const board* before,const board* after,int depth);

  void clean() const;
  
  void learn(bot_base* bot);

  void learn_parallel(bot_base* bot,int threads);
  
  value lookup(const board* b,int min_depth);
  
  void reload();
  
  std::string get_filename() const;
  
  std::vector<book_t*> split(int n) const;
  
  void set_csv_file(const std::string& filename);
  
  bool add_entry(const std::string& bs,const book_t::value& bv);
private:
  
  int get_move_index(const board* before,const board* after);
  
  board learn_move(bot_base* bot,const board* b,int depth,int n_left);
  
  void print_stats() const;
  
  bool is_correct_entry(const std::string& bs,const book_t::value& bv) const;
  
  void load();
};
