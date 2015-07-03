#include <gtkmm.h>

#include <map>

#include "gui/main_window.hpp"
#include "main/testing.hpp"
#include "main/args.hpp"


int main(int argc,char **argv){
  srand(std::time(NULL));
  
  arg_parser<squared_arg_t> parser(argc,(const char**)argv);
  
  parser.data.parser = &parser;
  parser.data.init_map();
  
  if(!parser.parse()){
    return 0;
  }
  
  squared_arg_t arg_data = parser.data;

  if(arg_data.show_flag){
    int turn = arg_data.gc.board_state.turn;
    std::cout << arg_data.gc.board_state.b.to_ascii_art(turn);
  }
  
  if(arg_data.start_windowed_game){
    Gtk::Main kit(argc,argv);
    main_window window;
    arg_data.gc.mw = &window;
    window.control = &arg_data.gc;
    window.update_fields();
    arg_data.gc.connect_timeout_signal();
    Gtk::Main::run(window);
  }
  
  return 0;
}
