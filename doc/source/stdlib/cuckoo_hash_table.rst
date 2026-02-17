
.. _stdlib_cuckoo_hash_table:

=================
Cuckoo hash table
=================

.. das:module:: cuckoo_hash_table

The CUCKOO_HASH_TABLE module implements a cuckoo hash table data structure.
Cuckoo hashing provides worst-case O(1) lookup time by using multiple hash
functions and displacing existing entries on collision.

All functions and symbols are in "cuckoo_hash_table" module, use require to get access to it. ::

    require daslib/cuckoo_hash_table



+++++++++++
Type macros
+++++++++++

.. _call-macro-cuckoo_hash_table-TCuckooHashTable:

.. das:attribute:: type macro TCuckooHashTable

:Arguments: * **ValueType** (Type)

            * **hashFunction0Name** (String = "hash0")

            * **hashFunction1Name** (String = "hash_extra")


++++++++++++++
Hash functions
++++++++++++++

  *  :ref:`hash0 (data: auto) : auto <function-cuckoo_hash_table_hash0_auto_0x17>`
  *  :ref:`hash_extra (data: auto) : auto <function-cuckoo_hash_table_hash_extra_auto_0x1d>`

.. _function-cuckoo_hash_table_hash0_auto_0x17:

.. das:function:: hash0(data: auto) : auto

this hash function converts and workhorse key to a 64 bit hash


:Arguments: * **data** : auto

.. _function-cuckoo_hash_table_hash_extra_auto_0x1d:

.. das:function:: hash_extra(data: auto) : auto

Returns a secondary hash derived from the upper 32 bits of the primary hash, used for cuckoo hashing.


:Arguments: * **data** : auto


