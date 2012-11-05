#include <gtkmm.h>

#include "mainwindow.hpp"

void run_no_gui(){

}



int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  main_window window;
  
  if(argc>=2 && std::string(argv[1])=="no-gui"){
    run_no_gui();
  }
  else{
    Gtk::Main::run(window);
  }
  return 0;
}