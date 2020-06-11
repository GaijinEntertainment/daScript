
.. _stdlib_network:

======================
Network socket library
======================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle_network_NetworkServer:

.. das:attribute:: NetworkServer

to be documented


+++++++
Classes
+++++++

.. _struct-network-Server:

.. das:class:: Server

to be documented

it defines as follows

  __rtti  : void?
  _server : smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` >

.. das:method:: Server.init ( self:Server; port:int const )  : bool 

init returns bool

method arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+port+int const                                       +
+----+------------------------------------------------+


to be documented


.. das:method:: Server.restore ( self:Server; shared_orphan:smart_ptr<network::NetworkServer>& )  

method arguments are

+-------------+--------------------------------------------------------------------------+
+self         + :ref:`network::Server <struct-network-Server>`                           +
+-------------+--------------------------------------------------------------------------+
+shared_orphan+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` >&+
+-------------+--------------------------------------------------------------------------+


to be documented


.. das:method:: Server.save ( self:Server; shared_orphan:smart_ptr<network::NetworkServer>& )  

method arguments are

+-------------+--------------------------------------------------------------------------+
+self         + :ref:`network::Server <struct-network-Server>`                           +
+-------------+--------------------------------------------------------------------------+
+shared_orphan+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` >&+
+-------------+--------------------------------------------------------------------------+


to be documented


.. das:method:: Server.has_session ( self:Server )  : bool 

has_session returns bool

to be documented


.. das:method:: Server.is_open ( self:Server )  : bool 

is_open returns bool

to be documented


.. das:method:: Server.is_connected ( self:Server )  : bool 

is_connected returns bool

to be documented


.. das:method:: Server.tick ( self:Server )  

to be documented


.. das:method:: Server.send ( self:Server; data:uint8? const; size:int const )  : bool 

send returns bool

method arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+data+uint8? const                                    +
+----+------------------------------------------------+
+size+int const                                       +
+----+------------------------------------------------+


to be documented


.. das:method:: Server.onConnect ( self:Server )  

to be documented


.. das:method:: Server.onDisconnect ( self:Server )  

to be documented


.. das:method:: Server.onData ( self:Server; buf:uint8? const; size:int const )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+buf +uint8? const                                    +
+----+------------------------------------------------+
+size+int const                                       +
+----+------------------------------------------------+


to be documented


.. das:method:: Server.onError ( self:Server; msg:string const; code:int const )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+msg +string const                                    +
+----+------------------------------------------------+
+code+int const                                       +
+----+------------------------------------------------+


to be documented


.. das:method:: Server.onLog ( self:Server; msg:string const )  

method arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+msg +string const                                    +
+----+------------------------------------------------+


to be documented



+++++++++
Functions
+++++++++

.. das:function:: make_server ( arg0:void? const implicit; arg1:rtti::StructInfo const? const implicit )  : bool

make_server returns bool



function arguments are

+----+------------------------------------------------------------------------+
+arg0+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg1+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+



to be documented


.. das:function:: server_init ( arg0:smart_ptr<network::NetworkServer> const implicit; arg1:int const )  : bool

server_init returns bool



function arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+
+arg1+int const                                                                               +
+----+----------------------------------------------------------------------------------------+



to be documented


.. das:function:: server_is_connected ( arg0:smart_ptr<network::NetworkServer> const implicit )  : bool

server_is_connected returns bool



function arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+



to be documented


.. das:function:: server_is_open ( arg0:smart_ptr<network::NetworkServer> const implicit )  : bool

server_is_open returns bool



function arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+



to be documented


.. das:function:: server_restore ( arg0:smart_ptr<network::NetworkServer> const implicit; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit ) 



function arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+
+arg1+void? const implicit                                                                    +
+----+----------------------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle_rtti_StructInfo>`  const? const implicit                +
+----+----------------------------------------------------------------------------------------+



to be documented


.. das:function:: server_send ( arg0:smart_ptr<network::NetworkServer> const implicit; arg1:uint8? const implicit; arg2:int const )  : bool

server_send returns bool



function arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+
+arg1+uint8? const implicit                                                                   +
+----+----------------------------------------------------------------------------------------+
+arg2+int const                                                                               +
+----+----------------------------------------------------------------------------------------+



to be documented


.. das:function:: server_tick ( arg0:smart_ptr<network::NetworkServer> const implicit ) 



function arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle_network_NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+



to be documented



