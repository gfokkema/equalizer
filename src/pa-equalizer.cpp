#include "pa-equalizer.h"

#include <iostream>

using namespace Gio;

Glib::RefPtr<DBus::Connection>
connect ()
{
  GDBusConnection* c_conn = nullptr;
  GVariant* c_socket = nullptr;
  GError* c_error = nullptr;

  auto lookup_conn = DBus::Connection::get_sync (DBus::BUS_TYPE_SESSION);
  auto lookup_params = Glib::VariantContainerBase::create_tuple (
    { var_ustring::create ("org.PulseAudio.ServerLookup1"),
      var_ustring::create ("Address") }
  );
  auto lookup_result = lookup_conn->call_sync (
    "/org/pulseaudio/server_lookup1",  // Object path
    "org.freedesktop.DBus.Properties", // Interface name
    "Get",                             // Method name
    lookup_params,
    "org.PulseAudio1"                  // Bus name
  );

  g_variant_get_child (lookup_result.gobj (), 0, "v", &c_socket);
  auto socket = Glib::Variant<Glib::ustring> (c_socket).get ();

  if (socket.size () > 0)
  {
    std::cout << socket << std::endl;
    c_conn = g_dbus_connection_new_for_address_sync (
      socket.c_str (),                 // Socket string
      G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT,
      NULL,                            // GDBusAuthObserver
      NULL,                            // GCancellable
      &c_error
    );
  }

  if (c_error) throw PAException (c_error->message);
  if (!c_conn) throw PAException ("Failed connecting to pulseaudio socket.");

  return Glib::wrap (c_conn);
}


void
print_type (GVariant* arg)
{
  std::cout << g_variant_get_type_string (arg) << std::endl;
}

PAEqualizer::PAEqualizer ()
{
  GDBusProxy* c_proxy = nullptr;
  GError* c_error = nullptr;

  init ();

  this->conn = connect ();

  c_proxy = g_dbus_proxy_new_sync (
    this->conn->gobj (),                // DBus connection
    G_DBUS_PROXY_FLAGS_NONE,            // Proxy flags
    NULL,                               // GDBusInterfaceInfo
    NULL,                               // Bus name
    "/org/pulseaudio/equalizing1",      // Object path
    "org.freedesktop.DBus.Properties",  // Interface name
    NULL,                               // GCancellable
    &c_error
  );

  if (c_error) throw PAException (c_error->message);
  if (!c_proxy) throw PAException ("Failed to create a proxy for pulseaudio.");

  this->proxy = Glib::wrap (c_proxy);
}

PAEqualizer::~PAEqualizer ()
{
  if (this->conn && !this-conn->is_closed ())
    this->conn->close ();
}
