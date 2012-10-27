#include "mainwindow.hpp"
#include "shared.hpp"
#include "field.hpp"
#include "gamecontrol.hpp"
#include "board.hpp"

main_window::main_window():
  table(8,8),
  control(this),
  aspect_frame("",0.5,0.5,1),
  fields(8,std::vector<field*>(8))
{

  init_ui();

 
  for(int y=0;y<8;y++){ 
    for(int x=0;x<8;x++){
      fields[x][y]=new field(this,x,y,IMAGE_PATH + "empty.png");
      table.attach(*fields[x][y],x,x+1,y,y+1);
    }
  }
  
  control.on_new_game();
  
  vbox.pack_start(aspect_frame,Gtk::PACK_SHRINK);
  aspect_frame.add(table);

  vbox.pack_start(status_bar,Gtk::PACK_SHRINK);
  
  show_all_children();
}


void main_window::init_ui(){
  
  set_default_size(400,400);
  
  
  set_title(Glib::ustring("Squared"));
  
  
  
  action_group = Gtk::ActionGroup::create();
  
  
  
  
  /* menus themselves */
  action_group->add(Gtk::Action::create("GameMenu","_Game"));
  action_group->add(Gtk::Action::create("SettingsMenu","_Settings"));

  /* Game menu */
  action_group->add(
    Gtk::Action::create("GameNew",Gtk::Stock::NEW,"_New","Start a new game"),
    sigc::mem_fun(control,&game_control::on_new_game)
  );
  action_group->add(
    Gtk::Action::create("GameUndo",Gtk::Stock::UNDO,"_Undo","Undo move"),
    sigc::mem_fun(control,&game_control::on_undo)
  );
  action_group->add(
    Gtk::Action::create("GameRedo",Gtk::Stock::REDO,"_Redo","Redo move"),
    sigc::mem_fun(control,&game_control::on_redo)
  );
  action_group->add(
    Gtk::Action::create("GameQuit",Gtk::Stock::QUIT,"_Quit","Quit game"),
    sigc::mem_fun(*this,&main_window::on_menu_game_quit)
  );
  
  /* Settings menu */
  action_group->add(
    Gtk::Action::create("SettingsFullscreen","_Full Screen"),
    sigc::mem_fun(*this,&main_window::on_menu_settings_fullscreen)
  );
  action_group->add(
    Gtk::Action::create("SettingsSettings","_Settings"),
    sigc::mem_fun(*this,&main_window::on_menu_settings_settings)
  );
  
  
  
  
  
  ui_manager = Gtk::UIManager::create();
  ui_manager->insert_action_group(action_group);
  
  
  try{
    ui_manager->add_ui_from_file(UI_PATH + "menus.xml");
  }
  catch(const Glib::Error& ex){
    std::cerr << "Adding ui from 'main_window.xml' failed: " << ex.what();
  }
  
  add(vbox);
  
  Gtk::Widget* menubar = ui_manager->get_widget("/MenuBar");
  if(menubar){
    vbox.pack_start(*menubar,Gtk::PACK_SHRINK);
  }
}

void main_window::on_menu_game_quit()
{
  Gtk::Dialog dialog("Quit?",*this);
  Gtk::Label label("Are you sure you want to quit?",0.5,0.5,false);
  dialog.get_vbox()->pack_start(label,Gtk::PACK_SHRINK,0);
  dialog.show_all_children();
  dialog.add_button(Gtk::Stock::NO,Gtk::RESPONSE_NO);
  dialog.add_button(Gtk::Stock::YES,Gtk::RESPONSE_YES);
  if(dialog.run() == Gtk::RESPONSE_YES){
    hide();
  }
}

main_window::~main_window()
{
  for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
      delete fields[x][y];
    }
  }
}

void main_window::on_menu_settings_fullscreen()
{
  std::cout << "Toggle full screen\n";
}

void main_window::on_menu_settings_settings()
{
  std::cout << "Show settings menu\n";
}

void main_window::update_fields()
{
  const board *b;
  int x,y;
  std::string imagefile;
  
  b = control.current;
  
  for(y=0;y<8;y++){
    for(x=0;x<8;x++){
      switch(b->get_color(x,y)){
        case WHITE:
          imagefile = "white.png";
          break;
        case BLACK:
          imagefile = "black.png";
          break;
        case EMPTY:
          if(b->is_valid_move(x,y,b->turn)){
            imagefile = "move.png";
          }
          else{
            imagefile = "empty.png";
          }
          break;
        default:
          /* does never happen */
          break;
      }
      fields[x][y]->image.set(IMAGE_PATH + imagefile);
    }
  }
}

void main_window::update_status_bar(const std::string& text)
{
  status_bar.pop();
  status_bar.push(text);
}
