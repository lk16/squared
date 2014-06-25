#pragma once

#include <cstdlib>

#include "util/csv.hpp"
#include "bots/bot_ali.hpp"

struct bot_ali_trainer{
  
  bot_ali *start,*best,test;
  std::vector<board> openings;
  
  bot_ali_trainer(bot_ali* ali):
    test(0,0)
  {
    start = best = ali;
    ali->disable_book();
    ali->disable_shell_output();
    
    csv op("sample");
    while(true){
      csv::line_t line = op.get_line();
      if(op.get_file()->fail()){
        break; 
      }
      openings.push_back(board(line[0]));
    }
  }
  
  int play_game(board* start,bot_ali* p1,bot_ali* p2);
  
  void run();
  
  void print_bot_location_values(const bot_ali* a);

  std::vector<bot_ali*> procreate(const bot_ali*, int);

  int play_match(bot_ali*,bot_ali*);
};
