
.. _stdlib_random:

========================
Random generator library
========================

.. include:: detail/random.rst

The random library implements basic random routines.

All functions and symbols are in "random" module, use require to get access to it. ::

    require random


+++++++++
Constants
+++++++++

.. _global-random-LCG_RAND_MAX:

.. das:attribute:: LCG_RAND_MAX = 32767

|variable-random-LCG_RAND_MAX|

.. _global-random-LCG_RAND_MAX_BIG:

.. das:attribute:: LCG_RAND_MAX_BIG = 1073741823

|variable-random-LCG_RAND_MAX_BIG|

.. _global-random-LCG_IRAND_MAX_FLT:

.. das:attribute:: LCG_IRAND_MAX_FLT = 3.05185e-05f

|variable-random-LCG_IRAND_MAX_FLT|

+++++++++++++++++++++++++
Seed and basic generators
+++++++++++++++++++++++++

  *  :ref:`random_seed (seed:int const) : int4 <function-_at_random_c__c_random_seed_Ci>` 
  *  :ref:`random_int (seed:int4& -const) : int <function-_at_random_c__c_random_int_&i4>` 
  *  :ref:`random_int4 (seed:int4& -const) : int4 <function-_at_random_c__c_random_int4_&i4>` 
  *  :ref:`random_float (seed:int4& -const) : float <function-_at_random_c__c_random_float_&i4>` 
  *  :ref:`random_float4 (seed:int4& -const) : float4 <function-_at_random_c__c_random_float4_&i4>` 

.. _function-_at_random_c__c_random_seed_Ci:

.. das:function:: random_seed(seed: int const)

random_seed returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int const    +
+--------+-------------+


|function-random-random_seed|

.. _function-_at_random_c__c_random_int_&i4:

.. das:function:: random_int(seed: int4&)

random_int returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_int|

.. _function-_at_random_c__c_random_int4_&i4:

.. das:function:: random_int4(seed: int4&)

random_int4 returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_int4|

.. _function-_at_random_c__c_random_float_&i4:

.. das:function:: random_float(seed: int4&)

random_float returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_float|

.. _function-_at_random_c__c_random_float4_&i4:

.. das:function:: random_float4(seed: int4&)

random_float4 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_float4|

++++++++++++++++++++++
Specific distributions
++++++++++++++++++++++

  *  :ref:`random_unit_vector (seed:int4& -const) : float3 <function-_at_random_c__c_random_unit_vector_&i4>` 
  *  :ref:`random_in_unit_sphere (seed:int4& -const) : float3 <function-_at_random_c__c_random_in_unit_sphere_&i4>` 
  *  :ref:`random_in_unit_disk (seed:int4& -const) : float3 <function-_at_random_c__c_random_in_unit_disk_&i4>` 

.. _function-_at_random_c__c_random_unit_vector_&i4:

.. das:function:: random_unit_vector(seed: int4&)

random_unit_vector returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_unit_vector|

.. _function-_at_random_c__c_random_in_unit_sphere_&i4:

.. das:function:: random_in_unit_sphere(seed: int4&)

random_in_unit_sphere returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_in_unit_sphere|

.. _function-_at_random_c__c_random_in_unit_disk_&i4:

.. das:function:: random_in_unit_disk(seed: int4&)

random_in_unit_disk returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_in_unit_disk|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_random_c__c_random_seed2D_&i4_Ci2_Ci:

.. das:function:: random_seed2D(seed: int4&; co: int2 const; cf: int const)

random_seed2D returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+
+co      +int2 const   +
+--------+-------------+
+cf      +int const    +
+--------+-------------+


|function-random-random_seed2D|

.. _function-_at_random_c__c_random_big_int_&i4:

.. das:function:: random_big_int(seed: int4&)

random_big_int returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +int4&        +
+--------+-------------+


|function-random-random_big_int|


