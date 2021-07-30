
.. _stdlib_random:

========================
Random generator library
========================

.. include:: detail/random.rst

The random library implements basic random routines.

All functions and symbols are in "random" module, use require to get access to it. ::

    require random


+++++++++++++++++++++++++
Seed and basic generators
+++++++++++++++++++++++++

  *  :ref:`random_float (arg0:int4& implicit) : float <function-_at_random_c__c_random_float__hh_ref_hh__hh_implicit_hh_int4>` 
  *  :ref:`random_float4 (arg0:int4& implicit) : float4 <function-_at_random_c__c_random_float4__hh_ref_hh__hh_implicit_hh_int4>` 
  *  :ref:`random_int (arg0:int4& implicit) : int <function-_at_random_c__c_random_int__hh_ref_hh__hh_implicit_hh_int4>` 
  *  :ref:`random_int4 (arg0:int4& implicit) : int4 <function-_at_random_c__c_random_int4__hh_ref_hh__hh_implicit_hh_int4>` 
  *  :ref:`random_seed (arg0:int const) : int4 <function-_at_random_c__c_random_seed__hh_const_hh_int>` 

.. _function-_at_random_c__c_random_float__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_float(arg0: int4& implicit)

random_float returns float

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_float|

.. _function-_at_random_c__c_random_float4__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_float4(arg0: int4& implicit)

random_float4 returns float4

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_float4|

.. _function-_at_random_c__c_random_int__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_int(arg0: int4& implicit)

random_int returns int

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_int|

.. _function-_at_random_c__c_random_int4__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_int4(arg0: int4& implicit)

random_int4 returns int4

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_int4|

.. _function-_at_random_c__c_random_seed__hh_const_hh_int:

.. das:function:: random_seed(arg0: int const)

random_seed returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+arg0    +int const    +
+--------+-------------+


|function-random-random_seed|

++++++++++++++++++++++
Specific distributions
++++++++++++++++++++++

  *  :ref:`random_in_unit_disk (arg0:int4& implicit) : float3 <function-_at_random_c__c_random_in_unit_disk__hh_ref_hh__hh_implicit_hh_int4>` 
  *  :ref:`random_in_unit_sphere (arg0:int4& implicit) : float3 <function-_at_random_c__c_random_in_unit_sphere__hh_ref_hh__hh_implicit_hh_int4>` 
  *  :ref:`random_unit_vector (arg0:int4& implicit) : float3 <function-_at_random_c__c_random_unit_vector__hh_ref_hh__hh_implicit_hh_int4>` 

.. _function-_at_random_c__c_random_in_unit_disk__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_in_unit_disk(arg0: int4& implicit)

random_in_unit_disk returns float3

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_in_unit_disk|

.. _function-_at_random_c__c_random_in_unit_sphere__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_in_unit_sphere(arg0: int4& implicit)

random_in_unit_sphere returns float3

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_in_unit_sphere|

.. _function-_at_random_c__c_random_unit_vector__hh_ref_hh__hh_implicit_hh_int4:

.. das:function:: random_unit_vector(arg0: int4& implicit)

random_unit_vector returns float3

+--------+--------------+
+argument+argument type +
+========+==============+
+arg0    +int4& implicit+
+--------+--------------+


|function-random-random_unit_vector|


