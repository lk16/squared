#include <gtkmm.h>

#include "mainwindow.hpp"

void run_no_gui(){
  board *b;
  bot_base* bots[2];
  std::string instring;
  int inint;

#if 0  
  instring = "";
  inint = -1;
  bots[0] = bots[1] = NULL;
  b = new board();
  b->reset();
  
  
  while(true){
    std::cout << "Is the black player a human? ";
    std::cin >> instring;
    switch((char)instring.substr(0,1)){
      case 'y':
      case 'Y':
        std::cout << '\n';
        break;
      case 'n':
      case 'N':
        std::cout << "Enter searchdepth of black player. "; 
        std::cin >> inint;
        bots[BLACK] = new bot_ali(BLACK,inint,2*inint);
      default:
        continue;
    }
  }
  while(true){
    std::cout << "Is the white player a human? ";
    std::cin >> instring;
    switch((char)instring.substr(0,1)){
      case 'y':
      case 'Y':
        std::cout << '\n';
        break;
      case 'n':
      case 'N':
        std::cout << "Enter searchdepth of white player. "; 
        std::cin >> inint;
        bots[WHITE] = new bot_ali(WHITE,inint,2*inint);
      default:
        continue;
    }
  }
#endif
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
