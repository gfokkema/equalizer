#include "pa-equalizer.h"

#include <iostream>

using namespace Gio;

void
print_type (GVariant* arg)
{
  std::cout << g_variant_get_type_string (arg) << std::endl;
}

PAEqualizer::PAEqualizer ()
{
  GVariant* c_socket;

  init ();

  auto lookup_conn = DBus::Connection::get_sync (DBus::BUS_TYPE_SESSION);
  auto lookup_params = Glib::VariantContainerBase::create_tuple (
    { var_ustring::create ("org.PulseAudio.ServerLookup1"),
      var_ustring::create ("Address") }
  );
  auto lookup_result = lookup_conn->call_sync (
    "/org/pulseaudio/server_lookup1",
    "org.freedesktop.DBus.Properties",
    "Get",
    lookup_params,
    "org.PulseAudio1"
  );

  g_variant_get_child (lookup_result.gobj (), 0, "v", &c_socket);
  auto str = Glib::Variant<Glib::ustring> (c_socket).get ();

  if (str.size () > 0)
  {
    std::cout << str << std::endl;
    DBus::Connection::create_for_address (
      str,
      sigc::mem_fun (*this, &PAEqualizer::on_pulseaudio_dbus),
      DBus::CONNECTION_FLAGS_AUTHENTICATION_CLIENT
    );
  }
}

PAEqualizer::~PAEqualizer ()
{
  if (this->conn && !this-conn->is_closed ())
    this->conn->close ();
}

void
PAEqualizer::on_pulseaudio_dbus (Glib::RefPtr<Gio::AsyncResult>& result)
{
  this->conn = DBus::Connection::create_finish (result);
  auto parameters = Glib::VariantContainerBase::create_tuple (
    { var_ustring::create ("org.PulseAudio.Ext.Equalizing1.Manager"),
      var_ustring::create ("EqualizedSinks") }
  );
  auto sinks_result = this->conn->call_sync (
    "/org/pulseaudio/equalizing1",
    "org.freedesktop.DBus.Properties",
    "Get",
    parameters,
    "org.PulseAudio"
  );

  GVariant* c_sinks;
  Glib::VariantBase cxx_sinks;

  // Containing tuple -> out: '(v)'
  std::cout << g_variant_get_type_string (sinks_result.gobj ()) << std::endl;

  // Glib get_child () -> out: 'ao'
  g_variant_get_child (sinks_result.gobj (), 0, "v", &c_sinks);
  std::cout << g_variant_get_type_string (c_sinks) << std::endl;

  // Glibmm get_child () -> out: 'v'
  cxx_sinks = sinks_result.get_child ();
  std::cout << g_variant_get_type_string (cxx_sinks.gobj ()) << std::endl;
}
