#include <gtkmm.h>

#include <map>

#include "gui/main_window.hpp"
#include "main/testing.hpp"
#include "main/args.hpp"


int main(int argc,char **argv){
  srand(std::time(NULL));
  
  squared_args parsed_args(argc,(const char**)argv);

  if(!parsed_args.parse()){
    return 0;
  }
  

  if(parsed_args.show_flag){
    int turn = parsed_args.gc.board_state.turn;
    std::cout << parsed_args.gc.board_state.b.to_ascii_art(turn);
  }
  
  if(parsed_args.start_windowed_game){
    Gtk::Main kit(argc,argv);
     
    main_window window;
    parsed_args.gc.mw = &window;
    window.control = &parsed_args.gc;
    window.update_fields();
    parsed_args.gc.connect_timeout_signal();
    Gtk::Main::run(window);
  }
  
  return 0;
}
