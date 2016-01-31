#ifndef PA_EQUALIZER_H_
#define PA_EQUALIZER_H_

#include <giomm.h>
#include <glibmm.h>
#include <memory>
#include <vector>
#include "pa-channel.h"

class PAEqualizer
{
public:
  PAEqualizer ();
  virtual ~PAEqualizer ();
  void on_proxy (Glib::RefPtr<Gio::AsyncResult>& result);
private:
  Glib::RefPtr<Gio::DBus::Connection> conn;
  Glib::RefPtr<Gio::DBus::Proxy> proxy;
  std::vector<PAChannel> channels;
};

#endif /* PA_EQUALIZER_H_ */
