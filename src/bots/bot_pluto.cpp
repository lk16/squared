#include "bots/bot_pluto.hpp"

REGISTER_BOT(pluto);

bot_pluto::bot_pluto()
{
  set_name("pluto");
}



int bot_pluto::heuristic()
{
  int my_moves = inspected.count_valid_moves();
  int opp_moves = inspected.count_opponent_moves();
  int disc_diff = inspected.get_disc_diff();
  if(my_moves==0 && opp_moves==0){
    return EXACT_SCORE_FACTOR * disc_diff;
  }
  
  int res = 0;
  
  res += my_moves - opp_moves;
  res -= disc_diff;
  
  return res;
}

  
void bot_pluto::on_new_game(){
  bot_base::on_new_game();
}
