#include "bots/bot_zolomon.hpp"

REGISTER_BOT(zolomon);

bot_zolomon::bot_zolomon()
{
  set_name("zolomon");  
}


void bot_zolomon::on_new_game()
{
  bot_base::on_new_game();
}

int bot_zolomon::heuristic()
{
  if(!inspected.has_valid_moves()){
    board copy = inspected;
    copy.switch_turn();
    if(!copy.has_valid_moves()){
      return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
    }
  }
  
  int res = 0;
  
  res += 15 * (
    (inspected.me & board::location[board::X_SQUARES]).count() 
    - (inspected.opp & board::location[board::X_SQUARES]).count()
  );
  
  res += 12 * (
    (inspected.me & bits64(0x7e8181818181817e)).count()
    - (inspected.opp & bits64(0x7e8181818181817e)).count()
  );
  
  int me = inspected.me.count();
  int opp = inspected.opp.count();
    
  res += (me > opp) ? 20 : ((me == opp) ? 0 : -20);
  
  return res;
}

