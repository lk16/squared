#include "gamecontrol.hpp"
#include "mainwindow.hpp"

game_control::game_control(main_window* _mw):
  mw(_mw),
  last_move(std::time(NULL))
{
  current = new board();
  bot[BLACK] = NULL;
  bot[WHITE] = NULL; 
  Glib::signal_timeout().connect(sigc::mem_fun(*this,&game_control::timeout_handler),100);
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

void game_control::on_human_do_move(int x, int y)
{
  board *move;
  
  if(bot[turn()]){
    return;
  }  
  if(current->is_valid_move(x,y,turn())){
    move = new board(current->do_move(x,y));
    on_any_move(move);
   }
}

void game_control::on_bot_do_move()
{
  board *move;
  if(!current->has_moves(turn())){
    return;
  }
  if(last_move == std::time(NULL)){
    return;
  }
  move = bot[turn()]->do_move(current);
  last_move = std::time(NULL);
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
  
  if(!current->has_moves(turn())){
    if(current->has_moves(opponent(turn()))){
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
    mw->update_fields();
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
    mw->update_fields();
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
  
  text += "Game has ended. White (" + tostr<int>(current->count_discs(WHITE)) + ") - Black (";
  text += tostr<int>(current->count_discs(BLACK)) + ")";
  
  
  std::cout << text << std::endl;
  mw->update_status_bar(text);
}

bool game_control::timeout_handler()
{
  if(bot[turn()]){
    on_bot_do_move();  
  }
  return true;
}


