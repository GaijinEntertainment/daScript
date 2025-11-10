
.. _stdlib_math_bits:

================
Math bit helpers
================

This module represents collection of bit representation routines, which allow accessing integer and floating point values packed into different types.

All functions and symbols are in "math_bits" module, or publicly available via "math_boost". Use require to get access to it. ::

    require daslib/math_bits
    require daslib/math_boost

+++++++++++++++++
float in int,uint
+++++++++++++++++

  *  :ref:`int_bits_to_float (x: int) : float <function-math_bits_int_bits_to_float_int>` 
  *  :ref:`int_bits_to_float (x: int2) : float2 <function-math_bits_int_bits_to_float_int2>` 
  *  :ref:`int_bits_to_float (x: int3) : float3 <function-math_bits_int_bits_to_float_int3>` 
  *  :ref:`int_bits_to_float (x: int4) : float4 <function-math_bits_int_bits_to_float_int4>` 
  *  :ref:`uint_bits_to_float (x: uint) : float <function-math_bits_uint_bits_to_float_uint>` 
  *  :ref:`uint_bits_to_float (x: uint2) : float2 <function-math_bits_uint_bits_to_float_uint2>` 
  *  :ref:`uint_bits_to_float (x: uint3) : float3 <function-math_bits_uint_bits_to_float_uint3>` 
  *  :ref:`uint_bits_to_float (x: uint4) : float4 <function-math_bits_uint_bits_to_float_uint4>` 

.. _function-math_bits_int_bits_to_float_int:

.. das:function:: int_bits_to_float(x: int) : float

bit representation of x is interpreted as a float

:Arguments: * **x** : int

.. _function-math_bits_int_bits_to_float_int2:

.. das:function:: int_bits_to_float(x: int2) : float2

bit representation of x is interpreted as a float

:Arguments: * **x** : int2

.. _function-math_bits_int_bits_to_float_int3:

.. das:function:: int_bits_to_float(x: int3) : float3

bit representation of x is interpreted as a float

:Arguments: * **x** : int3

.. _function-math_bits_int_bits_to_float_int4:

.. das:function:: int_bits_to_float(x: int4) : float4

bit representation of x is interpreted as a float

:Arguments: * **x** : int4

.. _function-math_bits_uint_bits_to_float_uint:

.. das:function:: uint_bits_to_float(x: uint) : float

bit representation of x is interpreted as a float

:Arguments: * **x** : uint

.. _function-math_bits_uint_bits_to_float_uint2:

.. das:function:: uint_bits_to_float(x: uint2) : float2

bit representation of x is interpreted as a float2

:Arguments: * **x** : uint2

.. _function-math_bits_uint_bits_to_float_uint3:

.. das:function:: uint_bits_to_float(x: uint3) : float3

bit representation of x is interpreted as a float3

:Arguments: * **x** : uint3

.. _function-math_bits_uint_bits_to_float_uint4:

.. das:function:: uint_bits_to_float(x: uint4) : float4

bit representation of x is interpreted as a float4

:Arguments: * **x** : uint4

+++++++++++++++++
int,uint in float
+++++++++++++++++

  *  :ref:`float_bits_to_int (x: float) : int <function-math_bits_float_bits_to_int_float>` 
  *  :ref:`float_bits_to_int (x: float2) : int2 <function-math_bits_float_bits_to_int_float2>` 
  *  :ref:`float_bits_to_int (x: float3) : int3 <function-math_bits_float_bits_to_int_float3>` 
  *  :ref:`float_bits_to_int (x: float4) : int4 <function-math_bits_float_bits_to_int_float4>` 
  *  :ref:`float_bits_to_uint (x: float) : uint <function-math_bits_float_bits_to_uint_float>` 
  *  :ref:`float_bits_to_uint (x: float2) : uint2 <function-math_bits_float_bits_to_uint_float2>` 
  *  :ref:`float_bits_to_uint (x: float3) : uint3 <function-math_bits_float_bits_to_uint_float3>` 
  *  :ref:`float_bits_to_uint (x: float4) : uint4 <function-math_bits_float_bits_to_uint_float4>` 

.. _function-math_bits_float_bits_to_int_float:

.. das:function:: float_bits_to_int(x: float) : int

bit representation of x is interpreted as a int

:Arguments: * **x** : float

.. _function-math_bits_float_bits_to_int_float2:

.. das:function:: float_bits_to_int(x: float2) : int2

bit representation of x is interpreted as a int2

:Arguments: * **x** : float2

.. _function-math_bits_float_bits_to_int_float3:

.. das:function:: float_bits_to_int(x: float3) : int3

bit representation of x is interpreted as a int3

:Arguments: * **x** : float3

.. _function-math_bits_float_bits_to_int_float4:

.. das:function:: float_bits_to_int(x: float4) : int4

bit representation of x is interpreted as a int4

:Arguments: * **x** : float4

.. _function-math_bits_float_bits_to_uint_float:

.. das:function:: float_bits_to_uint(x: float) : uint

bit representation of x is interpreted as a uint

:Arguments: * **x** : float

.. _function-math_bits_float_bits_to_uint_float2:

.. das:function:: float_bits_to_uint(x: float2) : uint2

bit representation of x is interpreted as a uint2

:Arguments: * **x** : float2

.. _function-math_bits_float_bits_to_uint_float3:

.. das:function:: float_bits_to_uint(x: float3) : uint3

bit representation of x is interpreted as a uint3

:Arguments: * **x** : float3

.. _function-math_bits_float_bits_to_uint_float4:

.. das:function:: float_bits_to_uint(x: float4) : uint4

bit representation of x is interpreted as a uint4

:Arguments: * **x** : float4

++++++++++++++++++++++
int64,uint64 in double
++++++++++++++++++++++

  *  :ref:`int64_bits_to_double (x: int64) : double <function-math_bits_int64_bits_to_double_int64>` 
  *  :ref:`uint64_bits_to_double (x: uint64) : double <function-math_bits_uint64_bits_to_double_uint64>` 
  *  :ref:`double_bits_to_int64 (x: double) : int64 <function-math_bits_double_bits_to_int64_double>` 
  *  :ref:`double_bits_to_uint64 (x: double) : uint64 <function-math_bits_double_bits_to_uint64_double>` 

.. _function-math_bits_int64_bits_to_double_int64:

.. das:function:: int64_bits_to_double(x: int64) : double

bit representation of x is interpreted as a double

:Arguments: * **x** : int64

.. _function-math_bits_uint64_bits_to_double_uint64:

.. das:function:: uint64_bits_to_double(x: uint64) : double

bit representation of x is interpreted as a double

:Arguments: * **x** : uint64

.. _function-math_bits_double_bits_to_int64_double:

.. das:function:: double_bits_to_int64(x: double) : int64

bit representation of x is interpreted as a int64

:Arguments: * **x** : double

.. _function-math_bits_double_bits_to_uint64_double:

.. das:function:: double_bits_to_uint64(x: double) : uint64

bit representation of x is interpreted as a uint64

:Arguments: * **x** : double

++++++++++++++
bit-cast vec4f
++++++++++++++

  *  :ref:`cast_to_vec4f (x: bool) : float4 <function-math_bits_cast_to_vec4f_bool>` 
  *  :ref:`cast_to_vec4f (x: int64) : float4 <function-math_bits_cast_to_vec4f_int64>` 
  *  :ref:`cast_to_int64 (data: float4) : int64 <function-math_bits_cast_to_int64_float4>` 
  *  :ref:`cast_to_int32 (data: float4) : int <function-math_bits_cast_to_int32_float4>` 
  *  :ref:`cast_to_int16 (data: float4) : int16 <function-math_bits_cast_to_int16_float4>` 
  *  :ref:`cast_to_int8 (data: float4) : int8 <function-math_bits_cast_to_int8_float4>` 
  *  :ref:`cast_to_string (data: float4) : string <function-math_bits_cast_to_string_float4>` 
  *  :ref:`cast_to_pointer (data: float4) : void? <function-math_bits_cast_to_pointer_float4>` 

.. _function-math_bits_cast_to_vec4f_bool:

.. das:function:: cast_to_vec4f(x: bool) : float4

return a float4 which stores bit-cast version of x

:Arguments: * **x** : bool

.. _function-math_bits_cast_to_vec4f_int64:

.. das:function:: cast_to_vec4f(x: int64) : float4

return a float4 which stores bit-cast version of x

:Arguments: * **x** : int64

.. _function-math_bits_cast_to_int64_float4:

.. das:function:: cast_to_int64(data: float4) : int64

return an int64 which was bit-cast from x

:Arguments: * **data** : float4

.. _function-math_bits_cast_to_int32_float4:

.. das:function:: cast_to_int32(data: float4) : int

return an int32 which was bit-cast from x

:Arguments: * **data** : float4

.. _function-math_bits_cast_to_int16_float4:

.. das:function:: cast_to_int16(data: float4) : int16

return an int16 which was bit-cast from x

:Arguments: * **data** : float4

.. _function-math_bits_cast_to_int8_float4:

.. das:function:: cast_to_int8(data: float4) : int8

return an int8 which was bit-cast from x

:Arguments: * **data** : float4

.. _function-math_bits_cast_to_string_float4:

.. das:function:: cast_to_string(data: float4) : string

return a string which pointer was bit-cast from x

:Arguments: * **data** : float4

.. _function-math_bits_cast_to_pointer_float4:

.. das:function:: cast_to_pointer(data: float4) : void?

return a pointer which was bit-cast from x

:Arguments: * **data** : float4


