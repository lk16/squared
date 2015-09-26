#include "bots/bot_pluto.hpp"

REGISTER_BOT(pluto);

bot_pluto::bot_pluto()
{
  set_name("pluto");
}



int bot_pluto::heuristic()
{
  bits64 my_moves = inspected.get_valid_moves();
  board copy = inspected;
  copy.switch_turn();
  bits64 opp_moves = copy.get_valid_moves();

  if(my_moves.none() && opp_moves.none()){
    return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
  }
  
  int res = inspected.get_mobility(my_moves) - copy.get_mobility(opp_moves);
  
  res += 3 *(
    (inspected.me & board::location[board::X_SQUARES]).count()
    - (inspected.opp & board::location[board::X_SQUARES]).count()
  );
  
  return res;
}

  
void bot_pluto::on_new_game(){
  bot_base::on_new_game();
}
