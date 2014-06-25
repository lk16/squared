#include "bots/bot_ali_trainer.hpp"

const size_t train_depth = 6;
const size_t train_optimal_depth = 8;

std::vector<bot_ali*> bot_ali_trainer::procreate(const bot_ali* father, int n_children){
  std::vector<bot_ali*> res;

  const size_t location_values_size= sizeof(bot_ali::location_values)/sizeof(father->location_values[0]);

  for(int k=0;k<n_children;k++){
    bot_ali* child = new bot_ali(train_depth, train_optimal_depth);
    res.push_back(child);
    for(size_t i=0;i<location_values_size;i++){
      child->location_values[i]+=10-rand()/(RAND_MAX/21);
    }
  }
  return res;
}

//returns a compounded score
int bot_ali_trainer::play_match(bot_ali* p1, bot_ali* p2){
  p1->disable_book();
  p1->disable_shell_output();
  p2->disable_book();
  p2->disable_shell_output();
  
  
  int p1_win = 0;
  int p2_win = 0;
  int draw = 0;
  int total_games = 0;
  
  const size_t sample_size = 20;

  print_bot_location_values(p1);
  std::cout  << " - ";
  print_bot_location_values(p2);
  std::cout  << "\t";

  for(size_t i=0;i<sample_size;i++){
    board copy = openings[rand()/(RAND_MAX/openings.size())]; 
    int x = play_game(&copy,p1,p2);
    if(x > 0){
      std::cout << '+' << std::flush;
      p1_win++;
    }
    else if(x < 0){
      std::cout << '-' << std::flush;
      p2_win++;
    }
    else{
      std::cout << '.' << std::flush;
      draw++;
    }

    total_games++;
  }
  std::cout << '\n';
  return p1_win-p2_win;
}

void bot_ali_trainer::run(){
  typedef std::vector<std::pair <bot_ali*, int> > player_list_t;
  player_list_t scores(1,std::make_pair(new bot_ali(train_depth,train_optimal_depth), 0));

  const size_t max_size=5;
  const size_t children_size=1;

  std::cout << "Starting config: ";
  print_bot_location_values(scores[0].first);
  std::cout << std::endl;
  
  while(1){
    player_list_t new_scores;
    for(std::pair<bot_ali*, int> &p : scores){
      std::vector<bot_ali*> children = procreate(p.first, children_size);
      new_scores.push_back({p.first,0});
      for(auto c : children){
        new_scores.push_back({c,0});
      }
    }

    std::swap(scores, new_scores);

    for(auto& p1 : scores){
      for(auto& p2 : scores){
        if(p1.first==p2.first) continue;

        int result=play_match (p1.first, p2.first);

        p1.second += result;
        p2.second -= result;
      }
    }
    std::sort(scores.begin(), scores.end(), [](player_list_t::value_type a, player_list_t::value_type b){
        return a.second>b.second;
    });
    
    if(scores.size()>max_size){
      for(auto i=scores.begin()+max_size;i!=scores.end();i++){
        delete i->first;
      }
      scores.erase(scores.begin()+max_size, scores.end());
    }

    std::cout << "new population: \n";
    for(auto i : scores){
      print_bot_location_values(i.first);
      std::cout << "\n";
    }
  }
}

void bot_ali_trainer::print_bot_location_values(const bot_ali* a)
{
  const int *x = a->get_location_values();
  for(int i=0;i<10;i++){
    std::cout << x[i] << ' ';
  }
}


int bot_ali_trainer::play_game(board* b, bot_ali* p1, bot_ali* p2)
{
  board tmp;
  
  bot_ali *p[2] = {p1,p2};
  int turn = 0;
  while(true){
    if(b->count_valid_moves() > 0){
      p[turn]->do_move(b,&tmp);
      *b = tmp;
      turn = 1-turn;
    }
    else{
      b->switch_turn();
      turn = 1-turn;
      if(b->count_valid_moves() > 0){
        p[turn]->do_move(b,&tmp);
        *b = tmp;
        turn = 1-turn;
      }
      else{
        turn = 1-turn;
        break;
      }
    }
  }
  return b->get_disc_diff();
}


