#ifndef SQUARED_FIELD_HPP
#define SQUARED_FIELD_HPP

#include <boost/bind.hpp>

#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>

#include "mainwindow.hpp"
#include "shared.hpp"

class main_window;

struct field:
  public Gtk::EventBox
{
  Gtk::Image image;
  int x,y;
  main_window* parent;
  

  field(main_window* par,int x,int y,const std::string& imagefile);
  
  void update_image(const char* imagefile);
  
  virtual bool on_button_press_event(GdkEventButton* event);
  
};
#endif