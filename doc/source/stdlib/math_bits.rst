
.. _stdlib_math_bits:

================
Math bit helpers
================

.. das:module:: math_bits

The MATH_BITS module provides bit manipulation functions for floating point
numbers, including type punning between integer and float representations,
and efficient integer math operations like ``int_bits_to_float`` and
``float_bits_to_int``.

All functions and symbols are in "math_bits" module, use require to get access to it. ::

    require daslib/math_bits

Example: ::

    require daslib/math_bits

        [export]
        def main() {
            let f = uint_bits_to_float(0x3F800000u)
            print("uint_bits_to_float(0x3F800000) = {f}\n")
            let back = float_bits_to_uint(1.0)
            print("float_bits_to_uint(1.0) = {back}\n")
        }
        // output:
        // uint_bits_to_float(0x3F800000) = 1
        // float_bits_to_uint(1.0) = 0x3f800000



+++++++++++++++++
float in int,uint
+++++++++++++++++

  *  :ref:`int_bits_to_float (x: int2) : float2 <function-math_bits_int_bits_to_float_int2>`
  *  :ref:`int_bits_to_float (x: int) : float <function-math_bits_int_bits_to_float_int>`
  *  :ref:`int_bits_to_float (x: int4) : float4 <function-math_bits_int_bits_to_float_int4>`
  *  :ref:`int_bits_to_float (x: int3) : float3 <function-math_bits_int_bits_to_float_int3>`
  *  :ref:`uint_bits_to_float (x: uint3) : float3 <function-math_bits_uint_bits_to_float_uint3>`
  *  :ref:`uint_bits_to_float (x: uint2) : float2 <function-math_bits_uint_bits_to_float_uint2>`
  *  :ref:`uint_bits_to_float (x: uint) : float <function-math_bits_uint_bits_to_float_uint>`
  *  :ref:`uint_bits_to_float (x: uint4) : float4 <function-math_bits_uint_bits_to_float_uint4>`


int_bits_to_float
^^^^^^^^^^^^^^^^^

.. _function-math_bits_int_bits_to_float_int2:

.. das:function:: int_bits_to_float(x: int2) : float2

bit representation of x is interpreted as a float


:Arguments: * **x** : int2

.. _function-math_bits_int_bits_to_float_int:

.. das:function:: int_bits_to_float(x: int) : float

.. _function-math_bits_int_bits_to_float_int4:

.. das:function:: int_bits_to_float(x: int4) : float4

.. _function-math_bits_int_bits_to_float_int3:

.. das:function:: int_bits_to_float(x: int3) : float3

----


uint_bits_to_float
^^^^^^^^^^^^^^^^^^

.. _function-math_bits_uint_bits_to_float_uint3:

.. das:function:: uint_bits_to_float(x: uint3) : float3

bit representation of x is interpreted as a float3


:Arguments: * **x** : uint3

.. _function-math_bits_uint_bits_to_float_uint2:

.. das:function:: uint_bits_to_float(x: uint2) : float2

.. _function-math_bits_uint_bits_to_float_uint:

.. das:function:: uint_bits_to_float(x: uint) : float

.. _function-math_bits_uint_bits_to_float_uint4:

.. das:function:: uint_bits_to_float(x: uint4) : float4


+++++++++++++++++
int,uint in float
+++++++++++++++++

  *  :ref:`float_bits_to_int (x: float2) : int2 <function-math_bits_float_bits_to_int_float2>`
  *  :ref:`float_bits_to_int (x: float) : int <function-math_bits_float_bits_to_int_float>`
  *  :ref:`float_bits_to_int (x: float4) : int4 <function-math_bits_float_bits_to_int_float4>`
  *  :ref:`float_bits_to_int (x: float3) : int3 <function-math_bits_float_bits_to_int_float3>`
  *  :ref:`float_bits_to_uint (x: float3) : uint3 <function-math_bits_float_bits_to_uint_float3>`
  *  :ref:`float_bits_to_uint (x: float2) : uint2 <function-math_bits_float_bits_to_uint_float2>`
  *  :ref:`float_bits_to_uint (x: float) : uint <function-math_bits_float_bits_to_uint_float>`
  *  :ref:`float_bits_to_uint (x: float4) : uint4 <function-math_bits_float_bits_to_uint_float4>`


float_bits_to_int
^^^^^^^^^^^^^^^^^

.. _function-math_bits_float_bits_to_int_float2:

.. das:function:: float_bits_to_int(x: float2) : int2

bit representation of x is interpreted as an int2


:Arguments: * **x** : float2

.. _function-math_bits_float_bits_to_int_float:

.. das:function:: float_bits_to_int(x: float) : int

.. _function-math_bits_float_bits_to_int_float4:

.. das:function:: float_bits_to_int(x: float4) : int4

.. _function-math_bits_float_bits_to_int_float3:

.. das:function:: float_bits_to_int(x: float3) : int3

----


float_bits_to_uint
^^^^^^^^^^^^^^^^^^

.. _function-math_bits_float_bits_to_uint_float3:

.. das:function:: float_bits_to_uint(x: float3) : uint3

bit representation of x is interpreted as a uint3


:Arguments: * **x** : float3

.. _function-math_bits_float_bits_to_uint_float2:

.. das:function:: float_bits_to_uint(x: float2) : uint2

.. _function-math_bits_float_bits_to_uint_float:

.. das:function:: float_bits_to_uint(x: float) : uint

.. _function-math_bits_float_bits_to_uint_float4:

.. das:function:: float_bits_to_uint(x: float4) : uint4


++++++++++++++++++++++
int64,uint64 in double
++++++++++++++++++++++

  *  :ref:`double_bits_to_int64 (x: double) : int64 <function-math_bits_double_bits_to_int64_double>`
  *  :ref:`double_bits_to_uint64 (x: double) : uint64 <function-math_bits_double_bits_to_uint64_double>`
  *  :ref:`int64_bits_to_double (x: int64) : double <function-math_bits_int64_bits_to_double_int64>`
  *  :ref:`uint64_bits_to_double (x: uint64) : double <function-math_bits_uint64_bits_to_double_uint64>`

.. _function-math_bits_double_bits_to_int64_double:

.. das:function:: double_bits_to_int64(x: double) : int64

bit representation of x is interpreted as a int64


:Arguments: * **x** : double

.. _function-math_bits_double_bits_to_uint64_double:

.. das:function:: double_bits_to_uint64(x: double) : uint64

bit representation of x is interpreted as a uint64


:Arguments: * **x** : double

.. _function-math_bits_int64_bits_to_double_int64:

.. das:function:: int64_bits_to_double(x: int64) : double

bit representation of x is interpreted as a double


:Arguments: * **x** : int64

.. _function-math_bits_uint64_bits_to_double_uint64:

.. das:function:: uint64_bits_to_double(x: uint64) : double

bit representation of x is interpreted as a double


:Arguments: * **x** : uint64


++++++++++++++
bit-cast vec4f
++++++++++++++

  *  :ref:`cast_to_int16 (data: float4) : int16 <function-math_bits_cast_to_int16_float4>`
  *  :ref:`cast_to_int32 (data: float4) : int <function-math_bits_cast_to_int32_float4>`
  *  :ref:`cast_to_int64 (data: float4) : int64 <function-math_bits_cast_to_int64_float4>`
  *  :ref:`cast_to_int8 (data: float4) : int8 <function-math_bits_cast_to_int8_float4>`
  *  :ref:`cast_to_pointer (data: float4) : void? <function-math_bits_cast_to_pointer_float4>`
  *  :ref:`cast_to_string (data: float4) : string <function-math_bits_cast_to_string_float4>`
  *  :ref:`cast_to_vec4f (x: int64) : float4 <function-math_bits_cast_to_vec4f_int64>`
  *  :ref:`cast_to_vec4f (x: bool) : float4 <function-math_bits_cast_to_vec4f_bool>`

.. _function-math_bits_cast_to_int16_float4:

.. das:function:: cast_to_int16(data: float4) : int16

return an int16 which was bit-cast from x


:Arguments: * **data** : float4

.. _function-math_bits_cast_to_int32_float4:

.. das:function:: cast_to_int32(data: float4) : int

return an int32 which was bit-cast from x


:Arguments: * **data** : float4

.. _function-math_bits_cast_to_int64_float4:

.. das:function:: cast_to_int64(data: float4) : int64

return an int64 which was bit-cast from x


:Arguments: * **data** : float4

.. _function-math_bits_cast_to_int8_float4:

.. das:function:: cast_to_int8(data: float4) : int8

return an int8 which was bit-cast from x


:Arguments: * **data** : float4

.. _function-math_bits_cast_to_pointer_float4:

.. das:function:: cast_to_pointer(data: float4) : void?

return a pointer which was bit-cast from x


:Arguments: * **data** : float4

.. _function-math_bits_cast_to_string_float4:

.. das:function:: cast_to_string(data: float4) : string

return a string which pointer was bit-cast from x


:Arguments: * **data** : float4


cast_to_vec4f
^^^^^^^^^^^^^

.. _function-math_bits_cast_to_vec4f_int64:

.. das:function:: cast_to_vec4f(x: int64) : float4

return a float4 which stores bit-cast version of x


:Arguments: * **x** : int64

.. _function-math_bits_cast_to_vec4f_bool:

.. das:function:: cast_to_vec4f(x: bool) : float4


