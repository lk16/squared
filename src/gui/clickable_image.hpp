#pragma once

#include <boost/bind.hpp>

#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>


class main_window;

class clickable_image:
  public Gtk::EventBox
{
  Gtk::Image image;
  int field_id;
  main_window* parent;
  
public:
  
  clickable_image() = default;
  
  // initialise values
  void init(main_window* par,int field_id,const std::string& imagefile);
  
  // handles button press event, always returns true
  virtual bool on_button_press_event(GdkEventButton* event);
  
  // change the shown image to specified one
  void set_image(const std::string& filename);
  
};