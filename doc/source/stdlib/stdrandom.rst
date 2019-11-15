.. _stdlib_stdrandom:

========================
The LCG Random library
========================

the random library implements basic random routines.

++++++++++++++
Unit functions
++++++++++++++

.. das:function:: int4 random_seed(seed:int32)

    constructs seed vector out of single integer seed
	
.. das:function:: int random_int(seed:int4)

    random integer 0..32767
	
.. das:function:: int4 random_int4(seed:int4)

    random int4, each component is 0..32767
	
.. das:function:: float random_float(seed:int4)

    random float 0..1
	
.. das:function:: float4 random_float4(seed:int4)

    random float4, each component is 0..1
	
++++++++++++++++++++++
Distribution functions
++++++++++++++++++++++

.. das:function:: float3 random_unit_vector(seed:int4)

    random float3 unit vector (length=1)
	
.. das:function:: float3 random_in_unit_sphere(seed:int4)

    random float3 unit vector (length=1) which happens to be inside a sphere R=1
	
.. das:function:: float3 random_in_unit_disk(seed:int4)

    random float3 unit vector (length=1) which happens to be inside a disk R=1, Z=0
