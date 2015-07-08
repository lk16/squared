#include <gtkmm.h>

#include "gui/main_window.hpp"
#include "main/testing.hpp"
#include "main/args.hpp"


int main(int argc,char **argv){
  srand(std::time(NULL));
  
  squared_args parsed_args(argc,(const char**)argv);

  
  game_control control;
  
  parsed_args.gc = &control;
  
  if(!parsed_args.parse()){
    return 1;
  }
  

  if(control.show_board_flag){
    const game_control::board_state_t* state = &control.board_state;
    std::cout << state->b.to_ascii_art(state->turn);
  }
  
  if(control.pgn_task){
    const pgn_task_t* task = control.pgn_task;
    bot_base* bot = bot_registration::bots()[control.bot_type]();
    bot->set_search_depth(task->search_depth,task->perfect_depth);
    bot->disable_shell_output();
    pgn pgn_file(task->filename);
    std::cout << pgn_file.analyse(bot,true,true);
    delete bot;
    return 0;
  }
  
  if(control.run_speed_test){
    bot_base* bot = bot_registration::bots()[control.bot_type]();
    bot->disable_shell_output();
    bot->disable_book();
    std::cout << "testing speed of bot_" << control.bot_type << " on this board:\n";
    std::cout << control.board_state.b.to_ascii_art(control.board_state.turn);
    for(int i=1;i<=60;i++){
      board b,dummy;
      b = control.board_state.b;
      bot->set_search_depth(i,i);
      bot->do_move(&b,&dummy);
      long long speed = bot->stats.get_nodes_per_second();
      std::cout << "At depth " << i << ":\t" << big_number(speed) << " nodes/s ";
      std::cout << "\ttook " << bot->stats.get_seconds() << " seconds.\n";
      if(bot->stats.get_seconds() > 10.0){
        break;
      }
    }
  }
  
  if(control.learn_threads != 0){
    book_t book(BOOK_PATH + control.bot_type + "_book.csv");
    book.learn(control.bot_type,control.learn_threads);
  }
  
  if(control.compress_book){
    book_t(BOOK_PATH + control.bot_type + "_book.csv").clean();
  }
  
  
  if(control.start_windowed_game){
    Gtk::Main kit(argc,argv);
    main_window window;
    control.mw = &window;
    window.control = &control;
    window.update_fields();
    control.connect_timeout_signal();
    Gtk::Main::run(window);
  }
  
  return 0;
}
