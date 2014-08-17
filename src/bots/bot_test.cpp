#include "bots/bot_test.hpp"

REGISTER_BOT(test);

bot_test::bot_test(){
  set_name("test");
}


bot_test::~bot_test(){  
}

int bot_test::heuristic()
{
  int res = inspected.count_valid_moves();
  board copy = inspected;
  copy.switch_turn();
  int opp_move_count = copy.count_valid_moves();
  if(res==0 && opp_move_count==0){
    return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
  }
  res -= opp_move_count;
  
  res += 2 *(
      bits64_count(inspected.me & board::location[0]) 
      - bits64_count(inspected.opp & board::location[0])
  );
  
  
  return res;
}

  
void bot_test::on_new_game(){
}
