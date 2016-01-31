#include "channelslider.h"

ChannelSlider::ChannelSlider ()
: Gtk::Scale (Gtk::Adjustment::create (0, 0, 100),
              Gtk::ORIENTATION_VERTICAL)
{
  this->set_digits (0);
  this->set_inverted (true);
}

ChannelSlider::~ChannelSlider ()
{
  
}
