#include <gtkmm.h>

#include "gui/main_window.hpp"

void run_no_gui(){
#if 0 
  board *b;
  bot_base* bots[2];
  std::string instring;
  int inint;

 
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

void run_debug(){
  board b;
  std::cout << b.get_stable_disc_count_diff(3) << std::endl;
}





int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  main_window window;
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1=="no-gui"){
      run_no_gui();
    }
    else if(argv1=="debug"){
      run_debug();
    }
    else{
      std::cout << "Invalid parameter: " << argv1 << '\n';
    }
  }
  else{
    Gtk::Main::run(window);
  }
  return 0;
}
