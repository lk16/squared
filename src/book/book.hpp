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

#include <unistd.h>

#include "bots/bot_register.hpp"
#include "game/board.hpp"
#include "util/csv.hpp"
#include "util/priority_threadpool.hpp"

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
  
  typedef std::map<std::string,value> data_type;
  typedef data_type::const_iterator citer;
  typedef data_type::iterator iter;

  // lowest depth for a move to be stored in the book_stddev
  static const int MIN_ACCEPT_DEPTH = 8;
  
  // lowest depth for a move to be learned
  static const int MIN_LEARN_DEPTH = 12;

  static const int ENTRY_MAX_DISCS = 24;
  static const int NOT_FOUND = -1;

private:
  
  data_type data;
  csv csv_file;
  mutable std::mutex data_mutex;
  
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
  // should not be used concurrently 
  void clean() const;
  
  // learn concurrently
  void learn(const std::string _bot_name,int threads);
  
  void learn_job(board b,int depth);
  void on_job_done();
  
  // lookup a board
  value lookup(const board* b,int min_depth);
  
  // reloads data content from csv file
  void reload();
  
  // returns the filename of the csv
  std::string get_filename() const;
  
  // change csv file
  void set_csv_file(const std::string& filename);
  
  
private:
  static int job_priority(data_type::const_iterator it);
  static int job_priority(const board* b,int depth,int last_heur);
  
  void print_stats() const;
  
  bool is_correct_entry(const std::string& bs,const book_t::value& bv) const;
  
  void load();
};
