#include "bots/bot_moves.hpp"

REGISTER_BOT(moves);

bot_moves::bot_moves(){
  set_name("moves");
  book = new book_t(BOOK_PATH + get_name() + "_book.csv");
}


bot_moves::~bot_moves(){  
}

int bot_moves::heuristic()
{
  int res = inspected.count_valid_moves();
  inspected.switch_turn();
  int opp_move_count = inspected.count_valid_moves();
  if(res==0 && opp_move_count==0){
    // end of game, return exact score
    return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
  }
  inspected.switch_turn();
  res -= opp_move_count;
  
  // add 3 x amount of corners
  res += 3 *(
      bits64_count(inspected.me & board::location[board::X_SQUARES]) 
      - bits64_count(inspected.opp & board::location[board::X_SQUARES])
  );
  
  return res;
}

  
void bot_moves::on_new_game(){
  if(get_use_book()){
    book->reload();
  }
}
