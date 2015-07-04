#pragma once

#include <unordered_map>
#include <string>

#define REGISTER_BOT(t) bot_registration REGISTER_##t(\
  #t,\
  &create_bot<bot_##t>\
)

struct bot_base;

template<class T>
bot_base* create_bot(){
  return new T;
}

struct bot_registration{
  
  typedef std::unordered_map<std::string,bot_base*(*)()> t_map;
  static t_map& bots();
  
  bot_registration(const char* key,bot_base*(*value)()){
    bots()[key] = value;
  }
};