
.. _stdlib_network:

======================
Network socket library
======================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-network-NetworkServer:

.. das:attribute:: NetworkServer

structure_annotation|network|NetworkServer to be documented

+++++++
Classes
+++++++

.. _struct-network-Server:

.. das:class:: Server

class|network|Server to be documented

it defines as follows

  __rtti  : void?
  _server : smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` >

.. das:method:: Server.init (self:Server; port:int const )  : bool

init returns bool

arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+port+int const                                       +
+----+------------------------------------------------+


method|network|Server.init to be documented

.. das:method:: Server.restore (self:Server; shared_orphan:smart_ptr<network::NetworkServer>& ) 

arguments are

+-------------+--------------------------------------------------------------------------+
+self         + :ref:`network::Server <struct-network-Server>`                           +
+-------------+--------------------------------------------------------------------------+
+shared_orphan+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` >&+
+-------------+--------------------------------------------------------------------------+


method|network|Server.restore to be documented

.. das:method:: Server.save (self:Server; shared_orphan:smart_ptr<network::NetworkServer>& ) 

arguments are

+-------------+--------------------------------------------------------------------------+
+self         + :ref:`network::Server <struct-network-Server>`                           +
+-------------+--------------------------------------------------------------------------+
+shared_orphan+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` >&+
+-------------+--------------------------------------------------------------------------+


method|network|Server.save to be documented

.. das:method:: Server.has_session (self:Server )  : bool

has_session returns bool

method|network|Server.has_session to be documented

.. das:method:: Server.is_open (self:Server )  : bool

is_open returns bool

method|network|Server.is_open to be documented

.. das:method:: Server.is_connected (self:Server )  : bool

is_connected returns bool

method|network|Server.is_connected to be documented

.. das:method:: Server.tick (self:Server ) 

method|network|Server.tick to be documented

.. das:method:: Server.send (self:Server; data:uint8? const; size:int const )  : bool

send returns bool

arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+data+uint8? const                                    +
+----+------------------------------------------------+
+size+int const                                       +
+----+------------------------------------------------+


method|network|Server.send to be documented

.. das:method:: Server.onConnect (self:Server ) 

method|network|Server.onConnect to be documented

.. das:method:: Server.onDisconnect (self:Server ) 

method|network|Server.onDisconnect to be documented

.. das:method:: Server.onData (self:Server; buf:uint8? const; size:int const ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+buf +uint8? const                                    +
+----+------------------------------------------------+
+size+int const                                       +
+----+------------------------------------------------+


method|network|Server.onData to be documented

.. das:method:: Server.onError (self:Server; msg:string const; code:int const ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+msg +string const                                    +
+----+------------------------------------------------+
+code+int const                                       +
+----+------------------------------------------------+


method|network|Server.onError to be documented

.. das:method:: Server.onLog (self:Server; msg:string const ) 

arguments are

+----+------------------------------------------------+
+self+ :ref:`network::Server <struct-network-Server>` +
+----+------------------------------------------------+
+msg +string const                                    +
+----+------------------------------------------------+


method|network|Server.onLog to be documented

.. das:function:: make_server (arg0:void? const implicit; arg1:rtti::StructInfo const? const implicit )  : bool

make_server returns bool

arguments are

+----+------------------------------------------------------------------------+
+arg0+void? const implicit                                                    +
+----+------------------------------------------------------------------------+
+arg1+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit+
+----+------------------------------------------------------------------------+


function|network|make_server to be documented

.. das:function:: server_init (arg0:smart_ptr<network::NetworkServer> const implicit; arg1:int const )  : bool

server_init returns bool

arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+
+arg1+int const                                                                               +
+----+----------------------------------------------------------------------------------------+


function|network|server_init to be documented

.. das:function:: server_is_connected (arg0:smart_ptr<network::NetworkServer> const implicit )  : bool

server_is_connected returns bool

arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+


function|network|server_is_connected to be documented

.. das:function:: server_is_open (arg0:smart_ptr<network::NetworkServer> const implicit )  : bool

server_is_open returns bool

arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+


function|network|server_is_open to be documented

.. das:function:: server_restore (arg0:smart_ptr<network::NetworkServer> const implicit; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit ) 

arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+
+arg1+void? const implicit                                                                    +
+----+----------------------------------------------------------------------------------------+
+arg2+ :ref:`rtti::StructInfo <handle-rtti-StructInfo>`  const? const implicit                +
+----+----------------------------------------------------------------------------------------+


function|network|server_restore to be documented

.. das:function:: server_send (arg0:smart_ptr<network::NetworkServer> const implicit; arg1:uint8? const implicit; arg2:int const )  : bool

server_send returns bool

arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+
+arg1+uint8? const implicit                                                                   +
+----+----------------------------------------------------------------------------------------+
+arg2+int const                                                                               +
+----+----------------------------------------------------------------------------------------+


function|network|server_send to be documented

.. das:function:: server_tick (arg0:smart_ptr<network::NetworkServer> const implicit ) 

arguments are

+----+----------------------------------------------------------------------------------------+
+arg0+smart_ptr< :ref:`network::NetworkServer <handle-network-NetworkServer>` > const implicit+
+----+----------------------------------------------------------------------------------------+


function|network|server_tick to be documented


