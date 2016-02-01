#ifndef CHANNEL_SLIDER_H_
#define CHANNEL_SLIDER_H_

#include <gtkmm-3.0/gtkmm.h>

class ChannelSlider : public Gtk::Scale
{
public:
  ChannelSlider();
  virtual ~ChannelSlider();
};

#endif /* CHANNEL_SLIDER_H_ */
