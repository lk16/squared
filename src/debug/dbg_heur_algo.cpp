#include "dbg_heur_algo.hpp"

int minimax_dbg(const board* b,int depth){
  
  
  assert(b->turn==WHITE || b->turn==BLACK);
  
  if(depth == 0){
    return b->get_disc_diff();
  }
  
  board children[32];
  int child_count;
  b->get_children(children,&child_count);
  if(child_count == 0){
    board copy(*b);
    copy.turn = opponent(copy.turn);
    if(child_count == 0){
      return b->get_disc_diff();
    }
    else{
      return minimax_dbg(&copy,depth-1);
    }
  }
  
  int best = (b->turn==WHITE) ? -99999 : 99999;
  
  for(int i=0;i<child_count;i++){
    if(b->turn==WHITE){ 
      best = max(minimax_dbg(children+i,depth-1),best);
    }
    else{
      best = min(minimax_dbg(children+i,depth-1),best);
    }
  }
  return best;
}


int alpha_beta_dbg(const board* b,int alpha,int beta,int depth){
  
  assert(b->turn==WHITE || b->turn==BLACK);
  
  if(depth == 0){
    return b->get_disc_diff();
  }
  
  board children[32];
  int child_count;
  b->get_children(children,&child_count);
  if(child_count == 0){
    board copy(*b);
    copy.turn = opponent(copy.turn);
    if(child_count == 0){
      return b->get_disc_diff();
    }
    else{
      return alpha_beta_dbg(&copy,alpha,beta,depth-1);
    }
  }

  if(b->turn==WHITE){ 
    for(int i=0;i<child_count;i++){
      alpha = max(alpha,alpha_beta_dbg(children+i,alpha,beta,depth-1));
      if(alpha >= beta){
        return beta;
      }
    }
    return alpha;
  }
  else{
    for(int i=0;i<child_count;i++){
      beta = min(beta,alpha_beta_dbg(children+i,alpha,beta,depth-1));
      if(alpha >= beta){
        return alpha;
      }
    }
    return beta;
  }
}

int negamax_dbg(const board* b,int alpha,int beta,int depth){
  
  assert(b->turn==WHITE || b->turn==BLACK);
  
  if(depth == 0){
    return (b->turn == WHITE ? 1 : -1) * b->get_disc_diff();
  }
  
  board children[32];
  int child_count;
  b->get_children(children,&child_count);
  if(child_count == 0){
    board copy(*b);
    copy.turn = opponent(copy.turn);
    if(child_count == 0){
      return b->get_disc_diff();
    }
    else{
      /*int tmp = beta;
      beta = -alpha;
      alpha = -tmp;*/
      return -negamax_dbg(&copy,-beta,-alpha,depth-1);
    }
  }
  
  for(int i=0;i<child_count;i++){
    int val = -negamax_dbg(children+i,-beta,-alpha,depth-1);
    if(val >= beta){
      return beta;
    }
    if(val > alpha){
      alpha = val;
    }
  }
  return alpha;
}

int negamax_perfect_dbg(const board* b,int alpha,int beta){
  
  assert(b->turn==WHITE || b->turn==BLACK);
  
  board children[32];
  int child_count;
  b->get_children(children,&child_count);
  if(child_count == 0){
    board copy(*b);
    copy.turn = opponent(copy.turn);
    if(child_count == 0){
      return b->get_disc_diff();
    }
    else{
      /*int tmp = beta;
      beta = -alpha;
      alpha = -tmp;*/
      return -negamax_perfect_dbg(&copy,-beta,-alpha);
    }
  }
  
  for(int i=0;i<child_count;i++){
    int val = -negamax_perfect_dbg(children+i,-beta,-alpha);
    if(val >= beta){
      return beta;
    }
    if(val > alpha){
      alpha = val;
    }
  }
  return alpha;
}