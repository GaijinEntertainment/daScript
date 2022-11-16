
.. _stdlib_math_bits:

================
Math bit helpers
================

.. include:: detail/math_bits.rst

This module represents collection of bit representation routines, which allow accessing integer and floating point values packed into different types.

All functions and symbols are in "math_bits" module, or publicly available via "math_boost". Use require to get access to it. ::

    require daslib/math_bits
    require daslib/math_boost



+++++++++++++++++
float in int,uint
+++++++++++++++++

  *  :ref:`int_bits_to_float (x:int const) : float <function-_at_math_bits_c__c_int_bits_to_float_Ci>` 
  *  :ref:`int_bits_to_float (x:int2 const) : float2 <function-_at_math_bits_c__c_int_bits_to_float_Ci2>` 
  *  :ref:`int_bits_to_float (x:int3 const) : float3 <function-_at_math_bits_c__c_int_bits_to_float_Ci3>` 
  *  :ref:`int_bits_to_float (x:int4 const) : float4 <function-_at_math_bits_c__c_int_bits_to_float_Ci4>` 
  *  :ref:`uint_bits_to_float (x:uint const) : float <function-_at_math_bits_c__c_uint_bits_to_float_Cu>` 
  *  :ref:`uint_bits_to_float (x:uint2 const) : float2 <function-_at_math_bits_c__c_uint_bits_to_float_Cu2>` 
  *  :ref:`uint_bits_to_float (x:uint3 const) : float3 <function-_at_math_bits_c__c_uint_bits_to_float_Cu3>` 
  *  :ref:`uint_bits_to_float (x:uint4 const) : float4 <function-_at_math_bits_c__c_uint_bits_to_float_Cu4>` 

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci:

.. das:function:: int_bits_to_float(x: int const)

int_bits_to_float returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int const    +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci2:

.. das:function:: int_bits_to_float(x: int2 const)

int_bits_to_float returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2 const   +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci3:

.. das:function:: int_bits_to_float(x: int3 const)

int_bits_to_float returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3 const   +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci4:

.. das:function:: int_bits_to_float(x: int4 const)

int_bits_to_float returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4 const   +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu:

.. das:function:: uint_bits_to_float(x: uint const)

uint_bits_to_float returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint const   +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu2:

.. das:function:: uint_bits_to_float(x: uint2 const)

uint_bits_to_float returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint2 const  +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu3:

.. das:function:: uint_bits_to_float(x: uint3 const)

uint_bits_to_float returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint3 const  +
+--------+-------------+


bit representation of x is interpreted as a float

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu4:

.. das:function:: uint_bits_to_float(x: uint4 const)

uint_bits_to_float returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint4 const  +
+--------+-------------+


bit representation of x is interpreted as a float

+++++++++++++++++
int,uint in float
+++++++++++++++++

  *  :ref:`float_bits_to_int (x:float const) : int <function-_at_math_bits_c__c_float_bits_to_int_Cf>` 
  *  :ref:`float_bits_to_int (x:float2 const) : int2 <function-_at_math_bits_c__c_float_bits_to_int_Cf2>` 
  *  :ref:`float_bits_to_int (x:float3 const) : int3 <function-_at_math_bits_c__c_float_bits_to_int_Cf3>` 
  *  :ref:`float_bits_to_int (x:float4 const) : int4 <function-_at_math_bits_c__c_float_bits_to_int_Cf4>` 
  *  :ref:`float_bits_to_uint (x:float const) : uint <function-_at_math_bits_c__c_float_bits_to_uint_Cf>` 
  *  :ref:`float_bits_to_uint (x:float2 const) : uint2 <function-_at_math_bits_c__c_float_bits_to_uint_Cf2>` 
  *  :ref:`float_bits_to_uint (x:float3 const) : uint3 <function-_at_math_bits_c__c_float_bits_to_uint_Cf3>` 
  *  :ref:`float_bits_to_uint (x:float4 const) : uint4 <function-_at_math_bits_c__c_float_bits_to_uint_Cf4>` 

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf:

.. das:function:: float_bits_to_int(x: float const)

float_bits_to_int returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


bit representation of x is interpreted as a int

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf2:

.. das:function:: float_bits_to_int(x: float2 const)

float_bits_to_int returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


bit representation of x is interpreted as a int

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf3:

.. das:function:: float_bits_to_int(x: float3 const)

float_bits_to_int returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


bit representation of x is interpreted as a int

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf4:

.. das:function:: float_bits_to_int(x: float4 const)

float_bits_to_int returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


bit representation of x is interpreted as a int

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf:

.. das:function:: float_bits_to_uint(x: float const)

float_bits_to_uint returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


bit representation of x is interpreted as a uint

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf2:

.. das:function:: float_bits_to_uint(x: float2 const)

float_bits_to_uint returns uint2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


bit representation of x is interpreted as a uint

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf3:

.. das:function:: float_bits_to_uint(x: float3 const)

float_bits_to_uint returns uint3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


bit representation of x is interpreted as a uint

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf4:

.. das:function:: float_bits_to_uint(x: float4 const)

float_bits_to_uint returns uint4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


bit representation of x is interpreted as a uint

++++++++++++++++++++++
int64,uint64 in double
++++++++++++++++++++++

  *  :ref:`int64_bits_to_double (x:int64 const) : double <function-_at_math_bits_c__c_int64_bits_to_double_Ci64>` 
  *  :ref:`uint64_bits_to_double (x:uint64 const) : double <function-_at_math_bits_c__c_uint64_bits_to_double_Cu64>` 
  *  :ref:`double_bits_to_int64 (x:double const) : int64 <function-_at_math_bits_c__c_double_bits_to_int64_Cd>` 
  *  :ref:`double_bits_to_uint64 (x:double const) : uint64 <function-_at_math_bits_c__c_double_bits_to_uint64_Cd>` 

.. _function-_at_math_bits_c__c_int64_bits_to_double_Ci64:

.. das:function:: int64_bits_to_double(x: int64 const)

int64_bits_to_double returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64 const  +
+--------+-------------+


bit representation of x is interpreted as a double

.. _function-_at_math_bits_c__c_uint64_bits_to_double_Cu64:

.. das:function:: uint64_bits_to_double(x: uint64 const)

uint64_bits_to_double returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64 const +
+--------+-------------+


bit representation of x is interpreted as a double

.. _function-_at_math_bits_c__c_double_bits_to_int64_Cd:

.. das:function:: double_bits_to_int64(x: double const)

double_bits_to_int64 returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


bit representation of x is interpreted as a int64

.. _function-_at_math_bits_c__c_double_bits_to_uint64_Cd:

.. das:function:: double_bits_to_uint64(x: double const)

double_bits_to_uint64 returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


bit representation of x is interpreted as a uint64


