#include "bots/bot_moves.hpp"

REGISTER_BOT(moves);

bot_moves::bot_moves(){
  search_max_discs = -1;
}


bot_moves::~bot_moves(){  
}

int bot_moves::heuristic(const board* b)
{
  int x,y,z;
  x = b->count_valid_moves();
  board copy = *b;
  copy.switch_turn();
  y = copy.count_valid_moves();
  if(x==0 && y==0){
    return EXACT_SCORE_FACTOR * b->get_disc_diff();
  }
  
  z = count_64(b->me & board::location[0]) - count_64(b->opp & board::location[0]);
  
  z *= 3;
  
  return x-y+z;
}

int bot_moves::alpha_beta(const board* b, int alpha, int beta)
{
  stats.inc_nodes();
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
  stats.start_timer();
  if(64 - in->count_discs() <= get_perfect_depth()){
    search_max_discs = 64;
  }
  else{
    search_max_discs = in->count_discs() + get_search_depth();
  }
  
  output() << "bot_moves searching at depth " << (search_max_discs-in->count_discs()) << '\n';
  
  int best_heur = MIN_HEURISTIC;
  
  // do the first move if we cannot prevent losing everything
  int best_index = 0;
  
  board children[32];
  board* children_end = in->get_children(children);
  
  if(children + 1 == children_end){
    *out = children[0];
    output() << "Only one move found, evaluation skipped.\n";
    return;
  }
  
  
  for(board* it=children;it!=children_end;it++){
    int heur = -alpha_beta(it,MIN_HEURISTIC,-best_heur);
    if(heur > best_heur){
      best_index = it - children;
      best_heur = heur;
    }
    output() << "move " << (it - children + 1) << "/" << (children_end-children);
    output() << ": " << (best_heur / (search_max_discs==64 ? EXACT_SCORE_FACTOR : 1)) << '\n';
  }
  
  stats.stop_timer();

  output() << big_number(stats.get_nodes()) << " nodes in ";
  output() << stats.get_seconds() << " seconds: ";
  output() << big_number(stats.get_nodes_per_second()) << " nodes / sec\n";

  *out = children[best_index];
}
  
void bot_moves::on_new_game(){
}

void bot_moves::disable_book(){
}

void bot_moves::disable_shell_output(){
}