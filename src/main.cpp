#include <gtkmm.h>

#include "mainwindow.hpp"

int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  main_window window;
  
  if(argc!=1){
    std::cout << "Run without params!\n";
    return 0;
  }
  
  Gtk::Main::run(window);
  
  return 0;
}