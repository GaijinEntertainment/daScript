
.. _stdlib_network:

======================
Network socket library
======================

.. include:: detail/network.rst

|module-network|

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-network-NetworkServer:

.. das:attribute:: NetworkServer

|structure_annotation-network-NetworkServer|

+++++++
Classes
+++++++

.. _struct-network-Server:

.. das:attribute:: Server

|class-network-Server|

it defines as follows

  __rtti  : void?
  _server : smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` >

.. das:function:: Server.init(self: Server; port: int const)

init returns bool

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`network::Server <struct-network-Server>` +
+--------+------------------------------------------------+
+port    +int const                                       +
+--------+------------------------------------------------+


|method-network-Server.init|

.. das:function:: Server.restore(self: Server; shared_orphan: smart_ptr<network::NetworkServer>&)

+-------------+--------------------------------------------------------------------------+
+argument     +argument type                                                             +
+=============+==========================================================================+
+self         + :ref:`network::Server <struct-network-Server>`                           +
+-------------+--------------------------------------------------------------------------+
+shared_orphan+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` >&+
+-------------+--------------------------------------------------------------------------+


|method-network-Server.restore|

.. das:function:: Server.save(self: Server; shared_orphan: smart_ptr<network::NetworkServer>&)

+-------------+--------------------------------------------------------------------------+
+argument     +argument type                                                             +
+=============+==========================================================================+
+self         + :ref:`network::Server <struct-network-Server>`                           +
+-------------+--------------------------------------------------------------------------+
+shared_orphan+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` >&+
+-------------+--------------------------------------------------------------------------+


|method-network-Server.save|

.. das:function:: Server.has_session(self: Server)

has_session returns bool

|method-network-Server.has_session|

.. das:function:: Server.is_open(self: Server)

is_open returns bool

|method-network-Server.is_open|

.. das:function:: Server.is_connected(self: Server)

is_connected returns bool

|method-network-Server.is_connected|

.. das:function:: Server.tick(self: Server)

|method-network-Server.tick|

.. das:function:: Server.send(self: Server; data: uint8? const; size: int const)

send returns bool

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`network::Server <struct-network-Server>` +
+--------+------------------------------------------------+
+data    +uint8? const                                    +
+--------+------------------------------------------------+
+size    +int const                                       +
+--------+------------------------------------------------+


|method-network-Server.send|

.. das:function:: Server.onConnect(self: Server)

|method-network-Server.onConnect|

.. das:function:: Server.onDisconnect(self: Server)

|method-network-Server.onDisconnect|

.. das:function:: Server.onData(self: Server; buf: uint8? const; size: int const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`network::Server <struct-network-Server>` +
+--------+------------------------------------------------+
+buf     +uint8? const                                    +
+--------+------------------------------------------------+
+size    +int const                                       +
+--------+------------------------------------------------+


|method-network-Server.onData|

.. das:function:: Server.onError(self: Server; msg: string const; code: int const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`network::Server <struct-network-Server>` +
+--------+------------------------------------------------+
+msg     +string const                                    +
+--------+------------------------------------------------+
+code    +int const                                       +
+--------+------------------------------------------------+


|method-network-Server.onError|

.. das:function:: Server.onLog(self: Server; msg: string const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+self    + :ref:`network::Server <struct-network-Server>` +
+--------+------------------------------------------------+
+msg     +string const                                    +
+--------+------------------------------------------------+


|method-network-Server.onLog|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_network_c__c_make_server__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: make_server(arg0: void? const implicit; arg1: rtti::StructInfo const? const implicit)

make_server returns bool

+--------+------------------------------------------------------------------------+
+argument+argument type                                                           +
+========+========================================================================+
+arg0    +void? const implicit                                                    +
+--------+------------------------------------------------------------------------+
+arg1    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+--------+------------------------------------------------------------------------+


|function-network-make_server|

.. _function-_at_network_c__c_server_init__hh_smart_ptr_hh__hh_handle_hh_NetworkServer_hh_const_hh_implicit_int_hh_const__hh_const:

.. das:function:: server_init(arg0: smart_ptr<network::NetworkServer> const implicit; arg1: int const)

server_init returns bool

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+arg0    +smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+--------+----------------------------------------------------------------------------------------+
+arg1    +int const                                                                               +
+--------+----------------------------------------------------------------------------------------+


|function-network-server_init|

.. _function-_at_network_c__c_server_is_connected__hh_smart_ptr_hh__hh_handle_hh_NetworkServer_hh_const_hh_implicit__hh_const:

.. das:function:: server_is_connected(arg0: smart_ptr<network::NetworkServer> const implicit)

server_is_connected returns bool

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+arg0    +smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+--------+----------------------------------------------------------------------------------------+


|function-network-server_is_connected|

.. _function-_at_network_c__c_server_is_open__hh_smart_ptr_hh__hh_handle_hh_NetworkServer_hh_const_hh_implicit__hh_const:

.. das:function:: server_is_open(arg0: smart_ptr<network::NetworkServer> const implicit)

server_is_open returns bool

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+arg0    +smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+--------+----------------------------------------------------------------------------------------+


|function-network-server_is_open|

.. _function-_at_network_c__c_server_restore__hh_smart_ptr_hh__hh_handle_hh_NetworkServer_hh_const_hh_implicit__hh_ptr_hh_const_hh_implicit__hh_ptr_hh__hh_handle_hh_StructInfo_hh_const_hh_const_hh_implicit__hh_const:

.. das:function:: server_restore(arg0: smart_ptr<network::NetworkServer> const implicit; arg1: void? const implicit; arg2: rtti::StructInfo const? const implicit)

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+arg0    +smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+--------+----------------------------------------------------------------------------------------+
+arg1    +void? const implicit                                                                    +
+--------+----------------------------------------------------------------------------------------+
+arg2    + :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit                +
+--------+----------------------------------------------------------------------------------------+


|function-network-server_restore|

.. _function-_at_network_c__c_server_send__hh_smart_ptr_hh__hh_handle_hh_NetworkServer_hh_const_hh_implicit__hh_ptr_hh_uint8_hh_const_hh_implicit_int_hh_const__hh_const:

.. das:function:: server_send(arg0: smart_ptr<network::NetworkServer> const implicit; arg1: uint8? const implicit; arg2: int const)

server_send returns bool

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+arg0    +smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+--------+----------------------------------------------------------------------------------------+
+arg1    +uint8? const implicit                                                                   +
+--------+----------------------------------------------------------------------------------------+
+arg2    +int const                                                                               +
+--------+----------------------------------------------------------------------------------------+


|function-network-server_send|

.. _function-_at_network_c__c_server_tick__hh_smart_ptr_hh__hh_handle_hh_NetworkServer_hh_const_hh_implicit__hh_const:

.. das:function:: server_tick(arg0: smart_ptr<network::NetworkServer> const implicit)

+--------+----------------------------------------------------------------------------------------+
+argument+argument type                                                                           +
+========+========================================================================================+
+arg0    +smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+--------+----------------------------------------------------------------------------------------+


|function-network-server_tick|


