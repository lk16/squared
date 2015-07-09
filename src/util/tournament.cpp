#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "tournament.h"

void play_tournament(struct tournament* t)
{
  
  
  int paired[MAX_TOURNAMENT_ENTRANTS];
  struct entrant* pairs[MAX_TOURNAMENT_ENTRANTS/2][2];
  int i;
    
  while(1){
    for(i=0;i<t->num_entrants;i++){
      paired[i] = 0;
    }
    for(i=0;i< t->num_entrants;i++){
      int x;
      do{
        x = rand() % t->num_entrants;
      }while(paired[x]);
      paired[x] = 1;
      pairs[i/2][i%2] = &t->entrants[x];
    }
    for(i=0;i<(t->num_entrants/2);i++){
      enum game_result res;
      struct board b;
      board_init(&b);
      board_do_random_moves(&b,5);
      res = play_othello_game(b,&pairs[i][0]->p,&pairs[i][1]->p);
      entrant_update_rating(pairs[i][0],pairs[i][1],res);
      // play same board different colour
      res = play_othello_game(b,&pairs[i][1]->p,&pairs[i][0]->p);
      entrant_update_rating(pairs[i][1],pairs[i][0],res);
      
      qsort(t->entrants,t->num_entrants,sizeof(struct entrant),entrant_compare);
      show_tournament_stats(t);
      
    }
  }
}

enum game_result play_othello_game(struct board b,struct player* black,struct player* white)
{
  struct player* p[2] = {black,white};
  int turn = 0;
  while(1){
    if(!board_has_valid_moves(&b)){
      board_switch_turn(&b);
      turn = 1-turn;
      if(!board_has_valid_moves(&b)){
        board_switch_turn(&b);
        turn = 1-turn;
        break;
      }
    }
    player_do_move(p[turn],&b,&b);
    turn = 1-turn;
  }
  int me,opp;
  me = uint64_count(b.me);
  opp = uint64_count(b.opp);
  if(me>opp){
    return turn ? WHITE_WINS : BLACK_WINS;
  }
  if(opp>me){
    return turn ? BLACK_WINS : WHITE_WINS;
  }
  return DRAW;
}

void entrant_update_rating(struct entrant* black,struct entrant* white,enum game_result res)
{
  struct entrant* p[2] = {black,white};
  float score[2],expected[2];
  switch(res){
    case DRAW: score[0] = 0.5; break;
    case WHITE_WINS: score[0] = 0.0; break;
    case BLACK_WINS: score[0] = 1.0; break;
    default: score[0] = 0.0; assert(0); break;
  }
  score[1] = 1.0 - score[0];  
  
  expected[0] = 1.0/(1.0+(pow(10.0,(p[1]->rating-p[0]->rating)/400.0)));
  int diff = (int)(K_FACTOR*(score[0]-expected[0]));
  p[0]->rating += diff;
  p[1]->rating -= diff;

}

void show_tournament_stats(const struct tournament* t)
{
  int i;
  printf("%s","--------------------------\n");
  for(i=0;i<t->num_entrants;i++){
    printf("%d\t%s\n",t->entrants[i].rating,t->entrants[i].name);
  }
}


int entrant_compare(const void* lhs, const void* rhs)
{
  return ((struct entrant*)rhs)->rating - ((struct entrant*)lhs)->rating;
}

void tournament_add(struct tournament* t,enum player_type type, int depth, int perfect_depth,FILE* out)
{
  if(t->num_entrants == MAX_TOURNAMENT_ENTRANTS){
    return;
  }
  struct entrant* ent;
  ent = &t->entrants[t->num_entrants];
  t->num_entrants++;
  snprintf(ent->name,40,"bot_%s (%d, %d)",player_type_to_name(type),depth,perfect_depth);

  
  player_init(&ent->p,type,depth,perfect_depth);
  
  ent->rating = TOURNAMENT_INIT_RATING;
  player_redirect_output(&ent->p,out);
}




int main(void){
  struct tournament t;
  t.num_entrants = 0;
  
  FILE* devnull = fopen("/dev/null","w");
  
  int i;
  
  
  
  for(i=1;i<=7;i++){
    tournament_add(&t,PLAYER_BOT_MOVES,i,2*i,devnull);
    tournament_add(&t,PLAYER_BOT_STABLE,i,2*i,devnull);
    tournament_add(&t,PLAYER_BOT_MOVES,i,2*i,devnull);
    tournament_add(&t,PLAYER_BOT_STABLE,i,2*i,devnull);
  }
  
  //tournament_add(&t,PLAYER_HUMAN,0,0,stdout);
  
  
  play_tournament(&t);
  
  return 0;  
}
