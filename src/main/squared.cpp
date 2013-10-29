#include <gtkmm.h>

#include "gui/main_window.hpp"
#include "misc/testing.hpp"
#include "misc/timing.hpp"

int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1=="testing"){
      testing_area();
    }
    else if(argv1=="timing"){
      timing_area();
    }
    else{
      std::cout << "Invalid argument: " << argv1 << '\n' << 
      "Try one of these:\n"
      "----------.-----------------------\n"
      "testing   | run the testing_area()\n"
      "timing    | run the timing_area()\n"
      "<no args> | run the windowed game\n";
      
    }
  }
  else{
    main_window window;
    Gtk::Main::run(window);
  }
  return 0;
}
