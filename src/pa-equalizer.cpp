#include "pa-equalizer.h"

#include <iostream>
#include "pa-exception.h"

using namespace Gio;
using namespace Glib;

//class PAProxy : public Gio::DBus::Proxy
//{
//  virtual void on_signal (const Glib::ustring& sender_name,
//                          const Glib::ustring& signal_name,
//                          const Glib::VariantContainerBase& parameters)
//  {
//    std::cout << signal_name << std::endl;
//  };
//};

void print_type (GVariantType* arg)
{
  std::cout << g_variant_type_dup_string (arg) << std::endl;
}

void
print_type (GVariant* arg)
{
  std::cout << g_variant_get_type_string (arg) << std::endl;
}

/**
 * Connect to the PulseAudio bus.
 *
 * First connects to the session bus to look up the socket path.
 * Then connects and returns an open DBus connection to PulseAudio.
 */
RefPtr<DBus::Connection>
connect ()
{
  GDBusConnection* c_conn = nullptr;
  GError* c_error = nullptr;

  auto lookup_conn = DBus::Connection::get_sync (DBus::BUS_TYPE_SESSION);
  auto lookup_params = VariantContainerBase::create_tuple (
    { var_ustring::create (DBUS_LOOKUP_IFACE),
      var_ustring::create ("Address") }
  );
  auto lookup_result = lookup_conn->call_sync (
    DBUS_LOOKUP_OBJPATH,               // Object path
    DBUS_PROPERTIES_IFACE,             // Interface name
    "Get",                             // Method name
    lookup_params,
    "org.PulseAudio1"                  // Bus name
  );
  auto socket = VariantBase::cast_dynamic<var_var_ustring> (
    lookup_result.get_child ()
  ).get ().get ();

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

  return wrap (c_conn);
}

/**
 * Create a new PAEqualizer instance.
 *
 * Initializes a connection and a proxy to DBUS_EQUALIZER_OBJPATH.
 */
PAEqualizer::PAEqualizer ()
{
  GDBusProxy* c_proxy = nullptr;
  GError* c_error = nullptr;

  Gio::init ();

  this->conn = connect ();

  c_proxy = g_dbus_proxy_new_sync (
    this->conn->gobj (),                // DBus connection
    G_DBUS_PROXY_FLAGS_NONE,            // Proxy flags
    NULL,                               // GDBusInterfaceInfo
    NULL,                               // Bus name
    DBUS_EQUALIZER_OBJPATH,             // Object path
    DBUS_PROPERTIES_IFACE,              // Interface name
    NULL,                               // GCancellable
    &c_error
  );

  if (c_error) throw PAException (c_error->message);
  if (!c_proxy) throw PAException ("Failed to create a proxy for pulseaudio.");

  this->proxy = wrap (c_proxy);
}

PAEqualizer::~PAEqualizer ()
{
  if (this->conn && !this-conn->is_closed ())
    this->conn->close ();
}

/**
 * Return a list of equalized sinks.
 */
std::vector<ustring>
PAEqualizer::get_sinks ()
{
  auto params = VariantContainerBase::create_tuple (
    { var_ustring::create (DBUS_MANAGER_IFACE),
      var_ustring::create ("EqualizedSinks") }
  );
  auto result = this->proxy->call_sync ("Get", params);
  auto sinks = VariantBase::cast_dynamic<var_var_vec_ustring> (
    result.get_child ()
  ).get ().get ();

  return sinks;
}

/**
 * Connect to the specified sink.
 */
void
PAEqualizer::connect_to_sink (ustring sink)
{
  GDBusProxy* c_proxy = nullptr;
  GDBusProxy* c_props_proxy = nullptr;
  GError* c_error = nullptr;

  c_proxy = g_dbus_proxy_new_sync (
    this->conn->gobj (),                // DBus connection
    G_DBUS_PROXY_FLAGS_NONE,            // Proxy flags
    NULL,                               // GDBusInterfaceInfo
    NULL,                               // Bus name
    sink.c_str (),                      // Object path
    DBUS_EQUALIZER_IFACE,               // Interface name
    NULL,                               // GCancellable
    &c_error
  );

  if (c_error) throw PAException (c_error->message);
  if (!c_proxy) throw PAException ("Failed to create a proxy for pulseaudio.");

  c_props_proxy = g_dbus_proxy_new_sync (
    this->conn->gobj (),                // DBus connection
    G_DBUS_PROXY_FLAGS_NONE,            // Proxy flags
    NULL,                               // GDBusInterfaceInfo
    NULL,                               // Bus name
    sink.c_str (),                      // Object path
    DBUS_PROPERTIES_IFACE,              // Interface name
    NULL,                               // GCancellable
    &c_error
  );

  if (c_error) throw PAException (c_error->message);
  if (!c_props_proxy) throw PAException ("Failed to create a proxy for pulseaudio.");
}
