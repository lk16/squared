#include <gtkmm.h>

#include "mainwindow.hpp"

int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  
  main_window window;
    
  Gtk::Main::run(window);
  
  return 0;
}