#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "util/string.hpp"
#include "game/board.hpp"
#include "bots/bot_base.hpp"

struct pgn_task_t{
  std::string filename;
  int search_depth,perfect_depth;
};



class pgn{
    
  std::map<std::string,std::string> data;
  std::vector<int> moves;
  
  std::string get_data(const std::string& key);
  
public:
  
  // ctor from file
  pgn(const std::string& file_name);
  
  // returns analysis of game by bot
  std::string analyse(bot_base* bot,bool analyse_white,bool analyse_black);

};