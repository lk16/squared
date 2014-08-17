#include "bots/bot_register.hpp"

bot_registration::t_map& bot_registration::bots()
{
  static t_map* it=new t_map;
  return *it;
}

