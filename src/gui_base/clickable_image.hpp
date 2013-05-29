#ifndef SQUARED_FIELD_HPP
#define SQUARED_FIELD_HPP

#include <boost/bind.hpp>

#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>

#include "gui/main_window.hpp"
#include "gui_base/scaling_image.hpp"
#include "game/util.hpp"

class main_window;

struct clickable_image:
  public Gtk::EventBox
{
  //aspect_scaling_image image; // << did not work properly
  Gtk::Image image;
  int field_id;
  main_window* parent;
  

  clickable_image(main_window* par,int field_id,const std::string& imagefile);
  
  virtual bool on_button_press_event(GdkEventButton* event);
  
  void set(const std::string& filename);
  
};
#endif