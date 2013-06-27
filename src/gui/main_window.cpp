#include "main_window.hpp"

main_window::main_window():
  table(FIELD_SIZE,FIELD_SIZE),
  control(this),
  aspect_frame("",0.5,0.5,1),
  fields(FIELD_SIZE,std::vector<clickable_image*>(FIELD_SIZE)),
  ui_file(UI_PATH + "menus.xml")
{
  init_ui();
  
  control.on_new_game();
  show_all_children();
}


void main_window::init_ui(){
    
  this->set_default_size(400,400);
  this->set_title(Glib::ustring("Squared"));
  
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
    std::cerr << "Adding ui from 'menus.xml' failed: " << ex.what();
  }
  
  add(vbox);
  
  Gtk::Widget* menubar = ui_manager->get_widget("/MenuBar");
  if(menubar){
    vbox.pack_start(*menubar,Gtk::PACK_SHRINK);
  }
  
  for(int y=0;y<8;y++){ 
    for(int x=0;x<8;x++){
      fields[x][y]=new clickable_image(this,y*FIELD_SIZE+x,IMAGE_PATH + "empty.png");
      table.attach(*fields[x][y],x,x+1,y,y+1);
    }
  }
  
  vbox.pack_start(aspect_frame,Gtk::PACK_EXPAND_WIDGET);
  aspect_frame.set_shadow_type(Gtk::SHADOW_NONE);
  aspect_frame.add(table);
  
  vbox.pack_start(status_bar,Gtk::PACK_SHRINK);
  
  this->set_resizable(false);
  
  show_all_children();
}

void main_window::on_menu_game_quit()
{
  Gtk::Dialog dialog("Quit?",*this);
  Gtk::Label label("Are you sure you want to quit?",0.5,0.5,false);
  dialog.get_vbox()->add(label);
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
  
  input_level[BLACK] = control.bot[BLACK] ? control.bot[BLACK]->get_search_depth() : -1;
  input_level[WHITE] = control.bot[WHITE] ? control.bot[WHITE]->get_search_depth() : -1;
  
  
  settings_dialog sd(*this,input_level[BLACK],input_level[WHITE]);
  
  if(sd.run() == Gtk::RESPONSE_OK){
    
    sd.collect_data(&output_level[BLACK],&output_level[WHITE]);
    
    if(output_level[BLACK]==-1){
      control.remove_bot(BLACK);
    }
    else{
      int x = output_level[BLACK];
      control.add_bot(BLACK,x,max(2*x+2,16),max(2*x+2,16));
    }
    if(output_level[WHITE]==-1){
      control.remove_bot(WHITE);
    }
    else{
      int x = output_level[WHITE];
      control.add_bot(WHITE,x,max(2*x+2,16),max(2*x+2,16));
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
      if(b->discs[WHITE].test(y*FIELD_SIZE+x)){
        imagefile = "white.png";
      }
      else if(b->discs[BLACK].test(y*FIELD_SIZE+x)){
        imagefile = "black.png";
      }
      else if(&dummy != b->do_move(y*FIELD_SIZE+x,&dummy)){
        imagefile = "move.png";
      }
      else{
        imagefile = "empty.png";
      }
      table.remove(*fields[x][y]);
      delete fields[x][y];
      fields[x][y] = new clickable_image(this,y*FIELD_SIZE+x,IMAGE_PATH + imagefile);
      table.attach(*fields[x][y],x,x+1,y,y+1);
      table.show_all_children();
    }
  }
}

void main_window::update_status_bar(const std::string& text)
{
  status_bar.pop();
  status_bar.push(text);
}
