#include "bot_base.hpp"

bot_base::bot_base(color _c, int _max_depth, int _max_endgame_depth):
  max_depth(_max_depth),
  max_endgame_depth(_max_endgame_depth),
  c(_c),
  prev_move_time(std::time(NULL))
{
}

int bot_base::heuristic(const board* b)
{
  b = NULL; /* prevent compiler complaints */
  std::cout << "This is bot_base::heuristic(). Don't call me! Call my superclass instead! \n";
  CRASH;
  return 0;
}

int bot_base::get_max_depth() const
{
  return max_depth;
}

void bot_base::do_move(const board* b, board* res)
{
  std::cout << "This is bot_base::do_move(). Don't call me! Call my superclass instead! \n";
  
  // prevent compiler complaints
  b->show();
  res = 0;
  CRASH;
}
