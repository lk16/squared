#include "bots/bot_ali_trainer.hpp"

void bot_ali_trainer::modify_ali(const bot_ali* origin, bot_ali* target)
{
  target->set_search_depth(origin->search_depth,origin->perfect_depth);
  const int* tmp = origin->get_location_values();
  int v[10];
  std::copy(tmp,tmp+10,v);
  
  int diff = (10 - (rand() % 5));
  
  if(diff==0){ 
    diff = 5; // make clones impossible
  }
  
  int i = rand() % 10;
  int j;
  do{
    j = rand() % 10;
  }while(i == j);
  
  v[i] += diff;
  v[j] -= diff;
  
  target->set_location_values(v);
  
  
}

void bot_ali_trainer::print_bot_location_values(const bot_ali* a)
{
  const int *x = a->get_location_values();
  for(int i=0;i<10;i++){
    std::cout << x[i] << ' ';
  }
}


void bot_ali_trainer::run()
{
  std::cout << "Starting config: ";
  print_bot_location_values(best);
  std::cout << std::endl;
    
  
  
  while(true){
    modify_ali(best,&test);
    int x = play_tournament(best,&test);
    std::cout << "win/loss diff = " << x << '\n';
    if(x < 0){
        best->set_location_values(test.get_location_values());
      
        std::cout << "New champion: ";
        print_bot_location_values(best);
        std::cout << std::endl;
    }
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

int bot_ali_trainer::play_tournament(bot_ali* p1, bot_ali* p2)
{
  p1->disable_book();
  p1->disable_shell_output();
  p2->disable_book();
  p2->disable_shell_output();
  
  
  int p1_win = 0;
  int p2_win = 0;
  int draw = 0;
  int total_games = 0;
  
  
  for(const board& b: openings){
    board copy(b);
    int x = play_game(&copy,p1,p2);
    if(x > 0){
      std::cout << '+' << std::flush;
      p1_win++;
    }
    else if(x < 0){
      p2_win++;
      std::cout << '-' << std::flush;
    }
    else{
      draw++;
      std::cout << '.' << std::flush;
    }
    
    copy = b;
    x = play_game(&copy,p2,p1); 
    if(x < 0){
      std::cout << '+' << std::flush;
      p1_win++;
    }
    else if(x > 0){
      p2_win++;
      std::cout << '-' << std::flush;
    }
    else{
      draw++;
      std::cout << '.' << std::flush;
    }
    
    total_games++;
        
    if(p1_win!=0 && p2_win!=0 && total_games >= 20){
      
      const double threshold = 1.3;
      
      double ratio = ((double)p1_win)/((double)p2_win);
      if(ratio > threshold || ratio < (1.0/threshold)){
        std::cout << "\nRatio boundary exceeded";
        break;
      }
    }
    
  }       
  std::cout << '\n';
     
  return p1_win-p2_win;
}

