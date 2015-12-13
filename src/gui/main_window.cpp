#include "main_window.hpp"

main_window::main_window():
  control(),
  table(8,8),
  aspect_frame("",0.5,0.5,1),
  ui_file(UI_PATH + "menus.xml")
{
  init_ui();
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
    sigc::mem_fun(*this,&main_window::on_new_game)
  );
  action_group->add(
    Gtk::Action::create("GameUndo",Gtk::Stock::UNDO,"_Undo","Undo move"),
    sigc::mem_fun(*this,&main_window::on_undo)
  );
  action_group->add(
    Gtk::Action::create("GameRedo",Gtk::Stock::REDO,"_Redo","Redo move"),
    sigc::mem_fun(*this,&main_window::on_redo)
  );
  action_group->add(
    Gtk::Action::create("GameQuit",Gtk::Stock::QUIT,"_Quit","Quit game"),
    sigc::mem_fun(*this,&main_window::on_menu_game_quit)
  );
  
  action_group->add(
  Gtk::Action::create("SettingsPreferences","_Preferences"),
    sigc::mem_fun(*this,&main_window::on_menu_settings_preferences)
  );
  
  
  ui_manager = Gtk::UIManager::create();
  ui_manager->insert_action_group(action_group);
  
  
  try{
    ui_manager->add_ui_from_file(UI_PATH + "menus.xml");
  }
  catch(const Glib::Error& ex){
    std::cerr << "Adding ui from 'menus.xml' failed: " << ex.what() << '\n';
    std::exit(1);
  }
  
  this->add(vbox);
  
  Gtk::Widget* menubar = ui_manager->get_widget("/MenuBar");
  if(menubar){
    vbox.pack_start(*menubar,Gtk::PACK_SHRINK);
  }
  
  for(size_t y=0;y<8;y++){ 
    for(size_t x=0;x<8;x++){
      fields[x][y].init(this,y*8+x,IMAGE_PATH + "empty.png");
      table.attach(fields[x][y],x,x+1,y,y+1);
    }
  }
  
  
  vbox.pack_start(aspect_frame,Gtk::PACK_EXPAND_WIDGET);
  aspect_frame.set_shadow_type(Gtk::SHADOW_NONE);
  aspect_frame.add(table);
  
  vbox.pack_start(status_bar,Gtk::PACK_SHRINK);
  
  this->set_resizable(false);
}

void main_window::on_menu_game_quit()
{
  hide();
}

void main_window::on_menu_settings_preferences()
{
  int input_level[2],output_level[2];
  
  input_level[BLACK] = control->bot[BLACK] ? control->bot[BLACK]->get_search_depth() : -1;
  input_level[WHITE] = control->bot[WHITE] ? control->bot[WHITE]->get_search_depth() : -1;
  
  
  preferences_dialog sd(*this,input_level[BLACK],input_level[WHITE]);
  
  if(sd.run() == Gtk::RESPONSE_OK){
    
    sd.collect_data(&output_level[BLACK],&output_level[WHITE]);
    
    if(output_level[BLACK]==-1){
      control->remove_bot(BLACK);
    }
    else{
      int x = output_level[BLACK];
      control->search_depth = x;
      control->perfect_depth = max(2*x,16);
      control->add_bot(BLACK);
    }
    if(output_level[WHITE]==-1){
      control->remove_bot(WHITE);
    }
    else{
      int x = output_level[WHITE];
      control->search_depth = x;
      control->perfect_depth = max(2*x,16);
      control->add_bot(WHITE);
    }
  
  }
}




void main_window::update_fields(
  const game_control::board_state_t* before_state,
  const game_control::board_state_t* after_state
){
  bits64 before_white = before_state->b.me;
  bits64 before_black = before_state->b.opp;
  bits64 after_white = after_state->b.me;
  bits64 after_black = after_state->b.opp;
  
  if(before_state->turn == BLACK){
    std::swap(before_white,before_black);
  }
  if(after_state->turn == BLACK){
    std::swap(after_white,after_black);
  }

  std::string before_str[64];
  for(int i=0;i<64;i++){
    if(before_white.test(i)){
      before_str[i] = "white";
    }
    else if(before_black.test(i)){
      before_str[i] = "black";
    }
    else if(before_state->b.is_valid_move(i)){
      if(before_state->turn == WHITE){
        before_str[i] = "move_white";
      }
      else{
        before_str[i] = "move_black";
      }
    }
    else{
      before_str[i] = "empty";
    }
  }
  
  std::string after_str;
  for(int i=0;i<64;i++){
    if(after_white.test(i)){
      after_str = "white";
    }
    else if(after_black.test(i)){
      after_str = "black";
    }
    else if(after_state->b.is_valid_move(i)){
      if(after_state->turn == WHITE){
        after_str = "move_white";
      }
      else{
        after_str = "move_black";
      }
    }
    else{
      after_str = "empty";
    }
    fields[i%8][i/8].set_image(IMAGE_PATH + before_str[i] + "_to_" + after_str + ".gif");
  }
}

void main_window::update_status_bar(const std::string& text)
{
  status_bar.push(text);
}

void main_window::on_new_game()
{
  control->on_new_game();
}

void main_window::on_redo()
{
  control->on_redo();
}

void main_window::on_undo()
{
  control->on_undo();
}

