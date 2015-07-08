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
  
    
  struct learn_job{
        
    board b;
    int depth;
    
    learn_job() = default;
    
    learn_job(board _b,int _depth): 
      b(_b),depth(_depth){}
    
    bool operator<(const learn_job& rhs) const {
      return priority() < rhs.priority();  
    }

    int priority() const{
      if(depth < MIN_LEARN_DEPTH){
        return 99999999-depth;
      }
      
      int res = 0;
      res += -7 * b.count_discs();
      res += -10 * depth;
      return res;
    }
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

  // lowest depth for a move to be stored 
  static const int MIN_ACCEPT_DEPTH = 8;
  
  // lowest depth for a move to be learned
  static const int MIN_LEARN_DEPTH = 12;

  static const int ENTRY_MAX_DISCS = 24;
  static const int NOT_FOUND = -1;

private:
  
  // the actual container of the book data
  container_t container;
  read_write_lock container_lock;
  
  csv csv_file;
  
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
  void learn(const std::string& bot_name,unsigned threads);
  
  // learn thread
  void learn_thread(const std::string& bot_name,std::priority_queue<learn_job>* pq,std::mutex* mutex);
  
  // run a learn job
  void learn_execute_job(bot_base* bot,learn_job* job);
  
  
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