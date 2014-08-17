#include "bot_ali.hpp"
#include "book/book.hpp"

#include <fstream>
#include <algorithm>

REGISTER_BOT(ali);

bot_ali::bot_ali():
  tpt(&board_hasher)
{
  set_name("ali");
  book = new book_t(BOOK_PATH + get_name() + "_book.csv");
  
  int tmp[] = {347,-39,-23,-40,-119,-35,-33,-10,-7,-5};
  std::copy(tmp,tmp+10,location_values);
}



bool bot_ali::set_location_values_from_file(const std::string& fname){
  std::fstream f(fname);
  int location_values_copy[10];
  
  if(!f.is_open()){
    return false;
  }
  
  for(int i=0;i<10;i++){
    if(!(f >> location_values_copy[i])){
      return false;
    }
  }

  set_location_values(location_values_copy);

  return true;
}

void bot_ali::set_location_values(const int* v)
{
  std::copy(v, v+10, location_values);
}




int bot_ali::heuristic()
{
  
#define LOCATION_HEUR(i) \
  bot_ali::location_values[i] * ( \
    bits64_count(inspected.me & board::location[i]) \
    - bits64_count(inspected.opp & board::location[i]) \
  )
  
  return
  LOCATION_HEUR(0) +
  LOCATION_HEUR(1) +
  LOCATION_HEUR(2) +
  LOCATION_HEUR(3) +
  LOCATION_HEUR(4) +
  LOCATION_HEUR(5) +
  LOCATION_HEUR(6) +
  LOCATION_HEUR(7) +
  LOCATION_HEUR(8) +
  LOCATION_HEUR(9);
  
#undef LOCATION_HEUR
}

bot_ali::~bot_ali(){}

void bot_ali::on_new_game()
{
  if(get_use_book()){
    book->reload();
  }
}


const int* bot_ali::get_location_values() const
{
  return location_values;
}
