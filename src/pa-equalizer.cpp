#include "pa-equalizer.h"

#include <iostream>

using namespace Gio;

PAEqualizer::PAEqualizer ()
{
  init();
  this->conn = DBus::Connection::get_sync (DBus::BUS_TYPE_SESSION);
  DBus::Proxy::create (
    this->conn, "org.PulseAudio1",
    "/org/pulseaudio/server_lookup1",
    "org.freedesktop.DBus.Properties",
    sigc::mem_fun (*this, &PAEqualizer::on_proxy)
  );
}

PAEqualizer::~PAEqualizer ()
{
  
}

void
PAEqualizer::on_proxy (Glib::RefPtr<Gio::AsyncResult>& result)
{
  this->proxy = Gio::DBus::Proxy::create_finish(result);
  if(!proxy)
  {
    std::cerr << "Failed to create a proxy to the session bus." << std::endl;
    return;
  }

  auto parameters = Glib::VariantContainerBase::create_tuple(
    { Glib::Variant<Glib::ustring>::create("org.PulseAudio.ServerLookup1"),
      Glib::Variant<Glib::ustring>::create("Address") }
  );

  const auto result_sockets = this->proxy->call_sync (
    "Get", parameters
  );

  Glib::Variant<std::vector<Glib::ustring>> sockets;
  result_sockets.get_child (sockets);

  for (auto str : sockets.get ())
  {
    std::cout << str << std::endl;
    DBus::Connection::create_for_address_sync (str);
  }
}
