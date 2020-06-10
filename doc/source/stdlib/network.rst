
.. _stdlib_network:

======================
Network socket library
======================

++++++++++++++++++
Handled structures
++++++++++++++++++

.. das:attribute:: NetworkServer

to be documented


+++++++
Classes
+++++++

.. das:class:: Server

to be documented

it defines as follows ::

  __rtti  : void?
  _server : smart_ptr<network::NetworkServer>

.. das:method:: Server.init (self:network::Server; port:int const) 

init returns bool

function arguments are

+----+---------+
+port+int const+
+----+---------+


to be documented


.. das:method:: Server.restore (self:network::Server; shared_orphan:smart_ptr<network::NetworkServer>&) 

function arguments are

+-------------+----------------------------------+
+shared_orphan+smart_ptr<network::NetworkServer>&+
+-------------+----------------------------------+


to be documented


.. das:method:: Server.save (self:network::Server; shared_orphan:smart_ptr<network::NetworkServer>&) 

function arguments are

+-------------+----------------------------------+
+shared_orphan+smart_ptr<network::NetworkServer>&+
+-------------+----------------------------------+


to be documented


.. das:method:: Server.has_session (self:network::Server) 

has_session returns bool

to be documented


.. das:method:: Server.is_open (self:network::Server) 

is_open returns bool

to be documented


.. das:method:: Server.is_connected (self:network::Server) 

is_connected returns bool

to be documented


.. das:method:: Server.tick (self:network::Server) 

to be documented


.. das:method:: Server.send (self:network::Server; data:uint8? const; size:int const) 

send returns bool

function arguments are

+----+------------+
+data+uint8? const+
+----+------------+
+size+int const   +
+----+------------+


to be documented


.. das:method:: Server.onConnect (self:network::Server) 

to be documented


.. das:method:: Server.onDisconnect (self:network::Server) 

to be documented


.. das:method:: Server.onData (self:network::Server; buf:uint8? const; size:int const) 

function arguments are

+----+------------+
+buf +uint8? const+
+----+------------+
+size+int const   +
+----+------------+


to be documented


.. das:method:: Server.onError (self:network::Server; msg:string const; code:int const) 

function arguments are

+----+------------+
+msg +string const+
+----+------------+
+code+int const   +
+----+------------+


to be documented


.. das:method:: Server.onLog (self:network::Server; msg:string const) 

function arguments are

+---+------------+
+msg+string const+
+---+------------+


to be documented



+++++++++
Functions
+++++++++

.. das:function:: make_server(arg0:void? const implicit; arg1:rtti::StructInfo const? const implicit)

make_server returns bool



function arguments are

+----+--------------------------------------+
+arg0+void? const implicit                  +
+----+--------------------------------------+
+arg1+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg2+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: server_init(arg0:smart_ptr<network::NetworkServer> const implicit; arg1:int const)

server_init returns bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+int const                                       +
+----+------------------------------------------------+
+arg2+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_is_connected(arg0:smart_ptr<network::NetworkServer> const implicit)

server_is_connected returns bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_is_open(arg0:smart_ptr<network::NetworkServer> const implicit)

server_is_open returns bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_restore(arg0:smart_ptr<network::NetworkServer> const implicit; arg1:void? const implicit; arg2:rtti::StructInfo const? const implicit)



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+void? const implicit                            +
+----+------------------------------------------------+
+arg2+rtti::StructInfo const? const implicit          +
+----+------------------------------------------------+
+arg3+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_send(arg0:smart_ptr<network::NetworkServer> const implicit; arg1:uint8? const implicit; arg2:int const)

server_send returns bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+uint8? const implicit                           +
+----+------------------------------------------------+
+arg2+int const                                       +
+----+------------------------------------------------+
+arg3+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_tick(arg0:smart_ptr<network::NetworkServer> const implicit)



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented



