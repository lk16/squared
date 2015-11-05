#include "bots/bot_hill.hpp"

REGISTER_BOT(hill);

bot_hill::bot_hill()
{
  set_name("hill");
}


void bot_hill::do_move(const board* b, board* res)
{
  int best_heur = MIN_HEURISTIC;
  int id = 0;
  board children[32];
  board* child_end = b->get_children(children);
  for(const board* child=children; child != child_end; ++child){
    board copy = *child;
    int current_heur = hill_climbing(&copy,get_search_depth(),get_perfect_depth()); 
    if(current_heur > best_heur){
      best_heur = current_heur;
      *res = *child;
    }
    output() << "move " << (id+1) << "/" << (child_end-children);
    output() << " (" << board::index_to_position(b->get_move_index(child)) << ')';
    output() << ": " << best_heur << '\n';
    ++id;
  }     
}

int bot_hill::hill_climbing(board* b,int d,int pd){
  int turn = 0;
  bot_moves movesbot;
  movesbot.disable_shell_output();
  movesbot.set_search_depth(d,pd);
  while(64 - b->count_discs() > pd){
    std::cout << ".";
    std::cout.flush();
    int best_heur = MIN_HEURISTIC;
    board children[32];
    board* child_end = b->get_children(children);
    if(child_end == children){
      b->switch_turn();
      if(b->has_valid_moves()){
        turn = 1 - turn;
        continue;
      }
      else{
        return -b->get_disc_diff();
      }
    }
    board best_child = children[0];
    for(const board* child = children;child != child_end;++child){
      int heur = movesbot.get_search_heuristic(child);
      if(heur > best_heur){
        best_heur = heur;
        best_child = *child;
      }
    }
    *b = best_child;
    turn = 1 - turn;
  }
  int heur = movesbot.get_search_heuristic(b);
  return heur;  
}



void bot_hill::on_new_game(){
  return;
}


