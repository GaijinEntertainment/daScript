
.. _stdlib_math:

============
Math library
============

.. include:: detail/math.rst

Floating point math in general is not bit-precise. Compiler can optimize permutations, replace divisions with multiplications, and some of functions are not bit-exact.
If you need precise math use double precision type.
All functions and symbols are in "math" module, use require to get access to it. ::

    require math


+++++++++
Constants
+++++++++

.. _global-math-FLT_EPSILON:

.. das:attribute:: FLT_EPSILON = 1.19209e-07f

|variable-math-FLT_EPSILON|

.. _global-math-PI:

.. das:attribute:: PI = 3.14159f

|variable-math-PI|

+++++++++++++
Handled types
+++++++++++++

.. _handle-math-float3x4:

.. das:attribute:: float3x4

|any_annotation-math-float3x4|

.. _handle-math-float4x4:

.. das:attribute:: float4x4

|any_annotation-math-float4x4|

++++++++++++++++++++++++++++++++++++++++++
all numerics (uint*, int*, float*, double)
++++++++++++++++++++++++++++++++++++++++++

  *  :ref:`max (x:double;y:double) : double <function-_at_math_c__c_max_double_double>` 
  *  :ref:`max (x:float;y:float) : float <function-_at_math_c__c_max_float_float>` 
  *  :ref:`max (x:float2;y:float2) : float2 <function-_at_math_c__c_max_float2_float2>` 
  *  :ref:`max (x:float3;y:float3) : float3 <function-_at_math_c__c_max_float3_float3>` 
  *  :ref:`max (x:float4;y:float4) : float4 <function-_at_math_c__c_max_float4_float4>` 
  *  :ref:`max (x:int;y:int) : int <function-_at_math_c__c_max_int_int>` 
  *  :ref:`max (x:int2;y:int2) : int2 <function-_at_math_c__c_max_int2_int2>` 
  *  :ref:`max (x:int3;y:int3) : int3 <function-_at_math_c__c_max_int3_int3>` 
  *  :ref:`max (x:int4;y:int4) : int4 <function-_at_math_c__c_max_int4_int4>` 
  *  :ref:`max (x:int64;y:int64) : int64 <function-_at_math_c__c_max_int64_int64>` 
  *  :ref:`max (x:uint;y:uint) : uint <function-_at_math_c__c_max_uint_uint>` 
  *  :ref:`max (x:uint64;y:uint64) : uint64 <function-_at_math_c__c_max_uint64_uint64>` 
  *  :ref:`min (x:double;y:double) : double <function-_at_math_c__c_min_double_double>` 
  *  :ref:`min (x:float;y:float) : float <function-_at_math_c__c_min_float_float>` 
  *  :ref:`min (x:float2;y:float2) : float2 <function-_at_math_c__c_min_float2_float2>` 
  *  :ref:`min (x:float3;y:float3) : float3 <function-_at_math_c__c_min_float3_float3>` 
  *  :ref:`min (x:float4;y:float4) : float4 <function-_at_math_c__c_min_float4_float4>` 
  *  :ref:`min (x:int;y:int) : int <function-_at_math_c__c_min_int_int>` 
  *  :ref:`min (x:int2;y:int2) : int2 <function-_at_math_c__c_min_int2_int2>` 
  *  :ref:`min (x:int3;y:int3) : int3 <function-_at_math_c__c_min_int3_int3>` 
  *  :ref:`min (x:int4;y:int4) : int4 <function-_at_math_c__c_min_int4_int4>` 
  *  :ref:`min (x:int64;y:int64) : int64 <function-_at_math_c__c_min_int64_int64>` 
  *  :ref:`min (x:uint;y:uint) : uint <function-_at_math_c__c_min_uint_uint>` 
  *  :ref:`min (x:uint64;y:uint64) : uint64 <function-_at_math_c__c_min_uint64_uint64>` 

.. _function-_at_math_c__c_max_double_double:

.. das:function:: max(x: double; y: double)

max returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double       +
+--------+-------------+
+y       +double       +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_float_float:

.. das:function:: max(x: float; y: float)

max returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+
+y       +float        +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_float2_float2:

.. das:function:: max(x: float2; y: float2)

max returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+
+y       +float2       +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_float3_float3:

.. das:function:: max(x: float3; y: float3)

max returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+
+y       +float3       +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_float4_float4:

.. das:function:: max(x: float4; y: float4)

max returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+
+y       +float4       +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_int_int:

.. das:function:: max(x: int; y: int)

max returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int          +
+--------+-------------+
+y       +int          +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_int2_int2:

.. das:function:: max(x: int2; y: int2)

max returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2         +
+--------+-------------+
+y       +int2         +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_int3_int3:

.. das:function:: max(x: int3; y: int3)

max returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3         +
+--------+-------------+
+y       +int3         +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_int4_int4:

.. das:function:: max(x: int4; y: int4)

max returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4         +
+--------+-------------+
+y       +int4         +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_int64_int64:

.. das:function:: max(x: int64; y: int64)

max returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64        +
+--------+-------------+
+y       +int64        +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_uint_uint:

.. das:function:: max(x: uint; y: uint)

max returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint         +
+--------+-------------+
+y       +uint         +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max_uint64_uint64:

.. das:function:: max(x: uint64; y: uint64)

max returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64       +
+--------+-------------+
+y       +uint64       +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_min_double_double:

.. das:function:: min(x: double; y: double)

min returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double       +
+--------+-------------+
+y       +double       +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_float_float:

.. das:function:: min(x: float; y: float)

min returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+
+y       +float        +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_float2_float2:

.. das:function:: min(x: float2; y: float2)

min returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+
+y       +float2       +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_float3_float3:

.. das:function:: min(x: float3; y: float3)

min returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+
+y       +float3       +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_float4_float4:

.. das:function:: min(x: float4; y: float4)

min returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+
+y       +float4       +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_int_int:

.. das:function:: min(x: int; y: int)

min returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int          +
+--------+-------------+
+y       +int          +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_int2_int2:

.. das:function:: min(x: int2; y: int2)

min returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2         +
+--------+-------------+
+y       +int2         +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_int3_int3:

.. das:function:: min(x: int3; y: int3)

min returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3         +
+--------+-------------+
+y       +int3         +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_int4_int4:

.. das:function:: min(x: int4; y: int4)

min returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4         +
+--------+-------------+
+y       +int4         +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_int64_int64:

.. das:function:: min(x: int64; y: int64)

min returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64        +
+--------+-------------+
+y       +int64        +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_uint_uint:

.. das:function:: min(x: uint; y: uint)

min returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint         +
+--------+-------------+
+y       +uint         +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min_uint64_uint64:

.. das:function:: min(x: uint64; y: uint64)

min returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64       +
+--------+-------------+
+y       +uint64       +
+--------+-------------+


|function-math-min|

+++++++++++++++++
float* and double
+++++++++++++++++

  *  :ref:`abs (x:double const) : double <function-_at_math_c__c_abs_double_hh_const>` 
  *  :ref:`abs (x:float) : float <function-_at_math_c__c_abs_float>` 
  *  :ref:`abs (x:float2) : float2 <function-_at_math_c__c_abs_float2>` 
  *  :ref:`abs (x:float3) : float3 <function-_at_math_c__c_abs_float3>` 
  *  :ref:`abs (x:float4) : float4 <function-_at_math_c__c_abs_float4>` 
  *  :ref:`acos (x:double const) : double <function-_at_math_c__c_acos_double_hh_const>` 
  *  :ref:`acos (x:float) : float <function-_at_math_c__c_acos_float>` 
  *  :ref:`acos (x:float2) : float2 <function-_at_math_c__c_acos_float2>` 
  *  :ref:`acos (x:float3) : float3 <function-_at_math_c__c_acos_float3>` 
  *  :ref:`acos (x:float4) : float4 <function-_at_math_c__c_acos_float4>` 
  *  :ref:`asin (x:double const) : double <function-_at_math_c__c_asin_double_hh_const>` 
  *  :ref:`asin (x:float) : float <function-_at_math_c__c_asin_float>` 
  *  :ref:`asin (x:float2) : float2 <function-_at_math_c__c_asin_float2>` 
  *  :ref:`asin (x:float3) : float3 <function-_at_math_c__c_asin_float3>` 
  *  :ref:`asin (x:float4) : float4 <function-_at_math_c__c_asin_float4>` 
  *  :ref:`atan (x:double const) : double <function-_at_math_c__c_atan_double_hh_const>` 
  *  :ref:`atan (x:float) : float <function-_at_math_c__c_atan_float>` 
  *  :ref:`atan (x:float2) : float2 <function-_at_math_c__c_atan_float2>` 
  *  :ref:`atan (x:float3) : float3 <function-_at_math_c__c_atan_float3>` 
  *  :ref:`atan (x:float4) : float4 <function-_at_math_c__c_atan_float4>` 
  *  :ref:`atan2 (y:double const;x:double const) : double <function-_at_math_c__c_atan2_double_hh_const_double_hh_const>` 
  *  :ref:`atan2 (y:float;x:float) : float <function-_at_math_c__c_atan2_float_float>` 
  *  :ref:`atan2 (y:float2;x:float2) : float2 <function-_at_math_c__c_atan2_float2_float2>` 
  *  :ref:`atan2 (y:float3;x:float3) : float3 <function-_at_math_c__c_atan2_float3_float3>` 
  *  :ref:`atan2 (y:float4;x:float4) : float4 <function-_at_math_c__c_atan2_float4_float4>` 
  *  :ref:`ceil (x:float) : float <function-_at_math_c__c_ceil_float>` 
  *  :ref:`ceil (x:float2) : float2 <function-_at_math_c__c_ceil_float2>` 
  *  :ref:`ceil (x:float3) : float3 <function-_at_math_c__c_ceil_float3>` 
  *  :ref:`ceil (x:float4) : float4 <function-_at_math_c__c_ceil_float4>` 
  *  :ref:`cos (x:double const) : double <function-_at_math_c__c_cos_double_hh_const>` 
  *  :ref:`cos (x:float) : float <function-_at_math_c__c_cos_float>` 
  *  :ref:`cos (x:float2) : float2 <function-_at_math_c__c_cos_float2>` 
  *  :ref:`cos (x:float3) : float3 <function-_at_math_c__c_cos_float3>` 
  *  :ref:`cos (x:float4) : float4 <function-_at_math_c__c_cos_float4>` 
  *  :ref:`exp (x:double const) : double <function-_at_math_c__c_exp_double_hh_const>` 
  *  :ref:`exp (x:float) : float <function-_at_math_c__c_exp_float>` 
  *  :ref:`exp (x:float2) : float2 <function-_at_math_c__c_exp_float2>` 
  *  :ref:`exp (x:float3) : float3 <function-_at_math_c__c_exp_float3>` 
  *  :ref:`exp (x:float4) : float4 <function-_at_math_c__c_exp_float4>` 
  *  :ref:`exp2 (x:double const) : double <function-_at_math_c__c_exp2_double_hh_const>` 
  *  :ref:`exp2 (x:float) : float <function-_at_math_c__c_exp2_float>` 
  *  :ref:`exp2 (x:float2) : float2 <function-_at_math_c__c_exp2_float2>` 
  *  :ref:`exp2 (x:float3) : float3 <function-_at_math_c__c_exp2_float3>` 
  *  :ref:`exp2 (x:float4) : float4 <function-_at_math_c__c_exp2_float4>` 
  *  :ref:`floor (x:float) : float <function-_at_math_c__c_floor_float>` 
  *  :ref:`floor (x:float2) : float2 <function-_at_math_c__c_floor_float2>` 
  *  :ref:`floor (x:float3) : float3 <function-_at_math_c__c_floor_float3>` 
  *  :ref:`floor (x:float4) : float4 <function-_at_math_c__c_floor_float4>` 
  *  :ref:`log (x:double const) : double <function-_at_math_c__c_log_double_hh_const>` 
  *  :ref:`log (x:float) : float <function-_at_math_c__c_log_float>` 
  *  :ref:`log (x:float2) : float2 <function-_at_math_c__c_log_float2>` 
  *  :ref:`log (x:float3) : float3 <function-_at_math_c__c_log_float3>` 
  *  :ref:`log (x:float4) : float4 <function-_at_math_c__c_log_float4>` 
  *  :ref:`log2 (x:double const) : double <function-_at_math_c__c_log2_double_hh_const>` 
  *  :ref:`log2 (x:float) : float <function-_at_math_c__c_log2_float>` 
  *  :ref:`log2 (x:float2) : float2 <function-_at_math_c__c_log2_float2>` 
  *  :ref:`log2 (x:float3) : float3 <function-_at_math_c__c_log2_float3>` 
  *  :ref:`log2 (x:float4) : float4 <function-_at_math_c__c_log2_float4>` 
  *  :ref:`pow (x:double const;y:double const) : double <function-_at_math_c__c_pow_double_hh_const_double_hh_const>` 
  *  :ref:`pow (x:float;y:float) : float <function-_at_math_c__c_pow_float_float>` 
  *  :ref:`pow (x:float2;y:float2) : float2 <function-_at_math_c__c_pow_float2_float2>` 
  *  :ref:`pow (x:float3;y:float3) : float3 <function-_at_math_c__c_pow_float3_float3>` 
  *  :ref:`pow (x:float4;y:float4) : float4 <function-_at_math_c__c_pow_float4_float4>` 
  *  :ref:`rcp (x:double const) : double <function-_at_math_c__c_rcp_double_hh_const>` 
  *  :ref:`rcp (x:float) : float <function-_at_math_c__c_rcp_float>` 
  *  :ref:`rcp (x:float2) : float2 <function-_at_math_c__c_rcp_float2>` 
  *  :ref:`rcp (x:float3) : float3 <function-_at_math_c__c_rcp_float3>` 
  *  :ref:`rcp (x:float4) : float4 <function-_at_math_c__c_rcp_float4>` 
  *  :ref:`saturate (x:float) : float <function-_at_math_c__c_saturate_float>` 
  *  :ref:`saturate (x:float2) : float2 <function-_at_math_c__c_saturate_float2>` 
  *  :ref:`saturate (x:float3) : float3 <function-_at_math_c__c_saturate_float3>` 
  *  :ref:`saturate (x:float4) : float4 <function-_at_math_c__c_saturate_float4>` 
  *  :ref:`sin (x:double const) : double <function-_at_math_c__c_sin_double_hh_const>` 
  *  :ref:`sin (x:float) : float <function-_at_math_c__c_sin_float>` 
  *  :ref:`sin (x:float2) : float2 <function-_at_math_c__c_sin_float2>` 
  *  :ref:`sin (x:float3) : float3 <function-_at_math_c__c_sin_float3>` 
  *  :ref:`sin (x:float4) : float4 <function-_at_math_c__c_sin_float4>` 
  *  :ref:`sincos (x:double const;s:double& implicit;c:double& implicit) : void <function-_at_math_c__c_sincos_double_hh_const_double_hh_ref_hh_implicit_double_hh_ref_hh_implicit>` 
  *  :ref:`sincos (x:float const;s:float& implicit;c:float& implicit) : void <function-_at_math_c__c_sincos_float_hh_const_float_hh_ref_hh_implicit_float_hh_ref_hh_implicit>` 
  *  :ref:`sqrt (x:double const) : double <function-_at_math_c__c_sqrt_double_hh_const>` 
  *  :ref:`sqrt (x:float) : float <function-_at_math_c__c_sqrt_float>` 
  *  :ref:`sqrt (x:float2) : float2 <function-_at_math_c__c_sqrt_float2>` 
  *  :ref:`sqrt (x:float3) : float3 <function-_at_math_c__c_sqrt_float3>` 
  *  :ref:`sqrt (x:float4) : float4 <function-_at_math_c__c_sqrt_float4>` 
  *  :ref:`tan (x:double const) : double <function-_at_math_c__c_tan_double_hh_const>` 
  *  :ref:`tan (x:float) : float <function-_at_math_c__c_tan_float>` 
  *  :ref:`tan (x:float2) : float2 <function-_at_math_c__c_tan_float2>` 
  *  :ref:`tan (x:float3) : float3 <function-_at_math_c__c_tan_float3>` 
  *  :ref:`tan (x:float4) : float4 <function-_at_math_c__c_tan_float4>` 

.. _function-_at_math_c__c_abs_double_hh_const:

.. das:function:: abs(x: double const)

abs returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs_float:

.. das:function:: abs(x: float)

abs returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs_float2:

.. das:function:: abs(x: float2)

abs returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs_float3:

.. das:function:: abs(x: float3)

abs returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs_float4:

.. das:function:: abs(x: float4)

abs returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_acos_double_hh_const:

.. das:function:: acos(x: double const)

acos returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos_float:

.. das:function:: acos(x: float)

acos returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos_float2:

.. das:function:: acos(x: float2)

acos returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos_float3:

.. das:function:: acos(x: float3)

acos returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos_float4:

.. das:function:: acos(x: float4)

acos returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_asin_double_hh_const:

.. das:function:: asin(x: double const)

asin returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin_float:

.. das:function:: asin(x: float)

asin returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin_float2:

.. das:function:: asin(x: float2)

asin returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin_float3:

.. das:function:: asin(x: float3)

asin returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin_float4:

.. das:function:: asin(x: float4)

asin returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_atan_double_hh_const:

.. das:function:: atan(x: double const)

atan returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan_float:

.. das:function:: atan(x: float)

atan returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan_float2:

.. das:function:: atan(x: float2)

atan returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan_float3:

.. das:function:: atan(x: float3)

atan returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan_float4:

.. das:function:: atan(x: float4)

atan returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan2_double_hh_const_double_hh_const:

.. das:function:: atan2(y: double const; x: double const)

atan2 returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +double const +
+--------+-------------+
+x       +double const +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2_float_float:

.. das:function:: atan2(y: float; x: float)

atan2 returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float        +
+--------+-------------+
+x       +float        +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2_float2_float2:

.. das:function:: atan2(y: float2; x: float2)

atan2 returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float2       +
+--------+-------------+
+x       +float2       +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2_float3_float3:

.. das:function:: atan2(y: float3; x: float3)

atan2 returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float3       +
+--------+-------------+
+x       +float3       +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2_float4_float4:

.. das:function:: atan2(y: float4; x: float4)

atan2 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float4       +
+--------+-------------+
+x       +float4       +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_ceil_float:

.. das:function:: ceil(x: float)

ceil returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_ceil_float2:

.. das:function:: ceil(x: float2)

ceil returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_ceil_float3:

.. das:function:: ceil(x: float3)

ceil returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_ceil_float4:

.. das:function:: ceil(x: float4)

ceil returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_cos_double_hh_const:

.. das:function:: cos(x: double const)

cos returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos_float:

.. das:function:: cos(x: float)

cos returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos_float2:

.. das:function:: cos(x: float2)

cos returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos_float3:

.. das:function:: cos(x: float3)

cos returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos_float4:

.. das:function:: cos(x: float4)

cos returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_exp_double_hh_const:

.. das:function:: exp(x: double const)

exp returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp_float:

.. das:function:: exp(x: float)

exp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp_float2:

.. das:function:: exp(x: float2)

exp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp_float3:

.. das:function:: exp(x: float3)

exp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp_float4:

.. das:function:: exp(x: float4)

exp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp2_double_hh_const:

.. das:function:: exp2(x: double const)

exp2 returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2_float:

.. das:function:: exp2(x: float)

exp2 returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2_float2:

.. das:function:: exp2(x: float2)

exp2 returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2_float3:

.. das:function:: exp2(x: float3)

exp2 returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2_float4:

.. das:function:: exp2(x: float4)

exp2 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_floor_float:

.. das:function:: floor(x: float)

floor returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_floor_float2:

.. das:function:: floor(x: float2)

floor returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_floor_float3:

.. das:function:: floor(x: float3)

floor returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_floor_float4:

.. das:function:: floor(x: float4)

floor returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_log_double_hh_const:

.. das:function:: log(x: double const)

log returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log_float:

.. das:function:: log(x: float)

log returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log_float2:

.. das:function:: log(x: float2)

log returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log_float3:

.. das:function:: log(x: float3)

log returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log_float4:

.. das:function:: log(x: float4)

log returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log2_double_hh_const:

.. das:function:: log2(x: double const)

log2 returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2_float:

.. das:function:: log2(x: float)

log2 returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2_float2:

.. das:function:: log2(x: float2)

log2 returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2_float3:

.. das:function:: log2(x: float3)

log2 returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2_float4:

.. das:function:: log2(x: float4)

log2 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_pow_double_hh_const_double_hh_const:

.. das:function:: pow(x: double const; y: double const)

pow returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+
+y       +double const +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow_float_float:

.. das:function:: pow(x: float; y: float)

pow returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+
+y       +float        +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow_float2_float2:

.. das:function:: pow(x: float2; y: float2)

pow returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+
+y       +float2       +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow_float3_float3:

.. das:function:: pow(x: float3; y: float3)

pow returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+
+y       +float3       +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow_float4_float4:

.. das:function:: pow(x: float4; y: float4)

pow returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+
+y       +float4       +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_rcp_double_hh_const:

.. das:function:: rcp(x: double const)

rcp returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp_float:

.. das:function:: rcp(x: float)

rcp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp_float2:

.. das:function:: rcp(x: float2)

rcp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp_float3:

.. das:function:: rcp(x: float3)

rcp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp_float4:

.. das:function:: rcp(x: float4)

rcp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_saturate_float:

.. das:function:: saturate(x: float)

saturate returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_saturate_float2:

.. das:function:: saturate(x: float2)

saturate returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_saturate_float3:

.. das:function:: saturate(x: float3)

saturate returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_saturate_float4:

.. das:function:: saturate(x: float4)

saturate returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_sin_double_hh_const:

.. das:function:: sin(x: double const)

sin returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin_float:

.. das:function:: sin(x: float)

sin returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin_float2:

.. das:function:: sin(x: float2)

sin returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin_float3:

.. das:function:: sin(x: float3)

sin returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin_float4:

.. das:function:: sin(x: float4)

sin returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sincos_double_hh_const_double_hh_ref_hh_implicit_double_hh_ref_hh_implicit:

.. das:function:: sincos(x: double const; s: double& implicit; c: double& implicit)

+--------+----------------+
+argument+argument type   +
+========+================+
+x       +double const    +
+--------+----------------+
+s       +double& implicit+
+--------+----------------+
+c       +double& implicit+
+--------+----------------+


|function-math-sincos|

.. _function-_at_math_c__c_sincos_float_hh_const_float_hh_ref_hh_implicit_float_hh_ref_hh_implicit:

.. das:function:: sincos(x: float const; s: float& implicit; c: float& implicit)

+--------+---------------+
+argument+argument type  +
+========+===============+
+x       +float const    +
+--------+---------------+
+s       +float& implicit+
+--------+---------------+
+c       +float& implicit+
+--------+---------------+


|function-math-sincos|

.. _function-_at_math_c__c_sqrt_double_hh_const:

.. das:function:: sqrt(x: double const)

sqrt returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt_float:

.. das:function:: sqrt(x: float)

sqrt returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt_float2:

.. das:function:: sqrt(x: float2)

sqrt returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt_float3:

.. das:function:: sqrt(x: float3)

sqrt returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt_float4:

.. das:function:: sqrt(x: float4)

sqrt returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_tan_double_hh_const:

.. das:function:: tan(x: double const)

tan returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan_float:

.. das:function:: tan(x: float)

tan returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan_float2:

.. das:function:: tan(x: float2)

tan returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan_float3:

.. das:function:: tan(x: float3)

tan returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan_float4:

.. das:function:: tan(x: float4)

tan returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-tan|

+++++++++++
float* only
+++++++++++

  *  :ref:`atan2_est (y:float;x:float) : float <function-_at_math_c__c_atan2_est_float_float>` 
  *  :ref:`atan2_est (y:float2;x:float2) : float2 <function-_at_math_c__c_atan2_est_float2_float2>` 
  *  :ref:`atan2_est (y:float3;x:float3) : float3 <function-_at_math_c__c_atan2_est_float3_float3>` 
  *  :ref:`atan2_est (y:float4;x:float4) : float4 <function-_at_math_c__c_atan2_est_float4_float4>` 
  *  :ref:`ceili (x:float) : int <function-_at_math_c__c_ceili_float>` 
  *  :ref:`ceili (x:float2) : int2 <function-_at_math_c__c_ceili_float2>` 
  *  :ref:`ceili (x:float3) : int3 <function-_at_math_c__c_ceili_float3>` 
  *  :ref:`ceili (x:float4) : int4 <function-_at_math_c__c_ceili_float4>` 
  *  :ref:`floori (x:float) : int <function-_at_math_c__c_floori_float>` 
  *  :ref:`floori (x:float2) : int2 <function-_at_math_c__c_floori_float2>` 
  *  :ref:`floori (x:float3) : int3 <function-_at_math_c__c_floori_float3>` 
  *  :ref:`floori (x:float4) : int4 <function-_at_math_c__c_floori_float4>` 
  *  :ref:`rcp_est (x:float) : float <function-_at_math_c__c_rcp_est_float>` 
  *  :ref:`rcp_est (x:float2) : float2 <function-_at_math_c__c_rcp_est_float2>` 
  *  :ref:`rcp_est (x:float3) : float3 <function-_at_math_c__c_rcp_est_float3>` 
  *  :ref:`rcp_est (x:float4) : float4 <function-_at_math_c__c_rcp_est_float4>` 
  *  :ref:`roundi (x:float) : int <function-_at_math_c__c_roundi_float>` 
  *  :ref:`roundi (x:float2) : int2 <function-_at_math_c__c_roundi_float2>` 
  *  :ref:`roundi (x:float3) : int3 <function-_at_math_c__c_roundi_float3>` 
  *  :ref:`roundi (x:float4) : int4 <function-_at_math_c__c_roundi_float4>` 
  *  :ref:`rsqrt (x:float) : float <function-_at_math_c__c_rsqrt_float>` 
  *  :ref:`rsqrt (x:float2) : float2 <function-_at_math_c__c_rsqrt_float2>` 
  *  :ref:`rsqrt (x:float3) : float3 <function-_at_math_c__c_rsqrt_float3>` 
  *  :ref:`rsqrt (x:float4) : float4 <function-_at_math_c__c_rsqrt_float4>` 
  *  :ref:`rsqrt_est (x:float) : float <function-_at_math_c__c_rsqrt_est_float>` 
  *  :ref:`rsqrt_est (x:float2) : float2 <function-_at_math_c__c_rsqrt_est_float2>` 
  *  :ref:`rsqrt_est (x:float3) : float3 <function-_at_math_c__c_rsqrt_est_float3>` 
  *  :ref:`rsqrt_est (x:float4) : float4 <function-_at_math_c__c_rsqrt_est_float4>` 
  *  :ref:`trunci (x:float) : int <function-_at_math_c__c_trunci_float>` 
  *  :ref:`trunci (x:float2) : int2 <function-_at_math_c__c_trunci_float2>` 
  *  :ref:`trunci (x:float3) : int3 <function-_at_math_c__c_trunci_float3>` 
  *  :ref:`trunci (x:float4) : int4 <function-_at_math_c__c_trunci_float4>` 

.. _function-_at_math_c__c_atan2_est_float_float:

.. das:function:: atan2_est(y: float; x: float)

atan2_est returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float        +
+--------+-------------+
+x       +float        +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_atan2_est_float2_float2:

.. das:function:: atan2_est(y: float2; x: float2)

atan2_est returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float2       +
+--------+-------------+
+x       +float2       +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_atan2_est_float3_float3:

.. das:function:: atan2_est(y: float3; x: float3)

atan2_est returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float3       +
+--------+-------------+
+x       +float3       +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_atan2_est_float4_float4:

.. das:function:: atan2_est(y: float4; x: float4)

atan2_est returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float4       +
+--------+-------------+
+x       +float4       +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_ceili_float:

.. das:function:: ceili(x: float)

ceili returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_ceili_float2:

.. das:function:: ceili(x: float2)

ceili returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_ceili_float3:

.. das:function:: ceili(x: float3)

ceili returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_ceili_float4:

.. das:function:: ceili(x: float4)

ceili returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_floori_float:

.. das:function:: floori(x: float)

floori returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_floori_float2:

.. das:function:: floori(x: float2)

floori returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_floori_float3:

.. das:function:: floori(x: float3)

floori returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_floori_float4:

.. das:function:: floori(x: float4)

floori returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_rcp_est_float:

.. das:function:: rcp_est(x: float)

rcp_est returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_rcp_est_float2:

.. das:function:: rcp_est(x: float2)

rcp_est returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_rcp_est_float3:

.. das:function:: rcp_est(x: float3)

rcp_est returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_rcp_est_float4:

.. das:function:: rcp_est(x: float4)

rcp_est returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_roundi_float:

.. das:function:: roundi(x: float)

roundi returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_roundi_float2:

.. das:function:: roundi(x: float2)

roundi returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_roundi_float3:

.. das:function:: roundi(x: float3)

roundi returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_roundi_float4:

.. das:function:: roundi(x: float4)

roundi returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_rsqrt_float:

.. das:function:: rsqrt(x: float)

rsqrt returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt_float2:

.. das:function:: rsqrt(x: float2)

rsqrt returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt_float3:

.. das:function:: rsqrt(x: float3)

rsqrt returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt_float4:

.. das:function:: rsqrt(x: float4)

rsqrt returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt_est_float:

.. das:function:: rsqrt_est(x: float)

rsqrt_est returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_rsqrt_est_float2:

.. das:function:: rsqrt_est(x: float2)

rsqrt_est returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_rsqrt_est_float3:

.. das:function:: rsqrt_est(x: float3)

rsqrt_est returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_rsqrt_est_float4:

.. das:function:: rsqrt_est(x: float4)

rsqrt_est returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_trunci_float:

.. das:function:: trunci(x: float)

trunci returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float        +
+--------+-------------+


|function-math-trunci|

.. _function-_at_math_c__c_trunci_float2:

.. das:function:: trunci(x: float2)

trunci returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2       +
+--------+-------------+


|function-math-trunci|

.. _function-_at_math_c__c_trunci_float3:

.. das:function:: trunci(x: float3)

trunci returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3       +
+--------+-------------+


|function-math-trunci|

.. _function-_at_math_c__c_trunci_float4:

.. das:function:: trunci(x: float4)

trunci returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4       +
+--------+-------------+


|function-math-trunci|

+++++++++++
float3 only
+++++++++++

  *  :ref:`cross (x:float3 const;y:float3 const) : float3 <function-_at_math_c__c_cross_float3_hh_const_float3_hh_const>` 
  *  :ref:`distance (x:float3 const;y:float3 const) : float <function-_at_math_c__c_distance_float3_hh_const_float3_hh_const>` 
  *  :ref:`distance_sq (x:float3 const;y:float3 const) : float <function-_at_math_c__c_distance_sq_float3_hh_const_float3_hh_const>` 
  *  :ref:`inv_distance (x:float3 const;y:float3 const) : float <function-_at_math_c__c_inv_distance_float3_hh_const_float3_hh_const>` 
  *  :ref:`inv_distance_sq (x:float3 const;y:float3 const) : float <function-_at_math_c__c_inv_distance_sq_float3_hh_const_float3_hh_const>` 
  *  :ref:`reflect (v:float3 const;n:float3 const) : float3 <function-_at_math_c__c_reflect_float3_hh_const_float3_hh_const>` 
  *  :ref:`refract (v:float3 const;n:float3 const;nint:float const;outRefracted:float3&) : bool <function-_at_math_c__c_refract_float3_hh_const_float3_hh_const_float_hh_const_float3_hh_ref>` 

.. _function-_at_math_c__c_cross_float3_hh_const_float3_hh_const:

.. das:function:: cross(x: float3 const; y: float3 const)

cross returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-cross|

.. _function-_at_math_c__c_distance_float3_hh_const_float3_hh_const:

.. das:function:: distance(x: float3 const; y: float3 const)

distance returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-distance|

.. _function-_at_math_c__c_distance_sq_float3_hh_const_float3_hh_const:

.. das:function:: distance_sq(x: float3 const; y: float3 const)

distance_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-distance_sq|

.. _function-_at_math_c__c_inv_distance_float3_hh_const_float3_hh_const:

.. das:function:: inv_distance(x: float3 const; y: float3 const)

inv_distance returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-inv_distance|

.. _function-_at_math_c__c_inv_distance_sq_float3_hh_const_float3_hh_const:

.. das:function:: inv_distance_sq(x: float3 const; y: float3 const)

inv_distance_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-inv_distance_sq|

.. _function-_at_math_c__c_reflect_float3_hh_const_float3_hh_const:

.. das:function:: reflect(v: float3 const; n: float3 const)

reflect returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +float3 const +
+--------+-------------+
+n       +float3 const +
+--------+-------------+


returns vector representing reflection of vector v from normal n same as ::

    def reflect(v, n: float3)
        return v - 2. * dot(v, n) * n

.. _function-_at_math_c__c_refract_float3_hh_const_float3_hh_const_float_hh_const_float3_hh_ref:

.. das:function:: refract(v: float3 const; n: float3 const; nint: float const; outRefracted: float3&)

refract returns bool

+------------+-------------+
+argument    +argument type+
+============+=============+
+v           +float3 const +
+------------+-------------+
+n           +float3 const +
+------------+-------------+
+nint        +float const  +
+------------+-------------+
+outRefracted+float3&      +
+------------+-------------+


returns vector representing refractoin of vector v from normal n same as ::

    def refract(v, n: float3; nint: float; outRefracted: float3&)
        let dt = dot(v, n)
        let discr = 1. - nint * nint * (1. - dt * dt)
        if discr > 0.
            outRefracted = nint * (v - n * dt) - n * sqrt(discr)
            return true
        return false

++++++++++++++++++++++
float2, float3, float4
++++++++++++++++++++++

  *  :ref:`dot (x:float2 const;y:float2 const) : float <function-_at_math_c__c_dot_float2_hh_const_float2_hh_const>` 
  *  :ref:`dot (x:float3 const;y:float3 const) : float <function-_at_math_c__c_dot_float3_hh_const_float3_hh_const>` 
  *  :ref:`dot (x:float4 const;y:float4 const) : float <function-_at_math_c__c_dot_float4_hh_const_float4_hh_const>` 
  *  :ref:`fast_normalize (x:float2 const) : float2 <function-_at_math_c__c_fast_normalize_float2_hh_const>` 
  *  :ref:`fast_normalize (x:float3 const) : float3 <function-_at_math_c__c_fast_normalize_float3_hh_const>` 
  *  :ref:`fast_normalize (x:float4 const) : float4 <function-_at_math_c__c_fast_normalize_float4_hh_const>` 
  *  :ref:`inv_length (x:float2 const) : float <function-_at_math_c__c_inv_length_float2_hh_const>` 
  *  :ref:`inv_length (x:float3 const) : float <function-_at_math_c__c_inv_length_float3_hh_const>` 
  *  :ref:`inv_length (x:float4 const) : float <function-_at_math_c__c_inv_length_float4_hh_const>` 
  *  :ref:`inv_length_sq (x:float2 const) : float <function-_at_math_c__c_inv_length_sq_float2_hh_const>` 
  *  :ref:`inv_length_sq (x:float3 const) : float <function-_at_math_c__c_inv_length_sq_float3_hh_const>` 
  *  :ref:`inv_length_sq (x:float4 const) : float <function-_at_math_c__c_inv_length_sq_float4_hh_const>` 
  *  :ref:`length (x:float2 const) : float <function-_at_math_c__c_length_float2_hh_const>` 
  *  :ref:`length (x:float3 const) : float <function-_at_math_c__c_length_float3_hh_const>` 
  *  :ref:`length (x:float4 const) : float <function-_at_math_c__c_length_float4_hh_const>` 
  *  :ref:`length_sq (x:float2 const) : float <function-_at_math_c__c_length_sq_float2_hh_const>` 
  *  :ref:`length_sq (x:float3 const) : float <function-_at_math_c__c_length_sq_float3_hh_const>` 
  *  :ref:`length_sq (x:float4 const) : float <function-_at_math_c__c_length_sq_float4_hh_const>` 
  *  :ref:`normalize (x:float2 const) : float2 <function-_at_math_c__c_normalize_float2_hh_const>` 
  *  :ref:`normalize (x:float3 const) : float3 <function-_at_math_c__c_normalize_float3_hh_const>` 
  *  :ref:`normalize (x:float4 const) : float4 <function-_at_math_c__c_normalize_float4_hh_const>` 

.. _function-_at_math_c__c_dot_float2_hh_const_float2_hh_const:

.. das:function:: dot(x: float2 const; y: float2 const)

dot returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+
+y       +float2 const +
+--------+-------------+


|function-math-dot|

.. _function-_at_math_c__c_dot_float3_hh_const_float3_hh_const:

.. das:function:: dot(x: float3 const; y: float3 const)

dot returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-dot|

.. _function-_at_math_c__c_dot_float4_hh_const_float4_hh_const:

.. das:function:: dot(x: float4 const; y: float4 const)

dot returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+
+y       +float4 const +
+--------+-------------+


|function-math-dot|

.. _function-_at_math_c__c_fast_normalize_float2_hh_const:

.. das:function:: fast_normalize(x: float2 const)

fast_normalize returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-fast_normalize|

.. _function-_at_math_c__c_fast_normalize_float3_hh_const:

.. das:function:: fast_normalize(x: float3 const)

fast_normalize returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-fast_normalize|

.. _function-_at_math_c__c_fast_normalize_float4_hh_const:

.. das:function:: fast_normalize(x: float4 const)

fast_normalize returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-fast_normalize|

.. _function-_at_math_c__c_inv_length_float2_hh_const:

.. das:function:: inv_length(x: float2 const)

inv_length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-inv_length|

.. _function-_at_math_c__c_inv_length_float3_hh_const:

.. das:function:: inv_length(x: float3 const)

inv_length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-inv_length|

.. _function-_at_math_c__c_inv_length_float4_hh_const:

.. das:function:: inv_length(x: float4 const)

inv_length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-inv_length|

.. _function-_at_math_c__c_inv_length_sq_float2_hh_const:

.. das:function:: inv_length_sq(x: float2 const)

inv_length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-inv_length_sq|

.. _function-_at_math_c__c_inv_length_sq_float3_hh_const:

.. das:function:: inv_length_sq(x: float3 const)

inv_length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-inv_length_sq|

.. _function-_at_math_c__c_inv_length_sq_float4_hh_const:

.. das:function:: inv_length_sq(x: float4 const)

inv_length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-inv_length_sq|

.. _function-_at_math_c__c_length_float2_hh_const:

.. das:function:: length(x: float2 const)

length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-length|

.. _function-_at_math_c__c_length_float3_hh_const:

.. das:function:: length(x: float3 const)

length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-length|

.. _function-_at_math_c__c_length_float4_hh_const:

.. das:function:: length(x: float4 const)

length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-length|

.. _function-_at_math_c__c_length_sq_float2_hh_const:

.. das:function:: length_sq(x: float2 const)

length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-length_sq|

.. _function-_at_math_c__c_length_sq_float3_hh_const:

.. das:function:: length_sq(x: float3 const)

length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-length_sq|

.. _function-_at_math_c__c_length_sq_float4_hh_const:

.. das:function:: length_sq(x: float4 const)

length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-length_sq|

.. _function-_at_math_c__c_normalize_float2_hh_const:

.. das:function:: normalize(x: float2 const)

normalize returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-normalize|

.. _function-_at_math_c__c_normalize_float3_hh_const:

.. das:function:: normalize(x: float3 const)

normalize returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-normalize|

.. _function-_at_math_c__c_normalize_float4_hh_const:

.. das:function:: normalize(x: float4 const)

normalize returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-normalize|

+++++++++++++++
Noise functions
+++++++++++++++

  *  :ref:`uint32_hash (seed:uint const) : uint <function-_at_math_c__c_uint32_hash_uint_hh_const>` 
  *  :ref:`uint_noise_1D (position:int const;seed:uint const) : uint <function-_at_math_c__c_uint_noise_1D_int_hh_const_uint_hh_const>` 
  *  :ref:`uint_noise_2D (position:int2 const;seed:uint const) : uint <function-_at_math_c__c_uint_noise_2D_int2_hh_const_uint_hh_const>` 
  *  :ref:`uint_noise_3D (position:int3 const;seed:uint const) : uint <function-_at_math_c__c_uint_noise_3D_int3_hh_const_uint_hh_const>` 

.. _function-_at_math_c__c_uint32_hash_uint_hh_const:

.. das:function:: uint32_hash(seed: uint const)

uint32_hash returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +uint const   +
+--------+-------------+


|function-math-uint32_hash|

.. _function-_at_math_c__c_uint_noise_1D_int_hh_const_uint_hh_const:

.. das:function:: uint_noise_1D(position: int const; seed: uint const)

uint_noise_1D returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+position+int const    +
+--------+-------------+
+seed    +uint const   +
+--------+-------------+


|function-math-uint_noise_1D|

.. _function-_at_math_c__c_uint_noise_2D_int2_hh_const_uint_hh_const:

.. das:function:: uint_noise_2D(position: int2 const; seed: uint const)

uint_noise_2D returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+position+int2 const   +
+--------+-------------+
+seed    +uint const   +
+--------+-------------+


|function-math-uint_noise_2D|

.. _function-_at_math_c__c_uint_noise_3D_int3_hh_const_uint_hh_const:

.. das:function:: uint_noise_3D(position: int3 const; seed: uint const)

uint_noise_3D returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+position+int3 const   +
+--------+-------------+
+seed    +uint const   +
+--------+-------------+


|function-math-uint_noise_3D|

++++++++++++++
lerp/mad/clamp
++++++++++++++

  *  :ref:`clamp (t:float;a:float;b:float) : float <function-_at_math_c__c_clamp_float_float_float>` 
  *  :ref:`clamp (t:float2;a:float2;b:float2) : float2 <function-_at_math_c__c_clamp_float2_float2_float2>` 
  *  :ref:`clamp (t:float3;a:float3;b:float3) : float3 <function-_at_math_c__c_clamp_float3_float3_float3>` 
  *  :ref:`clamp (t:float4;a:float4;b:float4) : float4 <function-_at_math_c__c_clamp_float4_float4_float4>` 
  *  :ref:`lerp (a:float;b:float;t:float) : float <function-_at_math_c__c_lerp_float_float_float>` 
  *  :ref:`lerp (a:float2;b:float2;t:float2) : float2 <function-_at_math_c__c_lerp_float2_float2_float2>` 
  *  :ref:`lerp (a:float3;b:float3;t:float3) : float3 <function-_at_math_c__c_lerp_float3_float3_float3>` 
  *  :ref:`lerp (a:float4;b:float4;t:float4) : float4 <function-_at_math_c__c_lerp_float4_float4_float4>` 
  *  :ref:`mad (a:float;b:float;c:float) : float <function-_at_math_c__c_mad_float_float_float>` 
  *  :ref:`mad (a:float2;b:float;c:float2) : float2 <function-_at_math_c__c_mad_float2_float_float2>` 
  *  :ref:`mad (a:float2;b:float2;c:float2) : float2 <function-_at_math_c__c_mad_float2_float2_float2>` 
  *  :ref:`mad (a:float3;b:float;c:float3) : float3 <function-_at_math_c__c_mad_float3_float_float3>` 
  *  :ref:`mad (a:float3;b:float3;c:float3) : float3 <function-_at_math_c__c_mad_float3_float3_float3>` 
  *  :ref:`mad (a:float4;b:float;c:float4) : float4 <function-_at_math_c__c_mad_float4_float_float4>` 
  *  :ref:`mad (a:float4;b:float4;c:float4) : float4 <function-_at_math_c__c_mad_float4_float4_float4>` 

.. _function-_at_math_c__c_clamp_float_float_float:

.. das:function:: clamp(t: float; a: float; b: float)

clamp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float        +
+--------+-------------+
+a       +float        +
+--------+-------------+
+b       +float        +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp_float2_float2_float2:

.. das:function:: clamp(t: float2; a: float2; b: float2)

clamp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float2       +
+--------+-------------+
+a       +float2       +
+--------+-------------+
+b       +float2       +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp_float3_float3_float3:

.. das:function:: clamp(t: float3; a: float3; b: float3)

clamp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float3       +
+--------+-------------+
+a       +float3       +
+--------+-------------+
+b       +float3       +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp_float4_float4_float4:

.. das:function:: clamp(t: float4; a: float4; b: float4)

clamp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float4       +
+--------+-------------+
+a       +float4       +
+--------+-------------+
+b       +float4       +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_lerp_float_float_float:

.. das:function:: lerp(a: float; b: float; t: float)

lerp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float        +
+--------+-------------+
+b       +float        +
+--------+-------------+
+t       +float        +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_lerp_float2_float2_float2:

.. das:function:: lerp(a: float2; b: float2; t: float2)

lerp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float2       +
+--------+-------------+
+b       +float2       +
+--------+-------------+
+t       +float2       +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_lerp_float3_float3_float3:

.. das:function:: lerp(a: float3; b: float3; t: float3)

lerp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float3       +
+--------+-------------+
+b       +float3       +
+--------+-------------+
+t       +float3       +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_lerp_float4_float4_float4:

.. das:function:: lerp(a: float4; b: float4; t: float4)

lerp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float4       +
+--------+-------------+
+b       +float4       +
+--------+-------------+
+t       +float4       +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_mad_float_float_float:

.. das:function:: mad(a: float; b: float; c: float)

mad returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float        +
+--------+-------------+
+b       +float        +
+--------+-------------+
+c       +float        +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad_float2_float_float2:

.. das:function:: mad(a: float2; b: float; c: float2)

mad returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float2       +
+--------+-------------+
+b       +float        +
+--------+-------------+
+c       +float2       +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad_float2_float2_float2:

.. das:function:: mad(a: float2; b: float2; c: float2)

mad returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float2       +
+--------+-------------+
+b       +float2       +
+--------+-------------+
+c       +float2       +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad_float3_float_float3:

.. das:function:: mad(a: float3; b: float; c: float3)

mad returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float3       +
+--------+-------------+
+b       +float        +
+--------+-------------+
+c       +float3       +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad_float3_float3_float3:

.. das:function:: mad(a: float3; b: float3; c: float3)

mad returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float3       +
+--------+-------------+
+b       +float3       +
+--------+-------------+
+c       +float3       +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad_float4_float_float4:

.. das:function:: mad(a: float4; b: float; c: float4)

mad returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float4       +
+--------+-------------+
+b       +float        +
+--------+-------------+
+c       +float4       +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad_float4_float4_float4:

.. das:function:: mad(a: float4; b: float4; c: float4)

mad returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float4       +
+--------+-------------+
+b       +float4       +
+--------+-------------+
+c       +float4       +
+--------+-------------+


|function-math-mad|

+++++++++++++++++
Matrix operations
+++++++++++++++++

  *  :ref:`\!= (x:math::float4x4 const implicit;y:math::float4x4 const implicit) : bool <function-_at_math_c__c__ex__eq___hh_handle_hh_float4x4_hh_const_hh_implicit__hh_handle_hh_float4x4_hh_const_hh_implicit>` 
  *  :ref:`* (x:math::float3x4 const implicit;y:math::float3x4 const implicit) : math::float3x4 <function-_at_math_c__c_*__hh_handle_hh_float3x4_hh_const_hh_implicit__hh_handle_hh_float3x4_hh_const_hh_implicit>` 
  *  :ref:`* (x:math::float3x4 const implicit;y:float3 const) : float3 <function-_at_math_c__c_*__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const>` 
  *  :ref:`* (x:math::float4x4 const implicit;y:math::float4x4 const implicit) : math::float4x4 <function-_at_math_c__c_*__hh_handle_hh_float4x4_hh_const_hh_implicit__hh_handle_hh_float4x4_hh_const_hh_implicit>` 
  *  :ref:`== (x:math::float4x4 const implicit;y:math::float4x4 const implicit) : bool <function-_at_math_c__c__eq__eq___hh_handle_hh_float4x4_hh_const_hh_implicit__hh_handle_hh_float4x4_hh_const_hh_implicit>` 

.. _function-_at_math_c__c__ex__eq___hh_handle_hh_float4x4_hh_const_hh_implicit__hh_handle_hh_float4x4_hh_const_hh_implicit:

.. das:function:: operator !=(x: float4x4 const implicit; y: float4x4 const implicit)

!= returns bool

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-!=|

.. _function-_at_math_c__c_*__hh_handle_hh_float3x4_hh_const_hh_implicit__hh_handle_hh_float3x4_hh_const_hh_implicit:

.. das:function:: operator *(x: float3x4 const implicit; y: float3x4 const implicit)

* returns  :ref:`math::float3x4 <handle-math-float3x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       + :ref:`math::float3x4 <handle-math-float3x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-*|

.. _function-_at_math_c__c_*__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const:

.. das:function:: operator *(x: float3x4 const implicit; y: float3 const)

* returns float3

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       +float3 const                                                 +
+--------+-------------------------------------------------------------+


|function-math-*|

.. _function-_at_math_c__c_*__hh_handle_hh_float4x4_hh_const_hh_implicit__hh_handle_hh_float4x4_hh_const_hh_implicit:

.. das:function:: operator *(x: float4x4 const implicit; y: float4x4 const implicit)

* returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-*|

.. _function-_at_math_c__c__eq__eq___hh_handle_hh_float4x4_hh_const_hh_implicit__hh_handle_hh_float4x4_hh_const_hh_implicit:

.. das:function:: operator ==(x: float4x4 const implicit; y: float4x4 const implicit)

== returns bool

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-==|

+++++++++++++++++++
Matrix constructors
+++++++++++++++++++

  *  :ref:`float3x4 () : math::float3x4 <function-_at_math_c__c_float3x4>` 
  *  :ref:`float4x4 () : math::float4x4 <function-_at_math_c__c_float4x4>` 

.. _function-_at_math_c__c_float3x4:

.. das:function:: float3x4()

float3x4 returns  :ref:`math::float3x4 <handle-math-float3x4>` 

|function-math-float3x4|

.. _function-_at_math_c__c_float4x4:

.. das:function:: float4x4()

float4x4 returns  :ref:`math::float4x4 <handle-math-float4x4>` 

|function-math-float4x4|

+++++++++++++++++++
Matrix manipulation
+++++++++++++++++++

  *  :ref:`identity (x:math::float3x4 implicit) : void <function-_at_math_c__c_identity__hh_handle_hh_float3x4_hh_implicit>` 
  *  :ref:`identity (x:math::float4x4 implicit) : void <function-_at_math_c__c_identity__hh_handle_hh_float4x4_hh_implicit>` 
  *  :ref:`inverse (x:math::float3x4 const implicit) : math::float3x4 <function-_at_math_c__c_inverse__hh_handle_hh_float3x4_hh_const_hh_implicit>` 
  *  :ref:`rotate (x:math::float3x4 const implicit;y:float3 const) : float3 <function-_at_math_c__c_rotate__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const>` 
  *  :ref:`translation (xyz:float3 const) : math::float4x4 <function-_at_math_c__c_translation_float3_hh_const>` 
  *  :ref:`transpose (x:math::float4x4 const implicit) : math::float4x4 <function-_at_math_c__c_transpose__hh_handle_hh_float4x4_hh_const_hh_implicit>` 

.. _function-_at_math_c__c_identity__hh_handle_hh_float3x4_hh_implicit:

.. das:function:: identity(x: float3x4 implicit)

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  implicit+
+--------+-------------------------------------------------------+


|function-math-identity|

.. _function-_at_math_c__c_identity__hh_handle_hh_float4x4_hh_implicit:

.. das:function:: identity(x: float4x4 implicit)

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  implicit+
+--------+-------------------------------------------------------+


|function-math-identity|

.. _function-_at_math_c__c_inverse__hh_handle_hh_float3x4_hh_const_hh_implicit:

.. das:function:: inverse(x: float3x4 const implicit)

inverse returns  :ref:`math::float3x4 <handle-math-float3x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-inverse|

.. _function-_at_math_c__c_rotate__hh_handle_hh_float3x4_hh_const_hh_implicit_float3_hh_const:

.. das:function:: rotate(x: float3x4 const implicit; y: float3 const)

rotate returns float3

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       +float3 const                                                 +
+--------+-------------------------------------------------------------+


|function-math-rotate|

.. _function-_at_math_c__c_translation_float3_hh_const:

.. das:function:: translation(xyz: float3 const)

translation returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+xyz     +float3 const +
+--------+-------------+


produces a translation by xyz

 +---+---+---+---+
 + 1 | 0 | 0 | 0 |
 +---+---+---+---+
 + 0 | 1 | 0 | 0 |
 +---+---+---+---+
 + 0 | 0 | 1 | 0 |
 +---+---+---+---+
 + x | y | z | 1 |
 +---+---+---+---+


.. _function-_at_math_c__c_transpose__hh_handle_hh_float4x4_hh_const_hh_implicit:

.. das:function:: transpose(x: float4x4 const implicit)

transpose returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-transpose|


