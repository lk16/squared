#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>


#include "game/board.hpp"
#include "bots/bot_base.hpp"

class pgn{
  
  static const int PASS = -1;
  static const int UNKNOWN = -2;
  
  std::map<std::string,std::string> data;
  std::vector<int> moves;
  
  // converts for example "a1" to 0
  // converts "--" to PASS
  // converts anything unrecognized to UNKNOWN
  int position_to_index(const std::string& str) const;
  
  // reverses position_to_index
  std::string index_to_position(int index) const;
  
  
  std::string get_data(const std::string& key);
  
public:
  
  // ctor from file
  pgn(const std::string& file_name);
  
  // returns analysis of game by bot
  std::string analyse(bot_base* bot,bool analyse_white,bool analyse_black);
  
  // TODO for dev/debuging purposes only
  void show() const;
};