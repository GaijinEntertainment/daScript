
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

.. das:function:: Server->__finalize (self) 

to be documented


.. das:function:: Server->init (self;port) 

init returns ::

 bool

function arguments are

+----+---------+
+port+int const+
+----+---------+


to be documented


.. das:function:: Server->restore (self;shared_orphan) 

function arguments are

+-------------+-----------------------------------------+
+shared_orphan+smart_ptr<network::NetworkServer>& -const+
+-------------+-----------------------------------------+


to be documented


.. das:function:: Server->save (self;shared_orphan) 

function arguments are

+-------------+-----------------------------------------+
+shared_orphan+smart_ptr<network::NetworkServer>& -const+
+-------------+-----------------------------------------+


to be documented


.. das:function:: Server->has_session (self) 

has_session returns ::

 bool

to be documented


.. das:function:: Server->is_open (self) 

is_open returns ::

 bool

to be documented


.. das:function:: Server->is_connected (self) 

is_connected returns ::

 bool

to be documented


.. das:function:: Server->tick (self) 

to be documented


.. das:function:: Server->send (self;data;size) 

send returns ::

 bool

function arguments are

+----+------------+
+data+uint8? const+
+----+------------+
+size+int const   +
+----+------------+


to be documented


.. das:function:: Server->onConnect (self) 

to be documented


.. das:function:: Server->onDisconnect (self) 

to be documented


.. das:function:: Server->onData (self;buf;size) 

function arguments are

+----+------------+
+buf +uint8? const+
+----+------------+
+size+int const   +
+----+------------+


to be documented


.. das:function:: Server->onError (self;msg;code) 

function arguments are

+----+------------+
+msg +string const+
+----+------------+
+code+int const   +
+----+------------+


to be documented


.. das:function:: Server->onLog (self;msg) 

function arguments are

+---+------------+
+msg+string const+
+---+------------+


to be documented



+++++++++
Functions
+++++++++

.. das:function:: make_server(arg0;arg1;arg2)

make_server returns ::

 bool



function arguments are

+----+--------------------------------------+
+arg0+void? const implicit                  +
+----+--------------------------------------+
+arg1+rtti::StructInfo const? const implicit+
+----+--------------------------------------+
+arg2+ const                                +
+----+--------------------------------------+



to be documented


.. das:function:: server_init(arg0;arg1;arg2)

server_init returns ::

 bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+int const                                       +
+----+------------------------------------------------+
+arg2+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_is_connected(arg0;arg1)

server_is_connected returns ::

 bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_is_open(arg0;arg1)

server_is_open returns ::

 bool



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented


.. das:function:: server_restore(arg0;arg1;arg2;arg3)



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


.. das:function:: server_send(arg0;arg1;arg2;arg3)

server_send returns ::

 bool



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


.. das:function:: server_tick(arg0;arg1)



function arguments are

+----+------------------------------------------------+
+arg0+smart_ptr<network::NetworkServer> const implicit+
+----+------------------------------------------------+
+arg1+ const                                          +
+----+------------------------------------------------+



to be documented



