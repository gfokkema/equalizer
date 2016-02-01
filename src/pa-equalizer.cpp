#include "pa-equalizer.h"

#include <iostream>

using namespace Gio;

PAEqualizer::PAEqualizer ()
{
  init();

  auto lookup_conn = DBus::Connection::get_sync (DBus::BUS_TYPE_SESSION);
  auto lookup_proxy = DBus::Proxy::create_sync (
    lookup_conn,
    "org.PulseAudio1",
    "/org/pulseaudio/server_lookup1",
    "org.freedesktop.DBus.Properties"
  );
  if (!lookup_proxy)
  {
    std::cerr << "Failed to create a proxy to the session bus." << std::endl;
    return;
  }
  std::cout << "Created a proxy to the session bus." << std::endl;

  auto lookup_params = Glib::VariantContainerBase::create_tuple (
    { Glib::Variant<Glib::ustring>::create ("org.PulseAudio.ServerLookup1"),
      Glib::Variant<Glib::ustring>::create ("Address") }
  );
  const auto lookup_result = lookup_proxy->call_sync (
    "Get", lookup_params
  );

  Glib::Variant<std::vector<Glib::ustring>> sockets;
  lookup_result.get_child (sockets);
  for (auto str : sockets.get ())
  {
    std::cout << str << std::endl;
    this->conn = DBus::Connection::create_for_address_sync (str, DBus::CONNECTION_FLAGS_AUTHENTICATION_CLIENT);
  }

  if (!this->conn)
  {
    std::cerr << "Failed to connect to the pulse bus." << std::endl;
    return;
  }
  std::cout << "Connected to the pulse bus." << std::endl;

  auto parameters = Glib::VariantContainerBase::create_tuple (
    { Glib::Variant<Glib::ustring>::create ("org.PulseAudio.Ext.Equalizing1.Manager"),
      Glib::Variant<Glib::ustring>::create ("EqualizedSinks") }
  );
  auto sinks_result = this->conn->call_sync (
    "/org/pulseaudio/equalizing1",
    "org.freedesktop.DBus.Properties",
    "Get",
    parameters,
    "org.PulseAudio"
  );

  GVariant* c_sinks;
  g_variant_get_child (sinks_result.gobj (), 0, "v", &c_sinks);
  Glib::Variant<std::vector<Glib::ustring>> sinks (c_sinks);
  for (auto sink : sinks.get ())
  {
    std::cout << sink << std::endl;
  }
}
 
PAEqualizer::~PAEqualizer ()
{
  
}

/*
  DBus::Proxy::create (
    this->conn, NULL,
    "/org/pulseaudio/core1",
    "org.freedesktop.DBus.Properties",
    sigc::mem_fun (*this, &PAEqualizer::on_pulse_proxy)
  );
}

void
PAEqualizer::on_pulse_proxy (Glib::RefPtr<Gio::AsyncResult>& result)
{
  this->proxy = DBus::Proxy::create_finish (result);
  if (!this->proxy)
  {
    std::cerr << "Failed to create a proxy to the pulse bus." << std::endl;
    return;
  }
  std::cout << "Connected to the pulse bus." << std::endl;
}
*/
