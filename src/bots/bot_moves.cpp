#include "bots/bot_moves.hpp"

REGISTER_BOT(moves);

bot_moves::bot_moves(){
  search_max_discs = -1;
}


bot_moves::~bot_moves(){  
}

int bot_moves::heuristic(const board* b)
{
  int x,y;
  x = b->count_valid_moves();
  board copy = *b;
  copy.switch_turn();
  y = copy.count_valid_moves();
  if(x==0 && y==0){
    return EXACT_SCORE_FACTOR * b->get_disc_diff();
  }
  return x-y;
}

int bot_moves::alpha_beta(const board* b, int alpha, int beta)
{
  if(b->count_discs() >= search_max_discs){
    return heuristic(b);
  }
  
  board children[32];
  board* children_end = b->get_children(children);

  if(children_end == children){
    board copy = *b;
    copy.switch_turn();
    if(copy.has_valid_moves()){
      return -alpha_beta(&copy,-beta,-alpha);
    }
    return heuristic(b);
  }
  
  for(board* it=children;it!=children_end;it++){
    int heur = -alpha_beta(it,-beta,-alpha);
    if(heur > alpha){
      alpha = heur;
    }
    if(alpha >= beta){
      break;
    }
  }
  return alpha;
}

void bot_moves::do_move(const board* in,board* out){
  if(64 - in->count_discs() <= perfect_depth){
    search_max_discs = 64;
  }
  else{
    search_max_discs = in->count_discs() + search_depth;
  }
  
  std::cout << "bot_moves searching at depth " << (search_max_discs-in->count_discs()) << '\n';
  
  int best_heur = MIN_HEURISTIC;
  int best_index = -1;
  
  board children[32];
  board* children_end = in->get_children(children);
  
  if(children + 1 == children_end){
    *out = children[0];
    return;
  }
  
  
  for(board* it=children;it!=children_end;it++){
    int heur = -alpha_beta(it,MIN_HEURISTIC,-best_heur);
    if(heur > best_heur){
      best_index = it - children;
      best_heur = heur;
    }
    std::cout << "move " << (it - children + 1) << "/" << (children_end-children);
    std::cout << ": " << (heur / (search_max_discs==64 ? EXACT_SCORE_FACTOR : 1)) << '\n';
  }
  
  *out = children[best_index];
}
  
void bot_moves::on_new_game(){
}

void bot_moves::disable_book(){
}

void bot_moves::disable_shell_output(){
}