#pragma once

#include <vector>

#include "game/board.hpp"
#include "bots/bot_base.hpp"



class tournament_t{
  
  const int TOURNAMENT_INIT_RATING = 1500;
  const double K_FACTOR = 32.0;
  
  
  enum game_result{
    WHITE_WINS,
    BLACK_WINS,
    DRAW
  };

  struct entrant{
    bot_base* bot;
    int rating;
    std::string nick;
    
    bool operator<(const entrant& rhs) const{
      return rating < rhs.rating;
    }
  };
  
  std::vector<entrant> entrants;
  
  game_result play_othello_game(bot_base* black,bot_base* white);

  void entrant_update_rating(entrant* black,entrant* white,game_result res);

public:
  
  tournament_t() = default;
  ~tournament_t();
  
  void run();
  void show_stats() const;
  void add_entrant(const std::string& bot_name,const std::string& nick,int depth,int perfect_depth);
  
};

