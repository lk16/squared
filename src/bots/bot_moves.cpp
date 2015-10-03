#include "bots/bot_moves.hpp"

REGISTER_BOT(moves);

bot_moves::bot_moves(){
  //set_name("moves");
}




int bot_moves::heuristic(const board* b)
{
  int res = b->count_valid_moves();
  int opp_move_count = b->count_opponent_moves();
  if(res==0 && opp_move_count==0){
    return EXACT_SCORE_FACTOR * b->get_disc_diff();
  }
  res -= opp_move_count;
  
  res += 3 *(
      (b->me & board::location[board::X_SQUARES]).count() 
      - (b->opp & board::location[board::X_SQUARES]).count()
  );
  
  return res;
}

  
void bot_moves::on_new_game(){
  bot_base::on_new_game();
}


