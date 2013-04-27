#include "mainwindow.hpp"


main_window::main_window():
  table(FIELD_SIZE,FIELD_SIZE),
  control(this),
  aspect_frame("",0.5,0.5,1),
  fields(FIELD_SIZE,std::vector<field*>(FIELD_SIZE))
{

  init_ui();

 
  for(int y=0;y<FIELD_SIZE;y++){ 
    for(int x=0;x<FIELD_SIZE;x++){
      fields[x][y]=new field(this,y*FIELD_SIZE+x,IMAGE_PATH + "empty.png");
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
    std::cerr << "Adding ui from 'menus.xml' failed: " << ex.what() << std::endl;
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
  for(int y=0;y<FIELD_SIZE;y++){
    for(int x=0;x<FIELD_SIZE;x++){
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
  int input_level[2],output_level[2];
  bot_base* bot;
  
  input_level[BLACK] = control.bot[BLACK] ? control.bot[BLACK]->get_max_depth() : -1;
  input_level[WHITE] = control.bot[WHITE] ? control.bot[WHITE]->get_max_depth() : -1;
  
  
  settings_dialog sd(*this,input_level[BLACK],input_level[WHITE]);
  
  std::cout << "Show settings\n";
  if(sd.run() == Gtk::RESPONSE_OK){
    
    sd.collect_data(&output_level[BLACK],&output_level[WHITE]);
    
    if(output_level[BLACK]==-1){
      control.remove_bot(BLACK);
    }
    else{
      control.add_bot(BLACK,output_level[BLACK],2*output_level[BLACK]);
    }
    if(output_level[WHITE]==-1){
      control.remove_bot(WHITE);
    }
    else{
      control.add_bot(WHITE,output_level[WHITE],2*output_level[WHITE]);
    }
  
  }
}

void main_window::update_fields()
{
  const board *b;
  int x,y;
  std::string imagefile;
  board dummy;
  
  b = control.current;
  
  for(y=0;y<FIELD_SIZE;y++){
    for(x=0;x<FIELD_SIZE;x++){
      switch(b->get_color(y*FIELD_SIZE+x)){
        case WHITE:
          imagefile = "white.png";
          break;
        case BLACK:
          imagefile = "black.png";
          break;
        case EMPTY:
          if(b->do_move(y*FIELD_SIZE+x,&dummy)){
            imagefile = "move.png";
          }
          else{
            imagefile = "empty.png";
          }
          break;
        default:
          /* does never happen */
          CRASH;
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
