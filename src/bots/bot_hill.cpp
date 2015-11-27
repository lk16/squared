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
    output() << "move " << (id+1) << "/" << "TODO";
    output() << " (" << "TODO" << ')';
    output() << ": " << best_heur << '\n';
    ++id;
  }     
}

int bot_hill::hill_climbing(board* b,int d,int pd){
  int turn = 0;
  bot_moves* movesbot = dynamic_cast<bot_moves*>(bot_registration::bots()["moves"]());
  if(!movesbot){
    output() << "ERROR! cannot cast bot_base to bot_moves in bot_hill::hill_climbing()\n";
    exit(1);
  }
  movesbot->disable_shell_output();
  movesbot->set_search_depth(d,pd);
  board dummy;
  while(b->count_empty_fields() > pd){
    board best_child;
    int best_heur = MIN_HEURISTIC;
    board children[32];
    board* child_end = b->get_children(children);
    for(const board* child = children;child != child_end;++child){
      board copy = *child;
      movesbot->do_move(child,&dummy);
      int heur = movesbot->last_heuristic();
      if(heur > best_heur){
        best_heur = heur;
        best_child = *child;
      }
    }
    *b = best_child;
    turn = 1 - turn;
  }
  movesbot->do_move(b,&dummy);
  int heur = movesbot->last_heuristic();
  delete movesbot;
  return heur;  
}



void bot_hill::on_new_game(){
  return;
}


