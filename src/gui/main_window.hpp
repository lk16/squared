#pragma once

#include <cassert>
#include <iostream>

#include <gtkmm.h>
#include <gtkmm/stock.h>

#include "bots/bot_ali.hpp"
#include "game/game_control.hpp"
#include "game/board.hpp"
#include "gui/clickable_image.hpp"
#include "gui/preferences_dialog.hpp"
#include "util/const.hpp"


class board;

class main_window:
  public Gtk::Window
{

public:
  
  // shows board in start position
  main_window();
  
  // shows board b
  main_window(const game_control& gc);

  
  /* update from control.current */
  void update_fields();
  
  /* allow easy updating of the statusbar */
  void update_status_bar(const std::string& text);
  
  /* public members */
  game_control control;
  
protected:  
  
  void init_ui();
  
  /* Signal handlers: */
  void on_new_game();
  void on_undo();
  void on_redo();
  void on_menu_game_quit();
  void on_menu_settings_preferences();
  
  /* Member widgets: */
  Gtk::Table table;
  Gtk::Statusbar status_bar;
  Gtk::VBox vbox;
  Gtk::AspectFrame aspect_frame;
  clickable_image fields[8][8];
  
  
  /* Other */
  Glib::RefPtr<Gtk::UIManager> ui_manager;
  Glib::RefPtr<Gtk::ActionGroup> action_group;
  std::string ui_file;


};
