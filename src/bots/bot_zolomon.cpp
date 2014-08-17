#include "bots/bot_zolomon.hpp"

REGISTER_BOT(zolomon);

bot_zolomon::bot_zolomon()
{
  set_name("zolomon");  
  book = new book_t(BOOK_PATH + get_name() + "_book.csv");
}

bot_zolomon::~bot_zolomon()
{
  if(book){
    delete book;
  }
}

void bot_zolomon::on_new_game()
{
  book->reload();
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
    bits64_count(inspected.me & board::location[0]) 
    - bits64_count(inspected.opp & board::location[0])
  );
  
  res += 12 * (
    bits64_count(inspected.me & 0x7e8181818181817e)
    - bits64_count(inspected.opp & 0x7e8181818181817e)
  );
  
  int me = bits64_count(inspected.me);
  int opp = bits64_count(inspected.opp);
  
  int x = (me > opp) ? 1 : ((me == opp) ? 0 : -1);
  
  res += 20 * x;
  
  return res;
}

