#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <queue>
#include <unordered_map>

#include <unistd.h>

#include "bots/bot_register.hpp"
#include "game/board.hpp"
#include "util/csv.hpp"
#include "util/priority_threadpool.hpp"
#include "util/read_write_lock.hpp"

struct bot_base;

class book_t{

public:
  
  enum{
    COL_BOARD = 0,
    COL_DEPTH = 1,
    COL_BEST_MOVE = 2,
    COL_HEUR = 3    
  };
  
  
  
  struct value{
    static const unsigned NEEDED_COLUMNS = 4;
    
    value(const csv::line_t&);
    value(int bm,int d,int h);
    value(){} // needed by STL
    
    void add_to_line(csv::line_t* l) const;
    
    int best_move,depth,heur;
  };
  
  typedef std::unordered_map<std::string,value> container_t;
  typedef container_t::const_iterator citer;
  typedef container_t::iterator iter;
  typedef container_t::value_type entry;

  // lowest depth for a move to be stored in the book_stddev
  static const int MIN_ACCEPT_DEPTH = 8;
  
  // lowest depth for a move to be learned
  static const int MIN_LEARN_DEPTH = 12;

  static const int ENTRY_MAX_DISCS = 24;
  static const int NOT_FOUND = -1;

private:
  
  container_t container;
  csv csv_file;
  read_write_lock container_lock;
  
  // used only for multithreaded learning
  // and should only be changed in learn()
  std::string bot_name;
  
  // used only for multithreaded learning
  // and should only be changed in learn()
  priority_threadpool* ppool;

public:
  // default ctor
  book_t();
  
  // ctor with filename
  book_t(const std::string& _filename);  
  
  // adds an entry
  bool add(const board* b,const book_t::value* bv);

  // remove redundant lines in the csv filename
  // WARNING: should not be used concurrently 
  void clean() const;
  
  // learn concurrently
  void learn(const std::string& bot_name,int threads);
  
  // run a learn job
  void learn_job(board b,int depth);
  
  
  // lookup a board
  value lookup(const board* b,int min_depth);
  
  // reloads data content from csv file
  void reload();
  
  // returns the filename of the csv
  std::string get_filename() const;
  
  // change csv file
  void set_csv_file(const std::string& filename);
  
  
private:
  static int job_priority(const board* b,int depth,int last_heur);
  
  void print_stats() const;
  
  bool is_correct_entry(const entry& e) const;
  bool is_suitable_entry(const entry& e) const;
  
  void load();
};
