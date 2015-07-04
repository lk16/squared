#include "bots/bot_mtdf_moves.hpp"

REGISTER_BOT(mtdf_moves);

bot_mtdf_moves::bot_mtdf_moves(){
  set_name("mtdf_moves");
}


bot_mtdf_moves::~bot_mtdf_moves(){  
}

int bot_mtdf_moves::heuristic()
{
  int res = inspected.count_valid_moves();
  int opp_move_count = inspected.count_opponent_moves();
  if(res==0 && opp_move_count==0){
    return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
  }
  res -= opp_move_count;
  
  res += 3 *(
      bits64_count(inspected.me & board::location[board::X_SQUARES]) 
      - bits64_count(inspected.opp & board::location[board::X_SQUARES])
  );
  
  return res;
}

  
void bot_mtdf_moves::on_new_game(){
  bot_base::on_new_game();
}
