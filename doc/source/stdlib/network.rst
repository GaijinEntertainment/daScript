
.. _stdlib_network:

======================
Network socket library
======================

The NETWORK module implements networking facilities including HTTP client/server
and low-level socket operations. It provides ``Server`` and ``Client`` classes
with event-driven callbacks for handling connections, requests, and responses.

All functions and symbols are in "network" module, use require to get access to it. ::

    require network

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-network-NetworkServer:

.. das:attribute:: NetworkServer

 Base implementation of the server.


+++++++
Classes
+++++++

.. _struct-network-Server:

.. das:attribute:: Server

:Fields: * **_server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > - Single-socket listener that manages one client connection at a time.


.. _function-network_Server_rq_make_server_adapter_Server_0xe:

.. das:function:: Server.make_server_adapter()

Creates a low-level server adapter bound to this ``Server`` instance.

.. _function-network_Server_rq_init_Server_int_0x16:

.. das:function:: Server.init(port: int) : bool

Initializes the server on the specified port; returns ``true`` on success.

:Arguments: * **port** : int

.. _function-network_Server_rq_restore_Server_smart_ptr_ls_NetworkServer_gr__0x19:

.. das:function:: Server.restore(shared_orphan: smart_ptr<NetworkServer>&)

Restores the server with the given shared orphan network server pointer.
This is necessary to re-establish the server state after reload of a script.

:Arguments: * **shared_orphan** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >&

.. _function-network_Server_rq_save_Server_smart_ptr_ls_NetworkServer_gr__0x20:

.. das:function:: Server.save(shared_orphan: smart_ptr<NetworkServer>&)

Saves the server state to a shared orphan network server pointer.
This is necessary to re-establish the server state after reload of a script.

:Arguments: * **shared_orphan** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >&

.. _function-network_Server_rq_has_session_Server_0x23:

.. das:function:: Server.has_session() : bool

Returns ``true`` if the server has an active client session.

.. _function-network_Server_rq_is_open_Server_0x26:

.. das:function:: Server.is_open() : bool

Returns ``true`` if the server is open and accepting connections.

.. _function-network_Server_rq_is_connected_Server_0x29:

.. das:function:: Server.is_connected() : bool

Returns ``true`` if the server is currently connected to a client.

.. _function-network_Server_rq_tick_Server_0x2c:

.. das:function:: Server.tick()

Processes pending connections and incoming data; must be called periodically.

.. _function-network_Server_rq_send_Server_uint8_q__int_0x31:

.. das:function:: Server.send(data: uint8?; size: int) : bool

Sends a data buffer to the connected client.

:Arguments: * **data** : uint8?

            * **size** : int

.. _function-network_Server_0xb:

.. das:function:: Server() : Server

Constructs a new ``Server`` instance with default settings.

++++++++++++++++++++++++++
Low level NetworkServer IO
++++++++++++++++++++++++++

  *  :ref:`make_server (class: void? implicit; info: StructInfo const? implicit) : bool <function-network_make_server_void_q__implicit_StructInfo_const_q__implicit>` 
  *  :ref:`server_init (server: smart_ptr\<NetworkServer\> implicit; port: int) : bool <function-network_server_init_smart_ptr_ls_NetworkServer_gr__implicit_int>` 
  *  :ref:`server_is_open (server: smart_ptr\<NetworkServer\> implicit) : bool <function-network_server_is_open_smart_ptr_ls_NetworkServer_gr__implicit>` 
  *  :ref:`server_is_connected (server: smart_ptr\<NetworkServer\> implicit) : bool <function-network_server_is_connected_smart_ptr_ls_NetworkServer_gr__implicit>` 
  *  :ref:`server_tick (server: smart_ptr\<NetworkServer\> implicit) <function-network_server_tick_smart_ptr_ls_NetworkServer_gr__implicit>` 
  *  :ref:`server_send (server: smart_ptr\<NetworkServer\> implicit; data: uint8? implicit; size: int) : bool <function-network_server_send_smart_ptr_ls_NetworkServer_gr__implicit_uint8_q__implicit_int>` 
  *  :ref:`server_restore (server: smart_ptr\<NetworkServer\> implicit; class: void? implicit; info: StructInfo const? implicit) <function-network_server_restore_smart_ptr_ls_NetworkServer_gr__implicit_void_q__implicit_StructInfo_const_q__implicit>` 

.. _function-network_make_server_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: make_server(class: void? implicit; info: StructInfo const? implicit) : bool

Creates a new ``Server`` instance.

:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-network_server_init_smart_ptr_ls_NetworkServer_gr__implicit_int:

.. das:function:: server_init(server: smart_ptr<NetworkServer> implicit; port: int) : bool

Initializes the server to listen on the specified port.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **port** : int

.. _function-network_server_is_open_smart_ptr_ls_NetworkServer_gr__implicit:

.. das:function:: server_is_open(server: smart_ptr<NetworkServer> implicit) : bool

Returns ``true`` if the server is listening on its bound port.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-network_server_is_connected_smart_ptr_ls_NetworkServer_gr__implicit:

.. das:function:: server_is_connected(server: smart_ptr<NetworkServer> implicit) : bool

Returns ``true`` if the server has an active client connection.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-network_server_tick_smart_ptr_ls_NetworkServer_gr__implicit:

.. das:function:: server_tick(server: smart_ptr<NetworkServer> implicit)

Processes pending network I/O; must be called periodically for the server to function.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-network_server_send_smart_ptr_ls_NetworkServer_gr__implicit_uint8_q__implicit_int:

.. das:function:: server_send(server: smart_ptr<NetworkServer> implicit; data: uint8? implicit; size: int) : bool

Sends data from the server to the connected client.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **data** : uint8? implicit

            * **size** : int

.. _function-network_server_restore_smart_ptr_ls_NetworkServer_gr__implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: server_restore(server: smart_ptr<NetworkServer> implicit; class: void? implicit; info: StructInfo const? implicit)

Restores a server from an orphaned or interrupted state.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit


