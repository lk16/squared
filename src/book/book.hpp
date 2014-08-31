#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <queue>

#include <unistd.h>

#include "game/board.hpp"
#include "util/csv.hpp"

struct bot_base;

class book_t{

public:
  struct value{
    static const unsigned NEEDED_COLUMNS = 3;
    
    value(const csv::line_t&);
    value(int bm,int d,int h);
    value(){} // needed by STL
    
    void add_to_line(csv::line_t* l) const;
    
    int best_move,depth,heur;
  };
  
  struct job_t{
    int priority;
    board b;
    value info;
    
    job_t(const board& _b,const value& _info,int book_stddev);    
    bool operator<(const job_t& j) const;
    void assign_priority(int book_stddev);
  };
  
  typedef std::map<std::string,value> data_type;
  typedef data_type::const_iterator citer;
  typedef data_type::iterator iter;

  static const int MIN_LEARN_DEPTH = 10;
  static const int ENTRY_MAX_DISCS = 24;
  static const int NOT_FOUND = -1;
  static const int RELOAD_INTERVAL = 300;

private:
  
  data_type data;
  csv csv_file;

public:
  book_t();
  
  book_t(const std::string& _filename);  
  
  bool add(const board* b,const book_t::value* bv);

  void clean() const;
  
  void learn(bot_base* bot);
  
  value lookup(const board* b,int min_depth);
  
  void reload();
  
  std::string get_filename() const;
  
  void set_csv_file(const std::string& filename);
  
  int get_heur_stddev() const;
  
private:
  value do_job(bot_base* bot,const job_t* job);
  
  void print_stats() const;
  
  bool is_correct_entry(const std::string& bs,const book_t::value& bv) const;
  
  void load();
};
