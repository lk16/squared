#include <gtkmm.h>

#include "gui/main_window.hpp"
#include "misc/testing.hpp"
#include "misc/timing.hpp"




// returns positive values if white won
int play_game(bot_base* b_player,bot_base* w_player){
 
  if(b_player->get_color()!=-1 || w_player->get_color()!=1){
    std::cout << "Invalid color configuration in play_game\n";
    return 0;
  }
  
  board b;
  b.reset();
  
  bot_base* turn = b_player;
  
  while(true){
    
    if(!b.has_children()){
      b.switch_turn();
      turn = (turn==b_player ? w_player : b_player);
      if(!b.has_children()){
        return b.get_disc_diff();
      }
    }
    
    board tmp;
    turn->do_move(&b,&tmp);
    b = tmp;
    
    turn = (turn==b_player ? w_player : b_player);
  }
}

void simulation(){
  bot_ali champion(-1,8,16);
  bot_ali challanger(1,8,16);
  
  champion.disable_shell_output();
  challanger.disable_shell_output();
  
  // bot_ali original:          {313,87,-57,-21,-16,-53,-49,-23,18,50} 
  // output after ran 1 hour    {328,51,-39,-36,-16,-62,-65,-12,39,36}
  // 30 oct 00:59               {328,24,-33,-20,-25,-79,-87,13,106,54}
  // 30 oct 19:38               {299,-68,-74,-32,-29,-85,-61,8,48,14}
  int best_values[10] = {328,51,-39,-36,-16,-62,-65,-12,39,36};
  int chal_values[10] = {328,51,-39,-36,-16,-62,-65,-12,39,36};
  
  champion.set_loc_values(best_values);
  challanger.set_loc_values(chal_values);
  
  int score = -999;
  
  while(true){
    
    if(score > 0){
      std::cout << "values: {";
      for(int i=0;i<9;i++){
        std::cout << best_values[i] << ',';
      }
      std::cout << best_values[9] << "}\n";
    }
    else{
      std::cout << ".\n";
    }
    
    
    score = 0;
    score += play_game(&champion,&challanger);
    champion.swap_color();
    challanger.swap_color();
    score -= play_game(&challanger,&champion);
    champion.swap_color();
    challanger.swap_color();
    
    if(score > 0){
      champion.set_loc_values(chal_values);
      for(int i=0;i<10;i++){
        best_values[i] = chal_values[i];
      }
    }
    else{
      for(int i=0;i<10;i++){
        chal_values[i] = best_values[i];
      }
    }
    chal_values[rand()%10] += ( (rand()%30) - (rand()%30));
    
    challanger.set_loc_values(chal_values);
    
    
    
    
  
  }
  
}












int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1 == "testing"){
      testing_area();
    }
    else if(argv1 == "timing"){
      timing_area();
    }
    else if(argv1 == "simulate"){
      srand(time(NULL));
      simulation();
    }
    else{
      std::cout << "Invalid argument: " << argv1 << '\n' << 
      "Try one of these:\n"
      "----------.-----------------------\n"
      "testing   | run the testing_area()\n"
      "timing    | run the timing_area()\n"
      "simulate  | run simulation()\n"
      "<no args> | run the windowed game\n";
      
    }
  }
  else{
    main_window window;
    Gtk::Main::run(window);
  }
  return 0;
}
