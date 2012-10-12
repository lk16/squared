#include "bot_random.hpp"

int bot_random::heuristic(const board* b)
{
  b = NULL; /* prevent compiler complaining */  
  return (int)(rand() % 100);
}

bot_random::bot_random(color c):
  bot_base(c,1,1) /* when doing a random move deep searching is useless */
{
  srand(std::time(NULL));
}
