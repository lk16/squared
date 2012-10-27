#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

#include <cassert>
#include <iostream>
#include <vector>

#include <glibmm/ustring.h>

#include <gtkmm/aspectframe.h>
#include <gtkmm/box.h>
#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/window.h>

#include "gamecontrol.hpp"
#include "shared.hpp"

struct field;
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
  std::vector<std::vector<field*> > fields;
  
  /* Other */
  Glib::RefPtr<Gtk::UIManager> ui_manager;
  Glib::RefPtr<Gtk::ActionGroup> action_group;


};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
