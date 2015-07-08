#include "game_control.hpp"
#include "gui/main_window.hpp"

game_control::game_control()
{
  quit_if_game_over = false;
  loop_game = false;
  show_board_flag = false;
  use_book = false;
  use_xot = false;
  
  run_speed_test = false;
  compress_book = false;
  pgn_task = NULL;
  random_moves = 0;
  learn_threads = 0;
  run_windowed_game = true;
  
  bot_type = "moves";
  search_depth = 10;
  perfect_depth = 16;
  
  bot[BLACK] = NULL;
  bot[WHITE] = NULL;
  
  board_state.b.reset();
  board_state.turn = BLACK;
}


game_control::~game_control()
{
  remove_bot(BLACK);
  remove_bot(WHITE);
  if(pgn_task){
    delete pgn_task;
  }
}

void game_control::run()
{
  if(do_special_tasks()){
    return;
  }
  if(run_windowed_game){
    Gtk::Main kit(0,NULL);
    main_window window;
    mw = &window;
    window.control = this;
    window.update_fields();
    connect_timeout_signal();
    on_new_game();
    Gtk::Main::run(window);
  }
}

bool game_control::do_special_tasks()
{
  if(show_board_flag){
    const game_control::board_state_t* state = &board_state;
    std::cout << state->b.to_ascii_art(state->turn);
    return true;
  }
  
  if(pgn_task){
    const pgn_task_t* task = pgn_task;
    bot_base* pgn_bot = bot_registration::bots()[bot_type]();
    pgn_bot->set_search_depth(task->search_depth,task->perfect_depth);
    pgn_bot->disable_shell_output();
    pgn pgn_file(task->filename);
    std::cout << pgn_file.analyse(pgn_bot,true,true);
    delete pgn_bot;
    return true;
  }
  
  if(run_speed_test){
    bot_base* speedrun_bot = bot_registration::bots()[bot_type]();
    speedrun_bot->disable_shell_output();
    speedrun_bot->disable_book();
    std::cout << "testing speed of bot_" << bot_type << " on this board:\n";
    std::cout << board_state.b.to_ascii_art(board_state.turn);
    for(int i=1;i<=60;i++){
      board dummy;
      speedrun_bot->set_search_depth(i,i);
      speedrun_bot->do_move(&board_state.b,&dummy);
      long long speed = speedrun_bot->stats.get_nodes_per_second();
      std::cout << "At depth " << i << ":\t" << big_number(speed) << " nodes/s ";
      std::cout << "\ttook " << speedrun_bot->stats.get_seconds() << " seconds.\n";
      if(speedrun_bot->stats.get_seconds() > 10.0){
        break;
      }
    }
    return true;
  }
  
  if(learn_threads != 0){
    std::cout << "learn book: " << BOOK_PATH + bot_type + "_book.csv" << '\n';
    book_t(BOOK_PATH + bot_type + "_book.csv").learn(bot_type,learn_threads);
    return true;
  }
  
  if(compress_book){
    book_t(BOOK_PATH + bot_type + "_book.csv").clean();
    return true;
  }
  
  return false;
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
    std::cout << board_state.b.to_ascii_art(board_state.turn);
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
  if(use_xot){
    board_state.b.xot();
  }
  
  
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
    std::cout << board_state.b.to_ascii_art(board_state.turn);
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
    if(!board_state.b.count_opponent_moves() != 0){
      return true;
    }
  }
  if(bot[board_state.turn]){
    on_bot_do_move();  
  }
  return true;
}

void game_control::add_bot(int colour)
{
  assert(colour==BLACK || colour==WHITE);
  
  if(bot[colour]){
    delete bot[colour];
  }
  bot[colour] = bot_registration::bots()[bot_type](); 
  bot[colour]->set_search_depth(search_depth,perfect_depth);
}

void game_control::remove_bot(int colour)
{
  assert(colour==BLACK || colour==WHITE);
  
  if(bot[colour]){
    delete bot[colour];
    bot[colour] = NULL;
  }
}

bot_base* game_control::get_bot_to_move()
{
  return bot[board_state.turn];
}

