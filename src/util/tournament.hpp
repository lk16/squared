#pragma once

#include <vector>

#include "game/board.hpp"
#include "bots/bot_base.hpp"



class tournament_t{
  
  const int TOURNAMENT_INIT_RATING = 1500;
  const double K_FACTOR = 32.0;
  
  
  enum game_result{
    WHITE_WINS = 0,
    DRAW = 1,
    BLACK_WINS = 2
  };

  struct entrant{
    bot_base* bot;
    int rating;
    int game_results[3]; // win,loss,draw
    
    std::string nick() const;
    bool operator<(const entrant& rhs) const;
  };
  
  std::vector<entrant> entrants;
  
  game_result play_othello_game(bot_base* black,bot_base* white);

  void update_entrants(entrant* black,entrant* white,game_result res);

public:
  
  tournament_t() = default;
  ~tournament_t();
  
  void run();
  void show_stats();
  void add_entrant(const std::string& bot_name,int depth,int perfect_depth);
  
};

