
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

  *  :ref:`int_bits_to_float (x: int) : float <function-_at_math_bits_c__c_int_bits_to_float_Ci>` 
  *  :ref:`int_bits_to_float (x: int2) : float2 <function-_at_math_bits_c__c_int_bits_to_float_Ci2>` 
  *  :ref:`int_bits_to_float (x: int3) : float3 <function-_at_math_bits_c__c_int_bits_to_float_Ci3>` 
  *  :ref:`int_bits_to_float (x: int4) : float4 <function-_at_math_bits_c__c_int_bits_to_float_Ci4>` 
  *  :ref:`uint_bits_to_float (x: uint) : float <function-_at_math_bits_c__c_uint_bits_to_float_Cu>` 
  *  :ref:`uint_bits_to_float (x: uint2) : float2 <function-_at_math_bits_c__c_uint_bits_to_float_Cu2>` 
  *  :ref:`uint_bits_to_float (x: uint3) : float3 <function-_at_math_bits_c__c_uint_bits_to_float_Cu3>` 
  *  :ref:`uint_bits_to_float (x: uint4) : float4 <function-_at_math_bits_c__c_uint_bits_to_float_Cu4>` 

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci:

.. das:function:: int_bits_to_float(x: int) : float

bit representation of x is interpreted as a float

:Arguments: * **x** : int

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci2:

.. das:function:: int_bits_to_float(x: int2) : float2

|detail/function-math_bits-int_bits_to_float-0xaafc59fcc1694d59|

:Arguments: * **x** : int2

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci3:

.. das:function:: int_bits_to_float(x: int3) : float3

|detail/function-math_bits-int_bits_to_float-0xaafc58fcc1694ba6|

:Arguments: * **x** : int3

.. _function-_at_math_bits_c__c_int_bits_to_float_Ci4:

.. das:function:: int_bits_to_float(x: int4) : float4

|detail/function-math_bits-int_bits_to_float-0xaafc5bfcc16950bf|

:Arguments: * **x** : int4

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu:

.. das:function:: uint_bits_to_float(x: uint) : float

bit representation of x is interpreted as a float

:Arguments: * **x** : uint

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu2:

.. das:function:: uint_bits_to_float(x: uint2) : float2

|detail/function-math_bits-uint_bits_to_float-0xd1a4ac25c694a828|

:Arguments: * **x** : uint2

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu3:

.. das:function:: uint_bits_to_float(x: uint3) : float3

|detail/function-math_bits-uint_bits_to_float-0xd1a4ad25c694a9db|

:Arguments: * **x** : uint3

.. _function-_at_math_bits_c__c_uint_bits_to_float_Cu4:

.. das:function:: uint_bits_to_float(x: uint4) : float4

|detail/function-math_bits-uint_bits_to_float-0xd1a4b225c694b25a|

:Arguments: * **x** : uint4

+++++++++++++++++
int,uint in float
+++++++++++++++++

  *  :ref:`float_bits_to_int (x: float) : int <function-_at_math_bits_c__c_float_bits_to_int_Cf>` 
  *  :ref:`float_bits_to_int (x: float2) : int2 <function-_at_math_bits_c__c_float_bits_to_int_Cf2>` 
  *  :ref:`float_bits_to_int (x: float3) : int3 <function-_at_math_bits_c__c_float_bits_to_int_Cf3>` 
  *  :ref:`float_bits_to_int (x: float4) : int4 <function-_at_math_bits_c__c_float_bits_to_int_Cf4>` 
  *  :ref:`float_bits_to_uint (x: float) : uint <function-_at_math_bits_c__c_float_bits_to_uint_Cf>` 
  *  :ref:`float_bits_to_uint (x: float2) : uint2 <function-_at_math_bits_c__c_float_bits_to_uint_Cf2>` 
  *  :ref:`float_bits_to_uint (x: float3) : uint3 <function-_at_math_bits_c__c_float_bits_to_uint_Cf3>` 
  *  :ref:`float_bits_to_uint (x: float4) : uint4 <function-_at_math_bits_c__c_float_bits_to_uint_Cf4>` 

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf:

.. das:function:: float_bits_to_int(x: float) : int

bit representation of x is interpreted as a int

:Arguments: * **x** : float

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf2:

.. das:function:: float_bits_to_int(x: float2) : int2

|detail/function-math_bits-float_bits_to_int-0x10b7acb821f31afe|

:Arguments: * **x** : float2

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf3:

.. das:function:: float_bits_to_int(x: float3) : int3

|detail/function-math_bits-float_bits_to_int-0x10b7adb821f31cb1|

:Arguments: * **x** : float3

.. _function-_at_math_bits_c__c_float_bits_to_int_Cf4:

.. das:function:: float_bits_to_int(x: float4) : int4

|detail/function-math_bits-float_bits_to_int-0x10b7aeb821f31e64|

:Arguments: * **x** : float4

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf:

.. das:function:: float_bits_to_uint(x: float) : uint

bit representation of x is interpreted as a uint

:Arguments: * **x** : float

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf2:

.. das:function:: float_bits_to_uint(x: float2) : uint2

|detail/function-math_bits-float_bits_to_uint-0x43434831d466ee1e|

:Arguments: * **x** : float2

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf3:

.. das:function:: float_bits_to_uint(x: float3) : uint3

|detail/function-math_bits-float_bits_to_uint-0x43444831d468a11e|

:Arguments: * **x** : float3

.. _function-_at_math_bits_c__c_float_bits_to_uint_Cf4:

.. das:function:: float_bits_to_uint(x: float4) : uint4

|detail/function-math_bits-float_bits_to_uint-0x433d4831d45cbc1e|

:Arguments: * **x** : float4

++++++++++++++++++++++
int64,uint64 in double
++++++++++++++++++++++

  *  :ref:`int64_bits_to_double (x: int64) : double <function-_at_math_bits_c__c_int64_bits_to_double_Ci64>` 
  *  :ref:`uint64_bits_to_double (x: uint64) : double <function-_at_math_bits_c__c_uint64_bits_to_double_Cu64>` 
  *  :ref:`double_bits_to_int64 (x: double) : int64 <function-_at_math_bits_c__c_double_bits_to_int64_Cd>` 
  *  :ref:`double_bits_to_uint64 (x: double) : uint64 <function-_at_math_bits_c__c_double_bits_to_uint64_Cd>` 

.. _function-_at_math_bits_c__c_int64_bits_to_double_Ci64:

.. das:function:: int64_bits_to_double(x: int64) : double

bit representation of x is interpreted as a double

:Arguments: * **x** : int64

.. _function-_at_math_bits_c__c_uint64_bits_to_double_Cu64:

.. das:function:: uint64_bits_to_double(x: uint64) : double

bit representation of x is interpreted as a double

:Arguments: * **x** : uint64

.. _function-_at_math_bits_c__c_double_bits_to_int64_Cd:

.. das:function:: double_bits_to_int64(x: double) : int64

bit representation of x is interpreted as a int64

:Arguments: * **x** : double

.. _function-_at_math_bits_c__c_double_bits_to_uint64_Cd:

.. das:function:: double_bits_to_uint64(x: double) : uint64

bit representation of x is interpreted as a uint64

:Arguments: * **x** : double

++++++++++++++
bit-cast vec4f
++++++++++++++

  *  :ref:`cast_to_vec4f (x: bool) : float4 <function-_at_math_bits_c__c_cast_to_vec4f_Cb>` 
  *  :ref:`cast_to_vec4f (x: int64) : float4 <function-_at_math_bits_c__c_cast_to_vec4f_Ci64>` 
  *  :ref:`cast_to_int64 (data: float4) : int64 <function-_at_math_bits_c__c_cast_to_int64_Cf4>` 
  *  :ref:`cast_to_int32 (data: float4) : int <function-_at_math_bits_c__c_cast_to_int32_Cf4>` 
  *  :ref:`cast_to_int16 (data: float4) : int16 <function-_at_math_bits_c__c_cast_to_int16_Cf4>` 
  *  :ref:`cast_to_int8 (data: float4) : int8 <function-_at_math_bits_c__c_cast_to_int8_Cf4>` 
  *  :ref:`cast_to_string (data: float4) : string <function-_at_math_bits_c__c_cast_to_string_Cf4>` 
  *  :ref:`cast_to_pointer (data: float4) : void? <function-_at_math_bits_c__c_cast_to_pointer_Cf4>` 

.. _function-_at_math_bits_c__c_cast_to_vec4f_Cb:

.. das:function:: cast_to_vec4f(x: bool) : float4

:Arguments: * **x** : bool


return a float4 which stores bit-cast version of x

.. _function-_at_math_bits_c__c_cast_to_vec4f_Ci64:

.. das:function:: cast_to_vec4f(x: int64) : float4

:Arguments: * **x** : int64


return a float4 which stores bit-cast version of x

.. _function-_at_math_bits_c__c_cast_to_int64_Cf4:

.. das:function:: cast_to_int64(data: float4) : int64

:Arguments: * **data** : float4


return an int64 which was bit-cast from x

.. _function-_at_math_bits_c__c_cast_to_int32_Cf4:

.. das:function:: cast_to_int32(data: float4) : int

:Arguments: * **data** : float4


return an int32 which was bit-cast from x

.. _function-_at_math_bits_c__c_cast_to_int16_Cf4:

.. das:function:: cast_to_int16(data: float4) : int16

:Arguments: * **data** : float4


return an int16 which was bit-cast from x

.. _function-_at_math_bits_c__c_cast_to_int8_Cf4:

.. das:function:: cast_to_int8(data: float4) : int8

:Arguments: * **data** : float4


return an int8 which was bit-cast from x

.. _function-_at_math_bits_c__c_cast_to_string_Cf4:

.. das:function:: cast_to_string(data: float4) : string

:Arguments: * **data** : float4


return a string which pointer was bit-cast from x

.. _function-_at_math_bits_c__c_cast_to_pointer_Cf4:

.. das:function:: cast_to_pointer(data: float4) : void?

:Arguments: * **data** : float4


return a pointer which was bit-cast from x


