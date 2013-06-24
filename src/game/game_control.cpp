#include "game_control.hpp"
#include "gui/main_window.hpp"

game_control::game_control(main_window* _mw):
  mw(_mw),
  current(new board())
{
  bot[WHITE] = bot[BLACK] = NULL;
  Glib::signal_timeout().connect(sigc::mem_fun(*this,&game_control::timeout_handler),200);
}

game_control::~game_control()
{
  while(!redo_stack.empty()){
    delete redo_stack.top();
    redo_stack.pop();
  } 
  while(!undo_stack.empty()){
    delete undo_stack.top();
    undo_stack.pop();
  } 
  delete current;
  
  if(bot[WHITE]) delete bot[WHITE];
  if(bot[BLACK]) delete bot[BLACK];
  
}


void game_control::on_human_do_move(int field_id)
{
  if(bot[turn()]){
    return;
  }

  board *move = new board;
 
  if(move != current->do_move(field_id,move)){
    on_any_move(move);
  }
}

void game_control::on_bot_do_move()
{
  if(bot[BLACK] && bot[WHITE]){
    current->show();
  }

  if(!current->has_moves()){
    return;
  }
  
  board* move = new board;
  bot[turn()]->do_move(current,move);
  on_any_move(move); 
}

void game_control::on_any_move(board* next)
{
  while(!redo_stack.empty()){
    delete redo_stack.top();
    redo_stack.pop();
  }
  undo_stack.push(current);
  
  current = next;
  mw->update_fields();
  if(!current->has_moves()){
    board copy = *current;
    copy.turn = opponent(copy.turn);
    if(copy.has_moves()){
      current->turn = opponent(turn());
      mw->update_fields();
    }
    else{
      on_game_ended();
    }
  }
}



void game_control::on_undo()
{
  if(undo_stack.empty()){
    return;
  }
  while(bot[undo_stack.top()->turn]){
    redo_stack.push(current);
    current = undo_stack.top();
    undo_stack.pop();
  }  
  redo_stack.push(current);
  current = undo_stack.top();
  undo_stack.pop();
  mw->update_fields();
}

void game_control::on_redo()
{ 
  if(redo_stack.empty()){
    return;
  } 
  while(bot[undo_stack.top()->turn]){
    redo_stack.push(current);
    current = undo_stack.top();
    undo_stack.pop();
  }
  undo_stack.push(current);
  current = redo_stack.top();
  redo_stack.pop();
  mw->update_fields();
}

void game_control::on_new_game()
{
  while(!redo_stack.empty()){
    delete redo_stack.top();
    redo_stack.pop();
  } 
  while(!undo_stack.empty()){
    delete undo_stack.top();
    undo_stack.pop();
  } 
  
  current->reset();
  mw->update_fields();
  mw->update_status_bar(std::string("A new game has started."));
}

void game_control::on_game_ended()
{
  std::string text;
  
  text += "Game has ended. White (" + tostr<int>(current->discs[WHITE].count()) + ") - Black (";
  text += tostr<int>(current->discs[BLACK].count())+ ")";
  
  std::cout << text << std::endl;
  mw->update_status_bar(text);
}

bool game_control::timeout_handler()
{
  if(current->test_game_ended()){
    return true;
  }
  if(bot[turn()]){
    mw->update_status_bar("I'm thinking...");
    on_bot_do_move();  
  }
  else{
    mw->update_status_bar("It is your turn.");
  }
  return true;
}

void game_control::add_bot(color _c, int d,int wld,int pd)
{
  if(bot[_c]){
    delete bot[_c];
  }
  bot[_c] = new bot_ali(_c,d,wld,pd);
}

void game_control::remove_bot(color col)
{
  if(bot[col]){
    delete bot[col];
    bot[col] = NULL;
  }
}
