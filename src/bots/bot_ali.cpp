#include "bot_ali.hpp"

REGISTER_BOT(ali);

const int bot_ali::location_values[] = {347,-39,-23,-40,-119,-35,-33,-10,-7,-5};


bot_ali::bot_ali()
{
  set_name("ali");
}

int bot_ali::heuristic()
{
  int heur = 0;
  for(int i=0;i<10;++i){
    heur += bot_ali::location_values[i] * ( 
      bits64_count(inspected.me & board::location[i]) - bits64_count(inspected.opp & board::location[i]) 
    );
  }
  return heur;
}

void bot_ali::on_new_game()
{
  bot_ali::on_new_game();
}
