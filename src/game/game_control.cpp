#include "game_control.hpp"
#include "gui/main_window.hpp"

game_control::game_control()
{
  bot[0] = bot[1] = NULL;
  board_state.b.reset();
  board_state.turn = BLACK;
  quit_if_game_over = false;
  loop_game = false;
  bot_type = "ali";
}



game_control::~game_control()
{
  remove_bot(BLACK);
  remove_bot(WHITE);
}

void game_control::connect_timeout_signal()
{
  Glib::signal_timeout().connect(sigc::mem_fun(*this,&game_control::timeout_handler),20);
}


void game_control::on_human_do_move(int field_id)
{
  if(get_bot_to_move()){
    return;
  }

 
 if(board_state.b.is_valid_move(field_id)){
    undo_stack.push(board_state);
    board_state.b.do_move(field_id);
    
    on_any_move();
  }
}

void game_control::on_bot_do_move()
{
  if(!board_state.b.has_valid_moves()){
    return;
  }
  
  board_state_t old = board_state;
  
  bot[board_state.turn]->do_move(&old.b,&board_state.b);
  undo_stack.push(old);
  on_any_move(); 
}

void game_control::on_any_move()
{  
  
  std::cout << board_state.b.to_string() << std::endl;
  
  board_state.switch_turn();
  
  while(!redo_stack.empty()){
    redo_stack.pop();
  }
  
  if(!board_state.b.has_valid_moves()){
    board_state.switch_turn();
    board_state.b.switch_turn();
    if(!board_state.b.has_valid_moves()){
      on_game_ended();
    }
  }
  mw->update_fields();
  if(bot[BLACK] && bot[WHITE]){
    board_state.b.show();
  }
}



void game_control::on_undo()
{
  while(!undo_stack.empty() && bot[(int)(undo_stack.top().turn)]){
    redo_stack.push(board_state);
    board_state = undo_stack.top();
    undo_stack.pop();
  } 
  
  if(undo_stack.empty()){
    mw->update_status_bar("Cannot undo");
  }
  else{
    redo_stack.push(board_state);
    board_state = undo_stack.top();
    undo_stack.pop();
  }
  mw->update_fields();
}

void game_control::on_redo()
{ 
  while(!redo_stack.empty() && bot[(int)(redo_stack.top().turn)]){
    undo_stack.push(board_state);
    board_state = redo_stack.top();
    redo_stack.pop();
  } 
  
  if(redo_stack.empty()){
    mw->update_status_bar("Cannot redo");
  }
  else{
    undo_stack.push(board_state);
    board_state = redo_stack.top();
    redo_stack.pop();
  }
  mw->update_fields();
}

void game_control::on_new_game()
{
  while(!redo_stack.empty()){
    redo_stack.pop();
  } 
  while(!undo_stack.empty()){
    undo_stack.pop();
  } 
  
  board_state.b.reset();
  board_state.turn = BLACK;
  mw->update_fields();
  mw->update_status_bar(std::string("A new game has started."));
  
  for(int i=0;i<2;i++){
    if(bot[i]){
      bot[i]->on_new_game();
    }
  }
}

void game_control::on_game_ended()
{
  //board_state.b.show();
  std::cout << board_state.b.to_string() << std::endl;
  
  int b_count = bits64_count(board_state.b.opp);
  int w_count = bits64_count(board_state.b.me);
  
  if(board_state.turn == BLACK){
    std::swap<int>(b_count,w_count);
  }
  
  
  std::string text = "Game has ended. White (" + to_str<int>(w_count) + ") - Black (";
  text += to_str<int>(b_count)+ ")";
  
  std::cout << text << std::endl;
  mw->update_status_bar(text);
  
  if(quit_if_game_over){
    board_state.b.show();
    mw->hide();
    exit(0);
  }
  if(loop_game){
    on_new_game();
  }
}

bool game_control::timeout_handler()
{
  if(!board_state.b.has_valid_moves()){
    board_state.switch_turn();
    if(!board_state.b.has_valid_moves()){
      board_state.switch_turn();
      return true;
    }
    board_state.switch_turn();
  }
  if(bot[board_state.turn]){
    on_bot_do_move();  
  }
  return true;
}

void game_control::add_bot(int c, int d,int pd)
{
  assert(c==0 || c==1);
  
  if(bot[c]){
    delete bot[c];
  }
  bot[c] = bot_registration::bots()[bot_type](); 
  bot[c]->set_search_depth(d,pd);
}

void game_control::remove_bot(int c)
{
  assert(c==WHITE || c==BLACK);
  
  if(bot[c]){
    delete bot[c];
    bot[c] = NULL;
  }
}

bot_base* game_control::get_bot_to_move()
{
  return bot[board_state.turn];
}

