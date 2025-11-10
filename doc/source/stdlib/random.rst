
.. _stdlib_random:

========================
Random generator library
========================

The random library implements basic random routines.

All functions and symbols are in "random" module, use require to get access to it. ::

    require random

+++++++++
Constants
+++++++++

.. _global-random-LCG_RAND_MAX:

.. das:attribute:: LCG_RAND_MAX = 32767

maximum possible output of random number generator

.. _global-random-LCG_RAND_MAX_BIG:

.. das:attribute:: LCG_RAND_MAX_BIG = 1073741823

maximum possible output of random_big_int

+++++++++++++++++++++++++
Seed and basic generators
+++++++++++++++++++++++++

  *  :ref:`random_seed (seed: int) : auto <function-random_random_seed_int>` 
  *  :ref:`random_seed2D (var seed: int4&; co: int2; cf: int = 0) : auto <function-random_random_seed2D_int4_int2_int>` 
  *  :ref:`random_int (var seed: int4&) : auto <function-random_random_int_int4>` 
  *  :ref:`random_big_int (var seed: int4&) : auto <function-random_random_big_int_int4>` 
  *  :ref:`random_uint (var seed: int4&) : auto <function-random_random_uint_int4>` 
  *  :ref:`random_int4 (var seed: int4&) : auto <function-random_random_int4_int4>` 
  *  :ref:`random_float (var seed: int4&) : auto <function-random_random_float_int4>` 
  *  :ref:`random_float4 (var seed: int4&) : auto <function-random_random_float4_int4>` 

.. _function-random_random_seed_int:

.. das:function:: random_seed(seed: int) : auto

constructs seed vector out of single integer seed

:Arguments: * **seed** : int

.. _function-random_random_seed2D_int4_int2_int:

.. das:function:: random_seed2D(seed: int4&; co: int2; cf: int = 0) : auto

constructs seed vector out of 2d screen coordinates and frame counter `cf`

:Arguments: * **seed** : int4&

            * **co** : int2

            * **cf** : int

.. _function-random_random_int_int4:

.. das:function:: random_int(seed: int4&) : auto

random integer 0..32767 (LCG_RAND_MAX)

:Arguments: * **seed** : int4&

.. _function-random_random_big_int_int4:

.. das:function:: random_big_int(seed: int4&) : auto

random integer 0..32768*32768-1 (LCG_RAND_MAX_BIG)

:Arguments: * **seed** : int4&

.. _function-random_random_uint_int4:

.. das:function:: random_uint(seed: int4&) : auto

random integer 0..32768*32768-1 (LCG_RAND_MAX_BIG)

:Arguments: * **seed** : int4&

.. _function-random_random_int4_int4:

.. das:function:: random_int4(seed: int4&) : auto

random int4, each component is 0..32767 (LCG_RAND_MAX)

:Arguments: * **seed** : int4&

.. _function-random_random_float_int4:

.. das:function:: random_float(seed: int4&) : auto

random float 0..1

:Arguments: * **seed** : int4&

.. _function-random_random_float4_int4:

.. das:function:: random_float4(seed: int4&) : auto

random float4, each component is 0..1

:Arguments: * **seed** : int4&

++++++++++++++++
Random iterators
++++++++++++++++

  *  :ref:`each_random_uint (rnd_seed: int = 13) : iterator\<uint\> <function-random_each_random_uint_int>` 

.. _function-random_each_random_uint_int:

.. das:function:: each_random_uint(rnd_seed: int = 13) : iterator<uint>

infinite generator of random uints initialized with `rnd_seed`

:Arguments: * **rnd_seed** : int

++++++++++++++++++++++
Specific distributions
++++++++++++++++++++++

  *  :ref:`random_unit_vector (var seed: int4&) : auto <function-random_random_unit_vector_int4>` 
  *  :ref:`random_in_unit_sphere (var seed: int4&) : auto <function-random_random_in_unit_sphere_int4>` 
  *  :ref:`random_in_unit_disk (var seed: int4&) : auto <function-random_random_in_unit_disk_int4>` 

.. _function-random_random_unit_vector_int4:

.. das:function:: random_unit_vector(seed: int4&) : auto

random float3 unit vector (length=1.)

:Arguments: * **seed** : int4&

.. _function-random_random_in_unit_sphere_int4:

.. das:function:: random_in_unit_sphere(seed: int4&) : auto

random float3 unit vector (length=1) which happens to be inside a sphere R=1

:Arguments: * **seed** : int4&

.. _function-random_random_in_unit_disk_int4:

.. das:function:: random_in_unit_disk(seed: int4&) : auto

random float3 unit vector (length=1) which happens to be inside a disk R=1, Z=0

:Arguments: * **seed** : int4&


