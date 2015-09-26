#include "game_control.hpp"
#include "gui/main_window.hpp"

game_control::game_control()
{
  quit_if_game_over = false;
  loop_game = false;
  show_board_flag = false;
  use_xot = false;
  
  run_speed_test = false;
  run_unit_test = false;
  pgn_task = NULL;
  random_moves = 0;
  learn_threads = 0;
  run_windowed_game = true;
  tournament = NULL;
  
  bot_type = "moves";
  search_depth = 10;
  perfect_depth = 16;
  
  bot[BLACK] = NULL;
  bot[WHITE] = NULL;
  
  // account for passed moves
  board_states = new board_state_t[100];
  
  current_state = last_redo = board_states;
  
  
  current_state->b.reset();
  current_state->turn = BLACK;
}


game_control::~game_control()
{
  remove_bot(BLACK);
  remove_bot(WHITE);
  if(pgn_task){
    delete pgn_task;
  }
  delete[] board_states;
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
    Glib::signal_timeout().connect(sigc::mem_fun(this,&game_control::timeout_handler),20);
    on_new_game();
    Gtk::Main::run(window);
  }
}

bool game_control::do_special_tasks()
{
  if(show_board_flag){
    const game_control::board_state_t* state = current_state;
    std::cout << state->b.to_ascii_art(state->turn);
    return true;
  }
  
  if(pgn_task){
    const pgn_task_t* task = pgn_task;
    bot_base* pgn_bot = bot_registration::bots()[bot_type]();
    pgn_bot->set_search_depth(task->search_depth,task->perfect_depth);
    pgn_bot->disable_shell_output();
    pgn pgn_file(task->filename);
    std::cout << "pgn_file.analyse() is no longer implemented\n";
    delete pgn_bot;
    return true;
  }
  if(run_unit_test){
    squared_unittest().run();
    return true;
  }
  if(run_speed_test){
    if(random_moves == 0){
      random_moves = 1;
    }
    current_state->b = current_state->b.do_random_moves(random_moves);
    
    bot_base* speedrun_bot = bot_registration::bots()[bot_type]();
    speedrun_bot->disable_shell_output();
    std::cout << "testing speed of bot_" << bot_type << " on this board:\n";
    std::cout << current_state->b.to_ascii_art(current_state->turn);
    std::cout << "Interrupt whenever you like.\n";
    for(int i=1;i<=60;i++){
      board dummy;
      speedrun_bot->set_search_depth(i,i);
      speedrun_bot->do_move(&current_state->b,&dummy);
      long long unsigned speed = speedrun_bot->stats.get_nodes_per_second();
      std::cout << "At depth " << i << ":\t" << big_number(speed) << " nodes/s ";
      std::cout << "\t " << big_number(speedrun_bot->stats.get_nodes()) << " nodes in \t";
      std::cout << speedrun_bot->stats.get_seconds() << " seconds.\n";
    }
    return true;
  }
  if(tournament){
    tournament->run();
    return true;
  }
  
  return false;
}


void game_control::on_human_do_move(int field_id)
{
  if(get_bot_to_move() || !current_state->b.is_valid_move(field_id)){
    return;
  }
 
  ++current_state;
  *current_state = *(current_state-1);
  current_state->b.do_move(field_id);
  on_any_move();
}

void game_control::on_bot_do_move()
{
  if(!current_state->b.has_valid_moves()){
    return;
  }
  
  ++current_state;
  *current_state = *(current_state-1);
  get_bot_to_move()->do_move(&(current_state-1)->b,&current_state->b);
  on_any_move(); 
}

void game_control::on_any_move()
{  
  
  std::cout << current_state->b.to_string() << '\n';
  
  current_state->switch_turn();
  last_redo = current_state;

  const board* b = &current_state->b;
  
  if(!b->has_valid_moves()){
    if(!b->opponent_has_moves()){
      on_game_ended();
    }
    else{
      ++current_state;
      *current_state = *(current_state-1);
      current_state->switch_turn();
      current_state->b.switch_turn();
    }
  }
  mw->update_fields();
  if(bot[BLACK] && bot[WHITE]){
    std::cout << current_state->b.to_ascii_art(current_state->turn);
  }
}



void game_control::on_undo()
{
  board_state_t* undo = current_state;
  do{
    --undo;
    if(undo<board_states){
      mw->update_status_bar("Cannot undo");
      return;
    }
  }while(bot[undo->turn] || !undo->b.has_valid_moves());
  current_state = undo;
  mw->update_fields();
}

void game_control::on_redo()
{ 
  board_state_t* redo = current_state;
  do{
    ++redo;
    if(redo>last_redo){
      mw->update_status_bar("Cannot redo");
      return;
    }
  }while(bot[redo->turn]);
  current_state = redo;
  mw->update_fields();
}

void game_control::on_new_game()
{
  current_state = last_redo = board_states;
  current_state->turn = BLACK;
  
  current_state->b.reset();
  if(use_xot){
    current_state->b.init_xot();
  }
  if(random_moves != 0){
    current_state->b = current_state->b.do_random_moves(random_moves);
  }
  if(board_string != ""){
    current_state->b = board(board_string);
  }
  
  
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
  //current_state->b.show();
  std::cout << current_state->b.to_string() << '\n';
  
  int b_count = current_state->b.opp.count();
  int w_count = current_state->b.me.count();
  
  if(current_state->turn == BLACK){
    std::swap<int>(b_count,w_count);
  }
  
  
  std::string text = "Game has ended. White (" + to_str<int>(w_count) + ") - Black (";
  text += to_str<int>(b_count)+ ")";
  
  std::cout << text << '\n';
  mw->update_status_bar(text);
  
  if(quit_if_game_over){
    std::cout << current_state->b.to_ascii_art(current_state->turn);
    mw->hide();
    exit(0);
  }
  if(loop_game){
    on_new_game();
  }
}

bool game_control::timeout_handler()
{
  if(bot[current_state->turn]){
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
  return bot[current_state->turn];
}

