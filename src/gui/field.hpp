#ifndef SQUARED_FIELD_HPP
#define SQUARED_FIELD_HPP

#include <boost/bind.hpp>

#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>

#include "mainwindow.hpp"
#include "mainwindow.hpp"
#include "game/util.hpp"

class main_window;

struct field:
  public Gtk::EventBox
{
  Gtk::Image image;
  int field_id;
  main_window* parent;
  

  field(main_window* par,int field_id,const std::string& imagefile);
  
  virtual bool on_button_press_event(GdkEventButton* event);
  
};
#endif