
.. _stdlib_network:

======================
Network socket library
======================

The NETWORK module implements basic TCP socket listening server (currently only one connection).
It would eventually be expanded to support client as well.

It its present form its used in Daslang Visual Studio Code plugin and upcoming debug server.

All functions and symbols are in "network" module, use require to get access to it. ::

    require network

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-network-NetworkServer:

.. das:attribute:: NetworkServer

 Base impliemntation of the server.


+++++++
Classes
+++++++

.. _struct-network-Server:

.. das:attribute:: Server

:Fields: * **_server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > -  Single socket listener combined with single socket connection.


.. _function-network_Server_rq_make_server_adapter_Server:

.. das:function:: Server.make_server_adapter()

Creates a server adapter for the given server instance.

.. _function-network_Server_rq_init_Server_int:

.. das:function:: Server.init(port: int) : bool

Returns true if the server was successfully initialized on the given port.

:Arguments: * **port** : int

.. _function-network_Server_rq_restore_Server_smart_ptr_ls_NetworkServer_gr_:

.. das:function:: Server.restore(shared_orphan: smart_ptr<NetworkServer>&)

Restores the server with the given shared orphan network server pointer.
This is necessary to re-establish the server state after reload of a script.


:Arguments: * **shared_orphan** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >&

.. _function-network_Server_rq_save_Server_smart_ptr_ls_NetworkServer_gr_:

.. das:function:: Server.save(shared_orphan: smart_ptr<NetworkServer>&)

Saves the server state to a shared orphan network server pointer.
This is necessary to re-establish the server state after reload of a script.

:Arguments: * **shared_orphan** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >&

.. _function-network_Server_rq_has_session_Server:

.. das:function:: Server.has_session() : bool

Returns true if the server has an active session.

.. _function-network_Server_rq_is_open_Server:

.. das:function:: Server.is_open() : bool

Return true if the server is open and ready to accept connections.

.. _function-network_Server_rq_is_connected_Server:

.. das:function:: Server.is_connected() : bool

Returns true if the server is connected to a client.

.. _function-network_Server_rq_tick_Server:

.. das:function:: Server.tick()

Ticks the server to process incoming connections and data.

.. _function-network_Server_rq_send_Server_uint8_q__int:

.. das:function:: Server.send(data: uint8?; size: int) : bool

Sends data to the connected client. Returns true if the data was sent successfully.

:Arguments: * **data** : uint8?

            * **size** : int

.. _function-network_Server:

.. das:function:: Server() : Server

Initializes and returns a new instance of the network Server class.




++++++++++++++++++++++++++
Low lever NetworkServer IO
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

 Creates new instance of the server.

:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-network_server_init_smart_ptr_ls_NetworkServer_gr__implicit_int:

.. das:function:: server_init(server: smart_ptr<NetworkServer> implicit; port: int) : bool

 Initializes server with given port.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **port** : int

.. _function-network_server_is_open_smart_ptr_ls_NetworkServer_gr__implicit:

.. das:function:: server_is_open(server: smart_ptr<NetworkServer> implicit) : bool

 Returns true if server is listening to the port.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-network_server_is_connected_smart_ptr_ls_NetworkServer_gr__implicit:

.. das:function:: server_is_connected(server: smart_ptr<NetworkServer> implicit) : bool

 Returns true if server is connected to the client.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-network_server_tick_smart_ptr_ls_NetworkServer_gr__implicit:

.. das:function:: server_tick(server: smart_ptr<NetworkServer> implicit)

 This needs to be called periodically for the server to work.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-network_server_send_smart_ptr_ls_NetworkServer_gr__implicit_uint8_q__implicit_int:

.. das:function:: server_send(server: smart_ptr<NetworkServer> implicit; data: uint8? implicit; size: int) : bool

 Sends data from server to the client.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **data** : uint8? implicit

            * **size** : int

.. _function-network_server_restore_smart_ptr_ls_NetworkServer_gr__implicit_void_q__implicit_StructInfo_const_q__implicit:

.. das:function:: server_restore(server: smart_ptr<NetworkServer> implicit; class: void? implicit; info: StructInfo const? implicit)

 Restores server from orphaned state.

:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit


