#include "tournament.hpp"


void tournament_t::run()
{
  std::vector<int> pairs;
  
  for(unsigned i=0;i<entrants.size();++i){
    pairs.push_back(i);
  }
    
  while(true){
    std::random_shuffle(pairs.begin(),pairs.end());
    for(unsigned p=0;p<pairs.size()-1;p+=2){
      game_result res;
      res = play_othello_game(entrants[p].bot,entrants[p+1].bot);
      entrant_update_rating(&entrants[p],&entrants[p+1],res);
      
      std::sort(entrants.rbegin(),entrants.rend());
      show_stats();
      
    }
  }
}

tournament_t::game_result tournament_t::play_othello_game(bot_base* black, bot_base* white)
{
  bot_base* p[2] = {black,white};
  int turn = 0;
  board b;
  b.reset();
  b.xot();
  while(true){
    if(!b.has_valid_moves()){
      b.switch_turn();
      turn = 1-turn;
      if(!b.has_valid_moves()){
        b.switch_turn();
        turn = 1-turn;
        break;
      }
    }
    p[turn]->do_move(&b,&b);
    turn = 1-turn;
  }
  int me,opp;
  me = bits64_count(b.me);
  opp = bits64_count(b.opp);
  if(me>opp){
    return turn ? WHITE_WINS : BLACK_WINS;
  }
  if(opp>me){
    return turn ? BLACK_WINS : WHITE_WINS;
  }
  return DRAW;
}

void tournament_t::entrant_update_rating(tournament_t::entrant* black, tournament_t::entrant* white, tournament_t::game_result res)
{
  entrant* p[2] = {black,white};
  float score[2],expected[2];
  switch(res){
    case DRAW: score[0] = 0.5; break;
    case WHITE_WINS: score[0] = 0.0; break;
    case BLACK_WINS: score[0] = 1.0; break;
    default: score[0] = 0.0; assert(0); break;
  }
  score[1] = 1.0 - score[0];  
   
  expected[0] = 1.0/(1.0+(std::pow(10.0,(p[1]->rating-p[0]->rating)/400.0)));
  int diff = (int)(K_FACTOR*(score[0]-expected[0]));
  p[0]->rating += diff;
  p[1]->rating -= diff;
}


void tournament_t::show_stats() const
{
  std::cout << "--------------------------\n";
  for(auto e: entrants){
    std::cout << e.rating << '\t' << e.nick << '\n';
  }
}


void tournament_t::add_entrant(const std::string& bot_name,const std::string& nick, int depth, int perfect_depth)
{
  entrant e;
  e.bot = bot_registration::bots()[bot_name]();
  e.bot->set_search_depth(depth,perfect_depth);
  e.bot->disable_book();
  e.bot->disable_shell_output();
  e.rating = TOURNAMENT_INIT_RATING;
  e.nick = nick;
  entrants.push_back(e);
  
}


tournament_t::~tournament_t()
{
  for(auto e: entrants){
    delete e.bot;
  }
}
