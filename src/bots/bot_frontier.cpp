#include "bot_frontier.hpp"

REGISTER_BOT(frontier);

bot_frontier::bot_frontier()
{
  set_name("frontier");
}

int bot_frontier::heuristic()
{
  if(!(inspected.has_valid_moves() || inspected.count_opponent_moves()>0)){
    return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
  }
  int me,opp;
  inspected.get_frontier_discs(&me,&opp);
  return me-opp;
}

  
void bot_frontier::on_new_game(){
  return;
}

bot_frontier::~bot_frontier()
{
}
