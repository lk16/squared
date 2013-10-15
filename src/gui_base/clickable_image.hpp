#ifndef SQUARED_FIELD_HPP
#define SQUARED_FIELD_HPP

#include <boost/bind.hpp>

#include <gtkmm/eventbox.h>
#include <gtkmm/image.h>

#include "game/util.hpp"

class main_window;

class clickable_image:
  public Gtk::EventBox
{
  Gtk::Image image;
  int field_id;
  main_window* parent;
  
public:
  
  /** 
  * @brief default ctor, doesnt do anything
  * WARNING: doesnt initialize anything
  */
  clickable_image();
  
  /**
  * @brief init values
  * @p init pointer to main_window
  * @p field_id id of the field this class is representing
  * @p imagefile filename of image to show
  */
  void init(main_window* par,int field_id,const std::string& imagefile);
  
  /**
  * @brief button press event
  * @p event event data
  * @return true, has to be called again whenever a click event occurs
  */
  virtual bool on_button_press_event(GdkEventButton* event);
  
  /**
  * @brief show different image 
  * @p filename filename of new image to be shown
  */
  void set_image(const std::string& filename);
  
};
#endif