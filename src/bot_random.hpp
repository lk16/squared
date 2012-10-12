#ifndef SQUARED_BOT_RANDOM_HPP
#define SQUARED_BOT_RANDOM_HPP

#include <ctime>

#include "bot_base.hpp"


class  bot_random:
  public bot_base
{
public:
    bot_random(color c);
  
    virtual int heuristic(const board* b);
};

#endif