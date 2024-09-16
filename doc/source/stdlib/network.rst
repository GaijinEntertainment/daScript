
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

// stub


+++++++
Classes
+++++++

.. _struct-network-Server:

.. das:attribute:: Server

:Fields: * **_server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >

// stub


.. das:function:: Server.make_server_adapter()

// stub


.. das:function:: Server.init(port: int) : bool

// stub



:Arguments: * **port** : int

.. das:function:: Server.restore(shared_orphan: smart_ptr<NetworkServer>&)

// stub



:Arguments: * **shared_orphan** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >&

.. das:function:: Server.save(shared_orphan: smart_ptr<NetworkServer>&)

// stub



:Arguments: * **shared_orphan** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` >&

.. das:function:: Server.has_session() : bool

// stub


.. das:function:: Server.is_open() : bool

// stub


.. das:function:: Server.is_connected() : bool

// stub


.. das:function:: Server.tick()

// stub


.. das:function:: Server.send(data: uint8?; size: int) : bool

// stub



:Arguments: * **data** : uint8?

            * **size** : int

.. _function-_at_network_c__c_Server:

.. das:function:: Server() : Server

// stub


++++++++++++++++++++++++++
Low lever NetworkServer IO
++++++++++++++++++++++++++

  *  :ref:`make_server (class: void? implicit; info: StructInfo const? implicit) : bool <function-_at_network_c__c_make_server_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c>` 
  *  :ref:`server_init (server: smart_ptr\<NetworkServer\> implicit; port: int) : bool <function-_at_network_c__c_server_init_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_Ci_C_c_C_l>` 
  *  :ref:`server_is_open (server: smart_ptr\<NetworkServer\> implicit) : bool <function-_at_network_c__c_server_is_open_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`server_is_connected (server: smart_ptr\<NetworkServer\> implicit) : bool <function-_at_network_c__c_server_is_connected_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`server_tick (server: smart_ptr\<NetworkServer\> implicit) <function-_at_network_c__c_server_tick_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_C_c_C_l>` 
  *  :ref:`server_send (server: smart_ptr\<NetworkServer\> implicit; data: uint8? implicit; size: int) : bool <function-_at_network_c__c_server_send_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_CI1_ls_u8_gr__qm__Ci_C_c_C_l>` 
  *  :ref:`server_restore (server: smart_ptr\<NetworkServer\> implicit; class: void? implicit; info: StructInfo const? implicit) <function-_at_network_c__c_server_restore_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c_C_l>` 

.. _function-_at_network_c__c_make_server_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c:

.. das:function:: make_server(class: void? implicit; info: StructInfo const? implicit) : bool

// stub



:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit

.. _function-_at_network_c__c_server_init_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_Ci_C_c_C_l:

.. das:function:: server_init(server: smart_ptr<NetworkServer> implicit; port: int) : bool

// stub



:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **port** : int

.. _function-_at_network_c__c_server_is_open_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_C_c_C_l:

.. das:function:: server_is_open(server: smart_ptr<NetworkServer> implicit) : bool

// stub



:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-_at_network_c__c_server_is_connected_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_C_c_C_l:

.. das:function:: server_is_connected(server: smart_ptr<NetworkServer> implicit) : bool

// stub



:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-_at_network_c__c_server_tick_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_C_c_C_l:

.. das:function:: server_tick(server: smart_ptr<NetworkServer> implicit)

// stub



:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

.. _function-_at_network_c__c_server_send_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_CI1_ls_u8_gr__qm__Ci_C_c_C_l:

.. das:function:: server_send(server: smart_ptr<NetworkServer> implicit; data: uint8? implicit; size: int) : bool

// stub



:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **data** : uint8? implicit

            * **size** : int

.. _function-_at_network_c__c_server_restore_CI1_ls_H_ls_network_c__c_NetworkServer_gr__gr__qm_M_CI_qm__CI1_ls_CH_ls_rtti_c__c_StructInfo_gr__gr__qm__C_c_C_l:

.. das:function:: server_restore(server: smart_ptr<NetworkServer> implicit; class: void? implicit; info: StructInfo const? implicit)

// stub



:Arguments: * **server** : smart_ptr< :ref:`NetworkServer <handle-network-NetworkServer>` > implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>` ? implicit


