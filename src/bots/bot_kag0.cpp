#include "bots/bot_kag0.hpp"

REGISTER_BOT(kag0);

bot_kag0::bot_kag0()
{
  set_name("kag0");  
  book = new book_t(BOOK_PATH + get_name() + "_book.csv");
}

bot_kag0::~bot_kag0()
{
  if(book){
    delete book;
  }
}

void bot_kag0::on_new_game()
{
  book->reload();
}

int bot_kag0::heuristic()
{
  if(!inspected.has_valid_moves()){
    board copy = inspected;
    copy.switch_turn();
    if(!copy.has_valid_moves()){
      return EXACT_SCORE_FACTOR * inspected.get_disc_diff();
    }
  }
  
  int res = 0;
  
  res += 30 * get_forfeit();
  res += 2 * get_mobility();
  res += 2 * get_frontier();
  res += 5 * inspected.get_disc_diff();
  
  return res;
}

int bot_kag0::get_forfeit() const
{
  if(!inspected.has_valid_moves()){
    return -1;
  }
  board copy = inspected;
  copy.switch_turn();
  if(!copy.has_valid_moves()){
    return 1;
  }
  return 0;
}

int bot_kag0::get_mobility() const
{
  return inspected.count_valid_moves() - inspected.count_opponent_moves();
}

int bot_kag0::get_frontier() const
{
  bits64 opp_frontier = 0ull;
  bits64 non_empty = inspected.get_non_empty_fields();
  
  opp_frontier |= ((inspected.opp << 9) & (non_empty << 18) & 0xFEFEFEFEFEFEFEFE);
  opp_frontier |= ((inspected.opp << 8) & (non_empty << 16) & 0xFEFEFEFEFEFEFEFE);
  opp_frontier |= ((inspected.opp << 7) & (non_empty << 14) & 0xFEFEFEFEFEFEFEFE);
  opp_frontier |= ((inspected.opp << 1) & (non_empty << 1) & 0xFEFEFEFEFEFEFEFE);
  
  opp_frontier |= ((inspected.opp >> 9) & (non_empty >> 18) & 0x7F7F7F7F7F7F7F7F);
  opp_frontier |= ((inspected.opp >> 8) & (non_empty >> 16) & 0x7F7F7F7F7F7F7F7F);
  opp_frontier |= ((inspected.opp >> 7) & (non_empty >> 14) & 0x7F7F7F7F7F7F7F7F);
  opp_frontier |= ((inspected.opp >> 1) & (non_empty >> 1) & 0x7F7F7F7F7F7F7F7F);
  
  opp_frontier &= ~non_empty;
  
  bits64 me_frontier = 0ull;
  
  me_frontier |= ((inspected.me << 9) & (non_empty << 18) & 0xFEFEFEFEFEFEFEFE);
  me_frontier |= ((inspected.me << 8) & (non_empty << 16) & 0xFEFEFEFEFEFEFEFE);
  me_frontier |= ((inspected.me << 7) & (non_empty << 14) & 0xFEFEFEFEFEFEFEFE);
  me_frontier |= ((inspected.me << 1) & (non_empty << 1) & 0xFEFEFEFEFEFEFEFE);
  
  me_frontier |= ((inspected.me >> 9) & (non_empty >> 18) & 0x7F7F7F7F7F7F7F7F);
  me_frontier |= ((inspected.me >> 8) & (non_empty >> 16) & 0x7F7F7F7F7F7F7F7F);
  me_frontier |= ((inspected.me >> 7) & (non_empty >> 14) & 0x7F7F7F7F7F7F7F7F);
  me_frontier |= ((inspected.me >> 1) & (non_empty >> 1) & 0x7F7F7F7F7F7F7F7F);
    
  me_frontier &= ~non_empty;
  
  
  return bits64_count(me_frontier) - bits64_count(opp_frontier);
}



