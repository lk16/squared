#include <gtkmm.h>

#include <vector>
#include <set>


#include "gui/main_window.hpp"

void test_area(){
  
  board b;
  b.reset();
  
  for(int i=1;i<13;i++){
    std::set<board> desc = b.get_descendants(i);
    
    
    std::cout << "At depth " << (i-1) << ": ";
    std::cout << desc.size() << " descendants \n";

  }
  
  
}





int main(int argc,char **argv){
  Gtk::Main kit(argc,argv);
  main_window window;
  
  // has to be done
  board::init_constants();
  
  
  if(argc>=2){
    std::string argv1(argv[1]);
    if(argv1=="testing"){
      test_area();
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
