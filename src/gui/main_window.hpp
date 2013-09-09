#ifndef SQUARED_MAIN_WINDOW_HPP
#define SQUARED_MAIN_WINDOW_HPP

#include <cassert>
#include <iostream>
#include <vector>

#include <gtkmm.h>
#include <gtkmm/stock.h>

#include "bots/bot_ali.hpp"

#include "game/game_control.hpp"
#include "game/util.hpp"
#include "game/board.hpp"

#include "gui_base/clickable_image.hpp"
#include "gui_base/scaling_image.hpp"
#include "gui/settings_dialog.hpp"

struct clickable_image;
class board;

class main_window:
  public Gtk::Window
{

public:
  main_window();
  virtual ~main_window();

  void init_ui();
  
  /* update from control.current */
  void update_fields();
  
  /* allow easy updating of the statusbar */
  void update_status_bar(const std::string& text);
  
  
  /* public members */
  Gtk::Table table;
  Gtk::Statusbar status_bar;
  game_control control;
  
protected:  
  
  /* Signal handlers: */
  void on_menu_game_quit();
  void on_menu_settings_fullscreen();
  void on_menu_settings_settings();
  
  /* Member widgets: */
  Gtk::VBox vbox;
  Gtk::AspectFrame aspect_frame;
  std::vector<std::vector<clickable_image*> > fields;
  
  
  /* Other */
  Glib::RefPtr<Gtk::UIManager> ui_manager;
  Glib::RefPtr<Gtk::ActionGroup> action_group;
  std::string ui_file;


};

#endif
