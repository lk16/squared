#ifndef SCALING_IMAGE_HPP
#define SCALING_IMAGE_HPP

#include "game/util.hpp"

#include <gtkmm/image.h>
#include <gtkmm/aspectframe.h>

/*
This is a modified version of code written by Sohail Soman in 2008
copied from this webpage: 
http://uint32t.blogspot.nl/2008/06/automatically-rescale-gtkimage-and.html

*/

/// This Widget only scales the image but does not preserve
/// aspect ratios.
class scaling_image:
  public Gtk::Image
{
  
  Glib::RefPtr<Gdk::Pixbuf> m_original;
  Gdk::InterpType m_interp;
  bool m_sized;
    
  virtual void on_size_allocate(Gtk::Allocation & r)
  {
      // This event is fired on all Widgets when their rectangle gets
      // resized. So we rescale our image. But rescaling our image fires
      // another resize event, and another, and another... You see the
      // problem!
      //
      // The most straightforward solution is to not rescale the image
      // the second time the event is fired. Hence the m_size toggle.
      //
      // Ideally I'd not like to do this but this was the best way I
      // found!
      if(!m_sized)
      {
        Glib::RefPtr<Gdk::Pixbuf> scaled = 
        m_original->scale_simple(r.get_width(),r.get_height(),m_interp);
        // Reset the image to be the new scaled image which
        // will cause the second size-allocate signal
        Gtk::Image::set(scaled);

        Gtk::Image::on_size_allocate(r);
        
        // Make sure we don't treat it as a rescale request 
        // next time
        m_sized = true;
      }
      else
      {
        // Reaction to set above. Call the base class's on_size_allocate
        // function. Shouldn't I have to call this above too? Maybe it
        // isn't necessary since it gets called anyway.
        Gtk::Image::on_size_allocate(r);
        // Treat next resize event as a rescale
        m_sized = false;
      }
    }
  
public:
  explicit scaling_image(Glib::RefPtr<Gdk::Pixbuf> pixbuf,
  Gdk::InterpType interp = Gdk::INTERP_NEAREST):
    Gtk::Image(pixbuf),
    m_original(pixbuf),
    m_interp(interp),
    m_sized(false)
  {}

public:
  scaling_image(const std::string& file,Gdk::InterpType interp):
    Gtk::Image(file),
    m_original(get_pixbuf()),
    m_interp(interp),
    m_sized(false)
  {
  }

};

/// This widget automatically scales the image but preserves the
/// aspect ratio of the original image. This is accomplished via use
/// of the AspectFrame class
///
/// Ideally would like this to be a Gtk::Image subclass.
class aspect_scaling_image:
  public Gtk::AspectFrame
{
  scaling_image m_img;  
  
public:
  
  explicit aspect_scaling_image(const std::string& filename,
                       Gdk::InterpType interp = Gdk::INTERP_BILINEAR):
                       m_img(filename,interp)
  {
    
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = m_img.get_pixbuf();
    
    Gtk::AspectFrame::set(0.5,0.5,
    // Aspect ratio of frame
    pixbuf->get_width()/float(pixbuf->get_height()));
    // This makes it appear as if there is no frame
    set_shadow_type(Gtk::SHADOW_NONE);
    // This allows a minimum size
    set_size_request(pixbuf->get_width()/4,pixbuf->get_height()/4);
    
    
    // Finally, add the image to be managed
    add(m_img);
  }
  
  /* BUG this doesnt work properly */
  void set(const std::string& filename){
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(filename);
    m_img.set(pixbuf);
  }
  
  
};

#endif