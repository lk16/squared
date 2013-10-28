#include "game_control.hpp"
#include "gui/main_window.hpp"

game_control::game_control(main_window* _mw):
  mw(_mw)
{
  bot[0] = bot[1] = NULL;
  Glib::signal_timeout().connect(sigc::mem_fun(*this,&game_control::timeout_handler),200);
}

game_control::~game_control()
{
  remove_bot(-1);
  remove_bot(1);
}


int game_control::turn() const
{
  return current.turn;
}

int game_control::turn_to_index() const
{
 return (turn()==-1 ? 0 : 1); 
}



void game_control::on_human_do_move(int field_id)
{
  if(get_bot_to_move()){
    return;
  }

 
 if(current.is_valid_move(field_id)){
    undo_stack.push(current);
    std::bitset<64> dummy;
    current.do_move(field_id,&dummy);
    on_any_move();
  }
}

void game_control::on_bot_do_move()
{
  if(is_bot(-1) && is_bot(1)){
    current.show();
  }

  if(!current.has_children()){
    return;
  }
  
  board old = current;
  
  bot[turn_to_index()]->do_move(&old,&current);
  undo_stack.push(old);
  on_any_move(); 
}

void game_control::on_any_move()
{
  current.check_do_move_experimental();
  
  while(!redo_stack.empty()){
    redo_stack.pop();
  }
  
  if(!current.has_children()){
    current.switch_turn();
    if(!current.has_children()){
      on_game_ended();
    }
  }
  mw->update_fields();
}



void game_control::on_undo()
{
  while(!undo_stack.empty() && is_bot(undo_stack.top().turn)){
    redo_stack.push(current);
    current = undo_stack.top();
    undo_stack.pop();
  } 
  
  if(undo_stack.empty()){
    mw->update_status_bar("Cannot undo");
  }
  else{
    redo_stack.push(current);
    current = undo_stack.top();
    undo_stack.pop();
  }
  mw->update_fields();
}

void game_control::on_redo()
{ 
  while(!redo_stack.empty() && is_bot(redo_stack.top().turn)){
    undo_stack.push(current);
    current = redo_stack.top();
    redo_stack.pop();
  } 
  
  if(redo_stack.empty()){
    mw->update_status_bar("Cannot redo");
  }
  else{
    undo_stack.push(current);
    current = redo_stack.top();
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
  
  current.reset();
  mw->update_fields();
  mw->update_status_bar(std::string("A new game has started."));
}

void game_control::on_game_ended()
{
  std::string text;
  
  int b_count = current.opp.count();
  int w_count = current.me.count();
  
  if(current.turn == -1){
    std::swap(b_count,w_count);
  }
  
  
  text += "Game has ended. White (" + tostr<int>(w_count) + ") - Black (";
  text += tostr<int>(b_count)+ ")";
  
  std::cout << text << std::endl;
  mw->update_status_bar(text);
}

bool game_control::timeout_handler()
{
  if(!current.has_children()){
    current.switch_turn();
    if(!current.has_children()){
      current.switch_turn();
      return true;
    }
    current.switch_turn();
  }
  if(bot[turn_to_index()]){
    //mw->update_status_bar("I'm thinking...");
    on_bot_do_move();  
  }
  else{
    //mw->update_status_bar("It is your turn.");
  }
  return true;
}

void game_control::add_bot(int _c, int d,int pd)
{
  int index = (_c == -1 ? 0 : 1);
  if(bot[index]){
    delete bot[index];
  }
  bot[index] = new bot_ali(_c,d,pd);
}

void game_control::remove_bot(int col)
{
  if(bot[col]){
    delete bot[col];
    bot[col] = NULL;
  }
}

bot_base* game_control::get_bot_to_move()
{
  return bot[current.turn==-1 ? 0 : 1];
}

bool game_control::is_bot(int color)
{
  assert(color==1 || color==-1);
  int index = (color==-1 ? 0 : 1);
  return bot[index] != NULL;
}

