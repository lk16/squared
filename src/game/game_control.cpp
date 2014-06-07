#include "game_control.hpp"
#include "gui/main_window.hpp"

game_control::game_control()
{
  bot[0] = bot[1] = NULL;
  board_state.b.reset();
  board_state.turn = BLACK;
}



game_control::game_control(const game_control& gc)
{
  bot[0] = gc.bot[0];
  bot[1] = gc.bot[1];
  board_state = gc.board_state;
  redo_stack = gc.redo_stack;
  undo_stack = gc.undo_stack;
  
  
  Glib::signal_timeout().connect(sigc::mem_fun(*this,&game_control::timeout_handler),200);
}

game_control::~game_control()
{
  remove_bot(BLACK);
  remove_bot(WHITE);
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
  board_state.switch_turn();
  
  std::cout << board_state.b.to_string() << std::endl;
  
  if(board_state.b.count_discs() < book_t::entry_max_discs){
    book_t book(BOOK_PATH + "book.csv");
    book.add_to_book_file(board_state.b.to_database_string(),0,0);
  }
  
  
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
}

void game_control::on_game_ended()
{
  //board_state.b.show();
  std::cout << board_state.b.to_string() << std::endl;
  
  int b_count = count_64(board_state.b.opp);
  int w_count = count_64(board_state.b.me);
  
  if(board_state.turn == BLACK){
    std::swap<int>(b_count,w_count);
  }
  
  
  std::string text = "Game has ended. White (" + to_str<int>(w_count) + ") - Black (";
  text += to_str<int>(b_count)+ ")";
  
  std::cout << text << std::endl;
  mw->update_status_bar(text);
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
  bot[c] = new bot_ali(d,pd);
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

