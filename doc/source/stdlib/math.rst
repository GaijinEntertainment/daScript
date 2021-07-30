
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

  *  :ref:`max (x:double const;y:double const) : double <function-_at_math_c__c_max__hh_const_hh_double__hh_const_hh_double>` 
  *  :ref:`max (x:float const;y:float const) : float <function-_at_math_c__c_max__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`max (x:float2 const;y:float2 const) : float2 <function-_at_math_c__c_max__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`max (x:float3 const;y:float3 const) : float3 <function-_at_math_c__c_max__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`max (x:float4 const;y:float4 const) : float4 <function-_at_math_c__c_max__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`max (x:int const;y:int const) : int <function-_at_math_c__c_max__hh_const_hh_int__hh_const_hh_int>` 
  *  :ref:`max (x:int2 const;y:int2 const) : int2 <function-_at_math_c__c_max__hh_const_hh_int2__hh_const_hh_int2>` 
  *  :ref:`max (x:int3 const;y:int3 const) : int3 <function-_at_math_c__c_max__hh_const_hh_int3__hh_const_hh_int3>` 
  *  :ref:`max (x:int4 const;y:int4 const) : int4 <function-_at_math_c__c_max__hh_const_hh_int4__hh_const_hh_int4>` 
  *  :ref:`max (x:int64 const;y:int64 const) : int64 <function-_at_math_c__c_max__hh_const_hh_int64__hh_const_hh_int64>` 
  *  :ref:`max (x:uint const;y:uint const) : uint <function-_at_math_c__c_max__hh_const_hh_uint__hh_const_hh_uint>` 
  *  :ref:`max (x:uint64 const;y:uint64 const) : uint64 <function-_at_math_c__c_max__hh_const_hh_uint64__hh_const_hh_uint64>` 
  *  :ref:`min (x:double const;y:double const) : double <function-_at_math_c__c_min__hh_const_hh_double__hh_const_hh_double>` 
  *  :ref:`min (x:float const;y:float const) : float <function-_at_math_c__c_min__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`min (x:float2 const;y:float2 const) : float2 <function-_at_math_c__c_min__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`min (x:float3 const;y:float3 const) : float3 <function-_at_math_c__c_min__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`min (x:float4 const;y:float4 const) : float4 <function-_at_math_c__c_min__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`min (x:int const;y:int const) : int <function-_at_math_c__c_min__hh_const_hh_int__hh_const_hh_int>` 
  *  :ref:`min (x:int2 const;y:int2 const) : int2 <function-_at_math_c__c_min__hh_const_hh_int2__hh_const_hh_int2>` 
  *  :ref:`min (x:int3 const;y:int3 const) : int3 <function-_at_math_c__c_min__hh_const_hh_int3__hh_const_hh_int3>` 
  *  :ref:`min (x:int4 const;y:int4 const) : int4 <function-_at_math_c__c_min__hh_const_hh_int4__hh_const_hh_int4>` 
  *  :ref:`min (x:int64 const;y:int64 const) : int64 <function-_at_math_c__c_min__hh_const_hh_int64__hh_const_hh_int64>` 
  *  :ref:`min (x:uint const;y:uint const) : uint <function-_at_math_c__c_min__hh_const_hh_uint__hh_const_hh_uint>` 
  *  :ref:`min (x:uint64 const;y:uint64 const) : uint64 <function-_at_math_c__c_min__hh_const_hh_uint64__hh_const_hh_uint64>` 

.. _function-_at_math_c__c_max__hh_const_hh_double__hh_const_hh_double:

.. das:function:: max(x: double const; y: double const)

max returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+
+y       +double const +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_float__hh_const_hh_float:

.. das:function:: max(x: float const; y: float const)

max returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+
+y       +float const  +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: max(x: float2 const; y: float2 const)

max returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+
+y       +float2 const +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: max(x: float3 const; y: float3 const)

max returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: max(x: float4 const; y: float4 const)

max returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+
+y       +float4 const +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_int__hh_const_hh_int:

.. das:function:: max(x: int const; y: int const)

max returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int const    +
+--------+-------------+
+y       +int const    +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_int2__hh_const_hh_int2:

.. das:function:: max(x: int2 const; y: int2 const)

max returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2 const   +
+--------+-------------+
+y       +int2 const   +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_int3__hh_const_hh_int3:

.. das:function:: max(x: int3 const; y: int3 const)

max returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3 const   +
+--------+-------------+
+y       +int3 const   +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_int4__hh_const_hh_int4:

.. das:function:: max(x: int4 const; y: int4 const)

max returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4 const   +
+--------+-------------+
+y       +int4 const   +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_int64__hh_const_hh_int64:

.. das:function:: max(x: int64 const; y: int64 const)

max returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64 const  +
+--------+-------------+
+y       +int64 const  +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_uint__hh_const_hh_uint:

.. das:function:: max(x: uint const; y: uint const)

max returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint const   +
+--------+-------------+
+y       +uint const   +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_max__hh_const_hh_uint64__hh_const_hh_uint64:

.. das:function:: max(x: uint64 const; y: uint64 const)

max returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64 const +
+--------+-------------+
+y       +uint64 const +
+--------+-------------+


|function-math-max|

.. _function-_at_math_c__c_min__hh_const_hh_double__hh_const_hh_double:

.. das:function:: min(x: double const; y: double const)

min returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+
+y       +double const +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_float__hh_const_hh_float:

.. das:function:: min(x: float const; y: float const)

min returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+
+y       +float const  +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: min(x: float2 const; y: float2 const)

min returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+
+y       +float2 const +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: min(x: float3 const; y: float3 const)

min returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: min(x: float4 const; y: float4 const)

min returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+
+y       +float4 const +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_int__hh_const_hh_int:

.. das:function:: min(x: int const; y: int const)

min returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int const    +
+--------+-------------+
+y       +int const    +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_int2__hh_const_hh_int2:

.. das:function:: min(x: int2 const; y: int2 const)

min returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2 const   +
+--------+-------------+
+y       +int2 const   +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_int3__hh_const_hh_int3:

.. das:function:: min(x: int3 const; y: int3 const)

min returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3 const   +
+--------+-------------+
+y       +int3 const   +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_int4__hh_const_hh_int4:

.. das:function:: min(x: int4 const; y: int4 const)

min returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4 const   +
+--------+-------------+
+y       +int4 const   +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_int64__hh_const_hh_int64:

.. das:function:: min(x: int64 const; y: int64 const)

min returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64 const  +
+--------+-------------+
+y       +int64 const  +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_uint__hh_const_hh_uint:

.. das:function:: min(x: uint const; y: uint const)

min returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint const   +
+--------+-------------+
+y       +uint const   +
+--------+-------------+


|function-math-min|

.. _function-_at_math_c__c_min__hh_const_hh_uint64__hh_const_hh_uint64:

.. das:function:: min(x: uint64 const; y: uint64 const)

min returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64 const +
+--------+-------------+
+y       +uint64 const +
+--------+-------------+


|function-math-min|

+++++++++++++++++
float* and double
+++++++++++++++++

  *  :ref:`abs (x:double const) : double <function-_at_math_c__c_abs__hh_const_hh_double>` 
  *  :ref:`abs (x:float const) : float <function-_at_math_c__c_abs__hh_const_hh_float>` 
  *  :ref:`abs (x:float2 const) : float2 <function-_at_math_c__c_abs__hh_const_hh_float2>` 
  *  :ref:`abs (x:float3 const) : float3 <function-_at_math_c__c_abs__hh_const_hh_float3>` 
  *  :ref:`abs (x:float4 const) : float4 <function-_at_math_c__c_abs__hh_const_hh_float4>` 
  *  :ref:`abs (x:int const) : int <function-_at_math_c__c_abs__hh_const_hh_int>` 
  *  :ref:`abs (x:int2 const) : int2 <function-_at_math_c__c_abs__hh_const_hh_int2>` 
  *  :ref:`abs (x:int3 const) : int3 <function-_at_math_c__c_abs__hh_const_hh_int3>` 
  *  :ref:`abs (x:int4 const) : int4 <function-_at_math_c__c_abs__hh_const_hh_int4>` 
  *  :ref:`abs (x:int64 const) : int64 <function-_at_math_c__c_abs__hh_const_hh_int64>` 
  *  :ref:`abs (x:uint const) : uint <function-_at_math_c__c_abs__hh_const_hh_uint>` 
  *  :ref:`abs (x:uint64 const) : uint64 <function-_at_math_c__c_abs__hh_const_hh_uint64>` 
  *  :ref:`acos (x:double const) : double <function-_at_math_c__c_acos__hh_const_hh_double>` 
  *  :ref:`acos (x:float const) : float <function-_at_math_c__c_acos__hh_const_hh_float>` 
  *  :ref:`acos (x:float2 const) : float2 <function-_at_math_c__c_acos__hh_const_hh_float2>` 
  *  :ref:`acos (x:float3 const) : float3 <function-_at_math_c__c_acos__hh_const_hh_float3>` 
  *  :ref:`acos (x:float4 const) : float4 <function-_at_math_c__c_acos__hh_const_hh_float4>` 
  *  :ref:`asin (x:double const) : double <function-_at_math_c__c_asin__hh_const_hh_double>` 
  *  :ref:`asin (x:float const) : float <function-_at_math_c__c_asin__hh_const_hh_float>` 
  *  :ref:`asin (x:float2 const) : float2 <function-_at_math_c__c_asin__hh_const_hh_float2>` 
  *  :ref:`asin (x:float3 const) : float3 <function-_at_math_c__c_asin__hh_const_hh_float3>` 
  *  :ref:`asin (x:float4 const) : float4 <function-_at_math_c__c_asin__hh_const_hh_float4>` 
  *  :ref:`atan (x:double const) : double <function-_at_math_c__c_atan__hh_const_hh_double>` 
  *  :ref:`atan (x:float const) : float <function-_at_math_c__c_atan__hh_const_hh_float>` 
  *  :ref:`atan (x:float2 const) : float2 <function-_at_math_c__c_atan__hh_const_hh_float2>` 
  *  :ref:`atan (x:float3 const) : float3 <function-_at_math_c__c_atan__hh_const_hh_float3>` 
  *  :ref:`atan (x:float4 const) : float4 <function-_at_math_c__c_atan__hh_const_hh_float4>` 
  *  :ref:`atan2 (y:double const;x:double const) : double <function-_at_math_c__c_atan2__hh_const_hh_double__hh_const_hh_double>` 
  *  :ref:`atan2 (y:float const;x:float const) : float <function-_at_math_c__c_atan2__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`atan2 (y:float2 const;x:float2 const) : float2 <function-_at_math_c__c_atan2__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`atan2 (y:float3 const;x:float3 const) : float3 <function-_at_math_c__c_atan2__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`atan2 (y:float4 const;x:float4 const) : float4 <function-_at_math_c__c_atan2__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`ceil (x:float const) : float <function-_at_math_c__c_ceil__hh_const_hh_float>` 
  *  :ref:`ceil (x:float2 const) : float2 <function-_at_math_c__c_ceil__hh_const_hh_float2>` 
  *  :ref:`ceil (x:float3 const) : float3 <function-_at_math_c__c_ceil__hh_const_hh_float3>` 
  *  :ref:`ceil (x:float4 const) : float4 <function-_at_math_c__c_ceil__hh_const_hh_float4>` 
  *  :ref:`cos (x:double const) : double <function-_at_math_c__c_cos__hh_const_hh_double>` 
  *  :ref:`cos (x:float const) : float <function-_at_math_c__c_cos__hh_const_hh_float>` 
  *  :ref:`cos (x:float2 const) : float2 <function-_at_math_c__c_cos__hh_const_hh_float2>` 
  *  :ref:`cos (x:float3 const) : float3 <function-_at_math_c__c_cos__hh_const_hh_float3>` 
  *  :ref:`cos (x:float4 const) : float4 <function-_at_math_c__c_cos__hh_const_hh_float4>` 
  *  :ref:`exp (x:double const) : double <function-_at_math_c__c_exp__hh_const_hh_double>` 
  *  :ref:`exp (x:float const) : float <function-_at_math_c__c_exp__hh_const_hh_float>` 
  *  :ref:`exp (x:float2 const) : float2 <function-_at_math_c__c_exp__hh_const_hh_float2>` 
  *  :ref:`exp (x:float3 const) : float3 <function-_at_math_c__c_exp__hh_const_hh_float3>` 
  *  :ref:`exp (x:float4 const) : float4 <function-_at_math_c__c_exp__hh_const_hh_float4>` 
  *  :ref:`exp2 (x:double const) : double <function-_at_math_c__c_exp2__hh_const_hh_double>` 
  *  :ref:`exp2 (x:float const) : float <function-_at_math_c__c_exp2__hh_const_hh_float>` 
  *  :ref:`exp2 (x:float2 const) : float2 <function-_at_math_c__c_exp2__hh_const_hh_float2>` 
  *  :ref:`exp2 (x:float3 const) : float3 <function-_at_math_c__c_exp2__hh_const_hh_float3>` 
  *  :ref:`exp2 (x:float4 const) : float4 <function-_at_math_c__c_exp2__hh_const_hh_float4>` 
  *  :ref:`floor (x:float const) : float <function-_at_math_c__c_floor__hh_const_hh_float>` 
  *  :ref:`floor (x:float2 const) : float2 <function-_at_math_c__c_floor__hh_const_hh_float2>` 
  *  :ref:`floor (x:float3 const) : float3 <function-_at_math_c__c_floor__hh_const_hh_float3>` 
  *  :ref:`floor (x:float4 const) : float4 <function-_at_math_c__c_floor__hh_const_hh_float4>` 
  *  :ref:`is_finite (x:double const) : bool <function-_at_math_c__c_is_finite__hh_const_hh_double>` 
  *  :ref:`is_finite (x:float const) : bool <function-_at_math_c__c_is_finite__hh_const_hh_float>` 
  *  :ref:`is_nan (x:double const) : bool <function-_at_math_c__c_is_nan__hh_const_hh_double>` 
  *  :ref:`is_nan (x:float const) : bool <function-_at_math_c__c_is_nan__hh_const_hh_float>` 
  *  :ref:`log (x:double const) : double <function-_at_math_c__c_log__hh_const_hh_double>` 
  *  :ref:`log (x:float const) : float <function-_at_math_c__c_log__hh_const_hh_float>` 
  *  :ref:`log (x:float2 const) : float2 <function-_at_math_c__c_log__hh_const_hh_float2>` 
  *  :ref:`log (x:float3 const) : float3 <function-_at_math_c__c_log__hh_const_hh_float3>` 
  *  :ref:`log (x:float4 const) : float4 <function-_at_math_c__c_log__hh_const_hh_float4>` 
  *  :ref:`log2 (x:double const) : double <function-_at_math_c__c_log2__hh_const_hh_double>` 
  *  :ref:`log2 (x:float const) : float <function-_at_math_c__c_log2__hh_const_hh_float>` 
  *  :ref:`log2 (x:float2 const) : float2 <function-_at_math_c__c_log2__hh_const_hh_float2>` 
  *  :ref:`log2 (x:float3 const) : float3 <function-_at_math_c__c_log2__hh_const_hh_float3>` 
  *  :ref:`log2 (x:float4 const) : float4 <function-_at_math_c__c_log2__hh_const_hh_float4>` 
  *  :ref:`pow (x:double const;y:double const) : double <function-_at_math_c__c_pow__hh_const_hh_double__hh_const_hh_double>` 
  *  :ref:`pow (x:float const;y:float const) : float <function-_at_math_c__c_pow__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`pow (x:float2 const;y:float2 const) : float2 <function-_at_math_c__c_pow__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`pow (x:float3 const;y:float3 const) : float3 <function-_at_math_c__c_pow__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`pow (x:float4 const;y:float4 const) : float4 <function-_at_math_c__c_pow__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`rcp (x:double const) : double <function-_at_math_c__c_rcp__hh_const_hh_double>` 
  *  :ref:`rcp (x:float const) : float <function-_at_math_c__c_rcp__hh_const_hh_float>` 
  *  :ref:`rcp (x:float2 const) : float2 <function-_at_math_c__c_rcp__hh_const_hh_float2>` 
  *  :ref:`rcp (x:float3 const) : float3 <function-_at_math_c__c_rcp__hh_const_hh_float3>` 
  *  :ref:`rcp (x:float4 const) : float4 <function-_at_math_c__c_rcp__hh_const_hh_float4>` 
  *  :ref:`saturate (x:float const) : float <function-_at_math_c__c_saturate__hh_const_hh_float>` 
  *  :ref:`saturate (x:float2 const) : float2 <function-_at_math_c__c_saturate__hh_const_hh_float2>` 
  *  :ref:`saturate (x:float3 const) : float3 <function-_at_math_c__c_saturate__hh_const_hh_float3>` 
  *  :ref:`saturate (x:float4 const) : float4 <function-_at_math_c__c_saturate__hh_const_hh_float4>` 
  *  :ref:`sign (x:double const) : double <function-_at_math_c__c_sign__hh_const_hh_double>` 
  *  :ref:`sign (x:float const) : float <function-_at_math_c__c_sign__hh_const_hh_float>` 
  *  :ref:`sign (x:float2 const) : float2 <function-_at_math_c__c_sign__hh_const_hh_float2>` 
  *  :ref:`sign (x:float3 const) : float3 <function-_at_math_c__c_sign__hh_const_hh_float3>` 
  *  :ref:`sign (x:float4 const) : float4 <function-_at_math_c__c_sign__hh_const_hh_float4>` 
  *  :ref:`sign (x:int const) : int <function-_at_math_c__c_sign__hh_const_hh_int>` 
  *  :ref:`sign (x:int2 const) : int2 <function-_at_math_c__c_sign__hh_const_hh_int2>` 
  *  :ref:`sign (x:int3 const) : int3 <function-_at_math_c__c_sign__hh_const_hh_int3>` 
  *  :ref:`sign (x:int4 const) : int4 <function-_at_math_c__c_sign__hh_const_hh_int4>` 
  *  :ref:`sign (x:int64 const) : int64 <function-_at_math_c__c_sign__hh_const_hh_int64>` 
  *  :ref:`sign (x:uint const) : uint <function-_at_math_c__c_sign__hh_const_hh_uint>` 
  *  :ref:`sign (x:uint64 const) : uint64 <function-_at_math_c__c_sign__hh_const_hh_uint64>` 
  *  :ref:`sin (x:double const) : double <function-_at_math_c__c_sin__hh_const_hh_double>` 
  *  :ref:`sin (x:float const) : float <function-_at_math_c__c_sin__hh_const_hh_float>` 
  *  :ref:`sin (x:float2 const) : float2 <function-_at_math_c__c_sin__hh_const_hh_float2>` 
  *  :ref:`sin (x:float3 const) : float3 <function-_at_math_c__c_sin__hh_const_hh_float3>` 
  *  :ref:`sin (x:float4 const) : float4 <function-_at_math_c__c_sin__hh_const_hh_float4>` 
  *  :ref:`sincos (x:double const;s:double& implicit;c:double& implicit) : void <function-_at_math_c__c_sincos__hh_const_hh_double__hh_ref_hh__hh_implicit_hh_double__hh_ref_hh__hh_implicit_hh_double>` 
  *  :ref:`sincos (x:float const;s:float& implicit;c:float& implicit) : void <function-_at_math_c__c_sincos__hh_const_hh_float__hh_ref_hh__hh_implicit_hh_float__hh_ref_hh__hh_implicit_hh_float>` 
  *  :ref:`sqrt (x:double const) : double <function-_at_math_c__c_sqrt__hh_const_hh_double>` 
  *  :ref:`sqrt (x:float const) : float <function-_at_math_c__c_sqrt__hh_const_hh_float>` 
  *  :ref:`sqrt (x:float2 const) : float2 <function-_at_math_c__c_sqrt__hh_const_hh_float2>` 
  *  :ref:`sqrt (x:float3 const) : float3 <function-_at_math_c__c_sqrt__hh_const_hh_float3>` 
  *  :ref:`sqrt (x:float4 const) : float4 <function-_at_math_c__c_sqrt__hh_const_hh_float4>` 
  *  :ref:`tan (x:double const) : double <function-_at_math_c__c_tan__hh_const_hh_double>` 
  *  :ref:`tan (x:float const) : float <function-_at_math_c__c_tan__hh_const_hh_float>` 
  *  :ref:`tan (x:float2 const) : float2 <function-_at_math_c__c_tan__hh_const_hh_float2>` 
  *  :ref:`tan (x:float3 const) : float3 <function-_at_math_c__c_tan__hh_const_hh_float3>` 
  *  :ref:`tan (x:float4 const) : float4 <function-_at_math_c__c_tan__hh_const_hh_float4>` 

.. _function-_at_math_c__c_abs__hh_const_hh_double:

.. das:function:: abs(x: double const)

abs returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_float:

.. das:function:: abs(x: float const)

abs returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_float2:

.. das:function:: abs(x: float2 const)

abs returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_float3:

.. das:function:: abs(x: float3 const)

abs returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_float4:

.. das:function:: abs(x: float4 const)

abs returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_int:

.. das:function:: abs(x: int const)

abs returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int const    +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_int2:

.. das:function:: abs(x: int2 const)

abs returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2 const   +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_int3:

.. das:function:: abs(x: int3 const)

abs returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3 const   +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_int4:

.. das:function:: abs(x: int4 const)

abs returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4 const   +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_int64:

.. das:function:: abs(x: int64 const)

abs returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64 const  +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_uint:

.. das:function:: abs(x: uint const)

abs returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint const   +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_abs__hh_const_hh_uint64:

.. das:function:: abs(x: uint64 const)

abs returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64 const +
+--------+-------------+


|function-math-abs|

.. _function-_at_math_c__c_acos__hh_const_hh_double:

.. das:function:: acos(x: double const)

acos returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos__hh_const_hh_float:

.. das:function:: acos(x: float const)

acos returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos__hh_const_hh_float2:

.. das:function:: acos(x: float2 const)

acos returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos__hh_const_hh_float3:

.. das:function:: acos(x: float3 const)

acos returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_acos__hh_const_hh_float4:

.. das:function:: acos(x: float4 const)

acos returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-acos|

.. _function-_at_math_c__c_asin__hh_const_hh_double:

.. das:function:: asin(x: double const)

asin returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin__hh_const_hh_float:

.. das:function:: asin(x: float const)

asin returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin__hh_const_hh_float2:

.. das:function:: asin(x: float2 const)

asin returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin__hh_const_hh_float3:

.. das:function:: asin(x: float3 const)

asin returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_asin__hh_const_hh_float4:

.. das:function:: asin(x: float4 const)

asin returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-asin|

.. _function-_at_math_c__c_atan__hh_const_hh_double:

.. das:function:: atan(x: double const)

atan returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan__hh_const_hh_float:

.. das:function:: atan(x: float const)

atan returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan__hh_const_hh_float2:

.. das:function:: atan(x: float2 const)

atan returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan__hh_const_hh_float3:

.. das:function:: atan(x: float3 const)

atan returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan__hh_const_hh_float4:

.. das:function:: atan(x: float4 const)

atan returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-atan|

.. _function-_at_math_c__c_atan2__hh_const_hh_double__hh_const_hh_double:

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

.. _function-_at_math_c__c_atan2__hh_const_hh_float__hh_const_hh_float:

.. das:function:: atan2(y: float const; x: float const)

atan2 returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float const  +
+--------+-------------+
+x       +float const  +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: atan2(y: float2 const; x: float2 const)

atan2 returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float2 const +
+--------+-------------+
+x       +float2 const +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: atan2(y: float3 const; x: float3 const)

atan2 returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float3 const +
+--------+-------------+
+x       +float3 const +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_atan2__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: atan2(y: float4 const; x: float4 const)

atan2 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float4 const +
+--------+-------------+
+x       +float4 const +
+--------+-------------+


|function-math-atan2|

.. _function-_at_math_c__c_ceil__hh_const_hh_float:

.. das:function:: ceil(x: float const)

ceil returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_ceil__hh_const_hh_float2:

.. das:function:: ceil(x: float2 const)

ceil returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_ceil__hh_const_hh_float3:

.. das:function:: ceil(x: float3 const)

ceil returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_ceil__hh_const_hh_float4:

.. das:function:: ceil(x: float4 const)

ceil returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-ceil|

.. _function-_at_math_c__c_cos__hh_const_hh_double:

.. das:function:: cos(x: double const)

cos returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos__hh_const_hh_float:

.. das:function:: cos(x: float const)

cos returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos__hh_const_hh_float2:

.. das:function:: cos(x: float2 const)

cos returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos__hh_const_hh_float3:

.. das:function:: cos(x: float3 const)

cos returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_cos__hh_const_hh_float4:

.. das:function:: cos(x: float4 const)

cos returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-cos|

.. _function-_at_math_c__c_exp__hh_const_hh_double:

.. das:function:: exp(x: double const)

exp returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp__hh_const_hh_float:

.. das:function:: exp(x: float const)

exp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp__hh_const_hh_float2:

.. das:function:: exp(x: float2 const)

exp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp__hh_const_hh_float3:

.. das:function:: exp(x: float3 const)

exp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp__hh_const_hh_float4:

.. das:function:: exp(x: float4 const)

exp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-exp|

.. _function-_at_math_c__c_exp2__hh_const_hh_double:

.. das:function:: exp2(x: double const)

exp2 returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2__hh_const_hh_float:

.. das:function:: exp2(x: float const)

exp2 returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2__hh_const_hh_float2:

.. das:function:: exp2(x: float2 const)

exp2 returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2__hh_const_hh_float3:

.. das:function:: exp2(x: float3 const)

exp2 returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_exp2__hh_const_hh_float4:

.. das:function:: exp2(x: float4 const)

exp2 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-exp2|

.. _function-_at_math_c__c_floor__hh_const_hh_float:

.. das:function:: floor(x: float const)

floor returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_floor__hh_const_hh_float2:

.. das:function:: floor(x: float2 const)

floor returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_floor__hh_const_hh_float3:

.. das:function:: floor(x: float3 const)

floor returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_floor__hh_const_hh_float4:

.. das:function:: floor(x: float4 const)

floor returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-floor|

.. _function-_at_math_c__c_is_finite__hh_const_hh_double:

.. das:function:: is_finite(x: double const)

is_finite returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-is_finite|

.. _function-_at_math_c__c_is_finite__hh_const_hh_float:

.. das:function:: is_finite(x: float const)

is_finite returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-is_finite|

.. _function-_at_math_c__c_is_nan__hh_const_hh_double:

.. das:function:: is_nan(x: double const)

is_nan returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-is_nan|

.. _function-_at_math_c__c_is_nan__hh_const_hh_float:

.. das:function:: is_nan(x: float const)

is_nan returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-is_nan|

.. _function-_at_math_c__c_log__hh_const_hh_double:

.. das:function:: log(x: double const)

log returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log__hh_const_hh_float:

.. das:function:: log(x: float const)

log returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log__hh_const_hh_float2:

.. das:function:: log(x: float2 const)

log returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log__hh_const_hh_float3:

.. das:function:: log(x: float3 const)

log returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log__hh_const_hh_float4:

.. das:function:: log(x: float4 const)

log returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-log|

.. _function-_at_math_c__c_log2__hh_const_hh_double:

.. das:function:: log2(x: double const)

log2 returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2__hh_const_hh_float:

.. das:function:: log2(x: float const)

log2 returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2__hh_const_hh_float2:

.. das:function:: log2(x: float2 const)

log2 returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2__hh_const_hh_float3:

.. das:function:: log2(x: float3 const)

log2 returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_log2__hh_const_hh_float4:

.. das:function:: log2(x: float4 const)

log2 returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-log2|

.. _function-_at_math_c__c_pow__hh_const_hh_double__hh_const_hh_double:

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

.. _function-_at_math_c__c_pow__hh_const_hh_float__hh_const_hh_float:

.. das:function:: pow(x: float const; y: float const)

pow returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+
+y       +float const  +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: pow(x: float2 const; y: float2 const)

pow returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+
+y       +float2 const +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: pow(x: float3 const; y: float3 const)

pow returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+
+y       +float3 const +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_pow__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: pow(x: float4 const; y: float4 const)

pow returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+
+y       +float4 const +
+--------+-------------+


|function-math-pow|

.. _function-_at_math_c__c_rcp__hh_const_hh_double:

.. das:function:: rcp(x: double const)

rcp returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp__hh_const_hh_float:

.. das:function:: rcp(x: float const)

rcp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp__hh_const_hh_float2:

.. das:function:: rcp(x: float2 const)

rcp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp__hh_const_hh_float3:

.. das:function:: rcp(x: float3 const)

rcp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_rcp__hh_const_hh_float4:

.. das:function:: rcp(x: float4 const)

rcp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-rcp|

.. _function-_at_math_c__c_saturate__hh_const_hh_float:

.. das:function:: saturate(x: float const)

saturate returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_saturate__hh_const_hh_float2:

.. das:function:: saturate(x: float2 const)

saturate returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_saturate__hh_const_hh_float3:

.. das:function:: saturate(x: float3 const)

saturate returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_saturate__hh_const_hh_float4:

.. das:function:: saturate(x: float4 const)

saturate returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-saturate|

.. _function-_at_math_c__c_sign__hh_const_hh_double:

.. das:function:: sign(x: double const)

sign returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_float:

.. das:function:: sign(x: float const)

sign returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_float2:

.. das:function:: sign(x: float2 const)

sign returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_float3:

.. das:function:: sign(x: float3 const)

sign returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_float4:

.. das:function:: sign(x: float4 const)

sign returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_int:

.. das:function:: sign(x: int const)

sign returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int const    +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_int2:

.. das:function:: sign(x: int2 const)

sign returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int2 const   +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_int3:

.. das:function:: sign(x: int3 const)

sign returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int3 const   +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_int4:

.. das:function:: sign(x: int4 const)

sign returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int4 const   +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_int64:

.. das:function:: sign(x: int64 const)

sign returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +int64 const  +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_uint:

.. das:function:: sign(x: uint const)

sign returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint const   +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sign__hh_const_hh_uint64:

.. das:function:: sign(x: uint64 const)

sign returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint64 const +
+--------+-------------+


|function-math-sign|

.. _function-_at_math_c__c_sin__hh_const_hh_double:

.. das:function:: sin(x: double const)

sin returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin__hh_const_hh_float:

.. das:function:: sin(x: float const)

sin returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin__hh_const_hh_float2:

.. das:function:: sin(x: float2 const)

sin returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin__hh_const_hh_float3:

.. das:function:: sin(x: float3 const)

sin returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sin__hh_const_hh_float4:

.. das:function:: sin(x: float4 const)

sin returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-sin|

.. _function-_at_math_c__c_sincos__hh_const_hh_double__hh_ref_hh__hh_implicit_hh_double__hh_ref_hh__hh_implicit_hh_double:

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

.. _function-_at_math_c__c_sincos__hh_const_hh_float__hh_ref_hh__hh_implicit_hh_float__hh_ref_hh__hh_implicit_hh_float:

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

.. _function-_at_math_c__c_sqrt__hh_const_hh_double:

.. das:function:: sqrt(x: double const)

sqrt returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt__hh_const_hh_float:

.. das:function:: sqrt(x: float const)

sqrt returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt__hh_const_hh_float2:

.. das:function:: sqrt(x: float2 const)

sqrt returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt__hh_const_hh_float3:

.. das:function:: sqrt(x: float3 const)

sqrt returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_sqrt__hh_const_hh_float4:

.. das:function:: sqrt(x: float4 const)

sqrt returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-sqrt|

.. _function-_at_math_c__c_tan__hh_const_hh_double:

.. das:function:: tan(x: double const)

tan returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +double const +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan__hh_const_hh_float:

.. das:function:: tan(x: float const)

tan returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan__hh_const_hh_float2:

.. das:function:: tan(x: float2 const)

tan returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan__hh_const_hh_float3:

.. das:function:: tan(x: float3 const)

tan returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-tan|

.. _function-_at_math_c__c_tan__hh_const_hh_float4:

.. das:function:: tan(x: float4 const)

tan returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-tan|

+++++++++++
float* only
+++++++++++

  *  :ref:`atan2_est (y:float const;x:float const) : float <function-_at_math_c__c_atan2_est__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`atan2_est (y:float2 const;x:float2 const) : float2 <function-_at_math_c__c_atan2_est__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`atan2_est (y:float3 const;x:float3 const) : float3 <function-_at_math_c__c_atan2_est__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`atan2_est (y:float4 const;x:float4 const) : float4 <function-_at_math_c__c_atan2_est__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`ceili (x:float const) : int <function-_at_math_c__c_ceili__hh_const_hh_float>` 
  *  :ref:`ceili (x:float2 const) : int2 <function-_at_math_c__c_ceili__hh_const_hh_float2>` 
  *  :ref:`ceili (x:float3 const) : int3 <function-_at_math_c__c_ceili__hh_const_hh_float3>` 
  *  :ref:`ceili (x:float4 const) : int4 <function-_at_math_c__c_ceili__hh_const_hh_float4>` 
  *  :ref:`floori (x:float const) : int <function-_at_math_c__c_floori__hh_const_hh_float>` 
  *  :ref:`floori (x:float2 const) : int2 <function-_at_math_c__c_floori__hh_const_hh_float2>` 
  *  :ref:`floori (x:float3 const) : int3 <function-_at_math_c__c_floori__hh_const_hh_float3>` 
  *  :ref:`floori (x:float4 const) : int4 <function-_at_math_c__c_floori__hh_const_hh_float4>` 
  *  :ref:`rcp_est (x:float const) : float <function-_at_math_c__c_rcp_est__hh_const_hh_float>` 
  *  :ref:`rcp_est (x:float2 const) : float2 <function-_at_math_c__c_rcp_est__hh_const_hh_float2>` 
  *  :ref:`rcp_est (x:float3 const) : float3 <function-_at_math_c__c_rcp_est__hh_const_hh_float3>` 
  *  :ref:`rcp_est (x:float4 const) : float4 <function-_at_math_c__c_rcp_est__hh_const_hh_float4>` 
  *  :ref:`roundi (x:float const) : int <function-_at_math_c__c_roundi__hh_const_hh_float>` 
  *  :ref:`roundi (x:float2 const) : int2 <function-_at_math_c__c_roundi__hh_const_hh_float2>` 
  *  :ref:`roundi (x:float3 const) : int3 <function-_at_math_c__c_roundi__hh_const_hh_float3>` 
  *  :ref:`roundi (x:float4 const) : int4 <function-_at_math_c__c_roundi__hh_const_hh_float4>` 
  *  :ref:`rsqrt (x:float const) : float <function-_at_math_c__c_rsqrt__hh_const_hh_float>` 
  *  :ref:`rsqrt (x:float2 const) : float2 <function-_at_math_c__c_rsqrt__hh_const_hh_float2>` 
  *  :ref:`rsqrt (x:float3 const) : float3 <function-_at_math_c__c_rsqrt__hh_const_hh_float3>` 
  *  :ref:`rsqrt (x:float4 const) : float4 <function-_at_math_c__c_rsqrt__hh_const_hh_float4>` 
  *  :ref:`rsqrt_est (x:float const) : float <function-_at_math_c__c_rsqrt_est__hh_const_hh_float>` 
  *  :ref:`rsqrt_est (x:float2 const) : float2 <function-_at_math_c__c_rsqrt_est__hh_const_hh_float2>` 
  *  :ref:`rsqrt_est (x:float3 const) : float3 <function-_at_math_c__c_rsqrt_est__hh_const_hh_float3>` 
  *  :ref:`rsqrt_est (x:float4 const) : float4 <function-_at_math_c__c_rsqrt_est__hh_const_hh_float4>` 
  *  :ref:`trunci (x:float const) : int <function-_at_math_c__c_trunci__hh_const_hh_float>` 
  *  :ref:`trunci (x:float2 const) : int2 <function-_at_math_c__c_trunci__hh_const_hh_float2>` 
  *  :ref:`trunci (x:float3 const) : int3 <function-_at_math_c__c_trunci__hh_const_hh_float3>` 
  *  :ref:`trunci (x:float4 const) : int4 <function-_at_math_c__c_trunci__hh_const_hh_float4>` 

.. _function-_at_math_c__c_atan2_est__hh_const_hh_float__hh_const_hh_float:

.. das:function:: atan2_est(y: float const; x: float const)

atan2_est returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float const  +
+--------+-------------+
+x       +float const  +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_atan2_est__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: atan2_est(y: float2 const; x: float2 const)

atan2_est returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float2 const +
+--------+-------------+
+x       +float2 const +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_atan2_est__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: atan2_est(y: float3 const; x: float3 const)

atan2_est returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float3 const +
+--------+-------------+
+x       +float3 const +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_atan2_est__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: atan2_est(y: float4 const; x: float4 const)

atan2_est returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+y       +float4 const +
+--------+-------------+
+x       +float4 const +
+--------+-------------+


|function-math-atan2_est|

.. _function-_at_math_c__c_ceili__hh_const_hh_float:

.. das:function:: ceili(x: float const)

ceili returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_ceili__hh_const_hh_float2:

.. das:function:: ceili(x: float2 const)

ceili returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_ceili__hh_const_hh_float3:

.. das:function:: ceili(x: float3 const)

ceili returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_ceili__hh_const_hh_float4:

.. das:function:: ceili(x: float4 const)

ceili returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-ceili|

.. _function-_at_math_c__c_floori__hh_const_hh_float:

.. das:function:: floori(x: float const)

floori returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_floori__hh_const_hh_float2:

.. das:function:: floori(x: float2 const)

floori returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_floori__hh_const_hh_float3:

.. das:function:: floori(x: float3 const)

floori returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_floori__hh_const_hh_float4:

.. das:function:: floori(x: float4 const)

floori returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-floori|

.. _function-_at_math_c__c_rcp_est__hh_const_hh_float:

.. das:function:: rcp_est(x: float const)

rcp_est returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_rcp_est__hh_const_hh_float2:

.. das:function:: rcp_est(x: float2 const)

rcp_est returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_rcp_est__hh_const_hh_float3:

.. das:function:: rcp_est(x: float3 const)

rcp_est returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_rcp_est__hh_const_hh_float4:

.. das:function:: rcp_est(x: float4 const)

rcp_est returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-rcp_est|

.. _function-_at_math_c__c_roundi__hh_const_hh_float:

.. das:function:: roundi(x: float const)

roundi returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_roundi__hh_const_hh_float2:

.. das:function:: roundi(x: float2 const)

roundi returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_roundi__hh_const_hh_float3:

.. das:function:: roundi(x: float3 const)

roundi returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_roundi__hh_const_hh_float4:

.. das:function:: roundi(x: float4 const)

roundi returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-roundi|

.. _function-_at_math_c__c_rsqrt__hh_const_hh_float:

.. das:function:: rsqrt(x: float const)

rsqrt returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt__hh_const_hh_float2:

.. das:function:: rsqrt(x: float2 const)

rsqrt returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt__hh_const_hh_float3:

.. das:function:: rsqrt(x: float3 const)

rsqrt returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt__hh_const_hh_float4:

.. das:function:: rsqrt(x: float4 const)

rsqrt returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-rsqrt|

.. _function-_at_math_c__c_rsqrt_est__hh_const_hh_float:

.. das:function:: rsqrt_est(x: float const)

rsqrt_est returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_rsqrt_est__hh_const_hh_float2:

.. das:function:: rsqrt_est(x: float2 const)

rsqrt_est returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_rsqrt_est__hh_const_hh_float3:

.. das:function:: rsqrt_est(x: float3 const)

rsqrt_est returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_rsqrt_est__hh_const_hh_float4:

.. das:function:: rsqrt_est(x: float4 const)

rsqrt_est returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-rsqrt_est|

.. _function-_at_math_c__c_trunci__hh_const_hh_float:

.. das:function:: trunci(x: float const)

trunci returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float const  +
+--------+-------------+


|function-math-trunci|

.. _function-_at_math_c__c_trunci__hh_const_hh_float2:

.. das:function:: trunci(x: float2 const)

trunci returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-trunci|

.. _function-_at_math_c__c_trunci__hh_const_hh_float3:

.. das:function:: trunci(x: float3 const)

trunci returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-trunci|

.. _function-_at_math_c__c_trunci__hh_const_hh_float4:

.. das:function:: trunci(x: float4 const)

trunci returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-trunci|

+++++++++++
float3 only
+++++++++++

  *  :ref:`cross (x:float3 const;y:float3 const) : float3 <function-_at_math_c__c_cross__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`distance (x:float3 const;y:float3 const) : float <function-_at_math_c__c_distance__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`distance_sq (x:float3 const;y:float3 const) : float <function-_at_math_c__c_distance_sq__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`inv_distance (x:float3 const;y:float3 const) : float <function-_at_math_c__c_inv_distance__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`inv_distance_sq (x:float3 const;y:float3 const) : float <function-_at_math_c__c_inv_distance_sq__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`reflect (v:float3 const;n:float3 const) : float3 <function-_at_math_c__c_reflect__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`refract (v:float3 const;n:float3 const;nint:float const;outRefracted:float3&) : bool <function-_at_math_c__c_refract__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float__hh_ref_hh_float3>` 

.. _function-_at_math_c__c_cross__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_distance__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_distance_sq__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_inv_distance__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_inv_distance_sq__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_reflect__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_refract__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float__hh_ref_hh_float3:

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

  *  :ref:`dot (x:float2 const;y:float2 const) : float <function-_at_math_c__c_dot__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`dot (x:float3 const;y:float3 const) : float <function-_at_math_c__c_dot__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`dot (x:float4 const;y:float4 const) : float <function-_at_math_c__c_dot__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`fast_normalize (x:float2 const) : float2 <function-_at_math_c__c_fast_normalize__hh_const_hh_float2>` 
  *  :ref:`fast_normalize (x:float3 const) : float3 <function-_at_math_c__c_fast_normalize__hh_const_hh_float3>` 
  *  :ref:`fast_normalize (x:float4 const) : float4 <function-_at_math_c__c_fast_normalize__hh_const_hh_float4>` 
  *  :ref:`inv_length (x:float2 const) : float <function-_at_math_c__c_inv_length__hh_const_hh_float2>` 
  *  :ref:`inv_length (x:float3 const) : float <function-_at_math_c__c_inv_length__hh_const_hh_float3>` 
  *  :ref:`inv_length (x:float4 const) : float <function-_at_math_c__c_inv_length__hh_const_hh_float4>` 
  *  :ref:`inv_length_sq (x:float2 const) : float <function-_at_math_c__c_inv_length_sq__hh_const_hh_float2>` 
  *  :ref:`inv_length_sq (x:float3 const) : float <function-_at_math_c__c_inv_length_sq__hh_const_hh_float3>` 
  *  :ref:`inv_length_sq (x:float4 const) : float <function-_at_math_c__c_inv_length_sq__hh_const_hh_float4>` 
  *  :ref:`length (x:float2 const) : float <function-_at_math_c__c_length__hh_const_hh_float2>` 
  *  :ref:`length (x:float3 const) : float <function-_at_math_c__c_length__hh_const_hh_float3>` 
  *  :ref:`length (x:float4 const) : float <function-_at_math_c__c_length__hh_const_hh_float4>` 
  *  :ref:`length_sq (x:float2 const) : float <function-_at_math_c__c_length_sq__hh_const_hh_float2>` 
  *  :ref:`length_sq (x:float3 const) : float <function-_at_math_c__c_length_sq__hh_const_hh_float3>` 
  *  :ref:`length_sq (x:float4 const) : float <function-_at_math_c__c_length_sq__hh_const_hh_float4>` 
  *  :ref:`normalize (x:float2 const) : float2 <function-_at_math_c__c_normalize__hh_const_hh_float2>` 
  *  :ref:`normalize (x:float3 const) : float3 <function-_at_math_c__c_normalize__hh_const_hh_float3>` 
  *  :ref:`normalize (x:float4 const) : float4 <function-_at_math_c__c_normalize__hh_const_hh_float4>` 

.. _function-_at_math_c__c_dot__hh_const_hh_float2__hh_const_hh_float2:

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

.. _function-_at_math_c__c_dot__hh_const_hh_float3__hh_const_hh_float3:

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

.. _function-_at_math_c__c_dot__hh_const_hh_float4__hh_const_hh_float4:

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

.. _function-_at_math_c__c_fast_normalize__hh_const_hh_float2:

.. das:function:: fast_normalize(x: float2 const)

fast_normalize returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-fast_normalize|

.. _function-_at_math_c__c_fast_normalize__hh_const_hh_float3:

.. das:function:: fast_normalize(x: float3 const)

fast_normalize returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-fast_normalize|

.. _function-_at_math_c__c_fast_normalize__hh_const_hh_float4:

.. das:function:: fast_normalize(x: float4 const)

fast_normalize returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-fast_normalize|

.. _function-_at_math_c__c_inv_length__hh_const_hh_float2:

.. das:function:: inv_length(x: float2 const)

inv_length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-inv_length|

.. _function-_at_math_c__c_inv_length__hh_const_hh_float3:

.. das:function:: inv_length(x: float3 const)

inv_length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-inv_length|

.. _function-_at_math_c__c_inv_length__hh_const_hh_float4:

.. das:function:: inv_length(x: float4 const)

inv_length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-inv_length|

.. _function-_at_math_c__c_inv_length_sq__hh_const_hh_float2:

.. das:function:: inv_length_sq(x: float2 const)

inv_length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-inv_length_sq|

.. _function-_at_math_c__c_inv_length_sq__hh_const_hh_float3:

.. das:function:: inv_length_sq(x: float3 const)

inv_length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-inv_length_sq|

.. _function-_at_math_c__c_inv_length_sq__hh_const_hh_float4:

.. das:function:: inv_length_sq(x: float4 const)

inv_length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-inv_length_sq|

.. _function-_at_math_c__c_length__hh_const_hh_float2:

.. das:function:: length(x: float2 const)

length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-length|

.. _function-_at_math_c__c_length__hh_const_hh_float3:

.. das:function:: length(x: float3 const)

length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-length|

.. _function-_at_math_c__c_length__hh_const_hh_float4:

.. das:function:: length(x: float4 const)

length returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-length|

.. _function-_at_math_c__c_length_sq__hh_const_hh_float2:

.. das:function:: length_sq(x: float2 const)

length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-length_sq|

.. _function-_at_math_c__c_length_sq__hh_const_hh_float3:

.. das:function:: length_sq(x: float3 const)

length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-length_sq|

.. _function-_at_math_c__c_length_sq__hh_const_hh_float4:

.. das:function:: length_sq(x: float4 const)

length_sq returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-length_sq|

.. _function-_at_math_c__c_normalize__hh_const_hh_float2:

.. das:function:: normalize(x: float2 const)

normalize returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float2 const +
+--------+-------------+


|function-math-normalize|

.. _function-_at_math_c__c_normalize__hh_const_hh_float3:

.. das:function:: normalize(x: float3 const)

normalize returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float3 const +
+--------+-------------+


|function-math-normalize|

.. _function-_at_math_c__c_normalize__hh_const_hh_float4:

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

  *  :ref:`uint32_hash (seed:uint const) : uint <function-_at_math_c__c_uint32_hash__hh_const_hh_uint>` 
  *  :ref:`uint_noise_1D (position:int const;seed:uint const) : uint <function-_at_math_c__c_uint_noise_1D__hh_const_hh_int__hh_const_hh_uint>` 
  *  :ref:`uint_noise_2D (position:int2 const;seed:uint const) : uint <function-_at_math_c__c_uint_noise_2D__hh_const_hh_int2__hh_const_hh_uint>` 
  *  :ref:`uint_noise_3D (position:int3 const;seed:uint const) : uint <function-_at_math_c__c_uint_noise_3D__hh_const_hh_int3__hh_const_hh_uint>` 

.. _function-_at_math_c__c_uint32_hash__hh_const_hh_uint:

.. das:function:: uint32_hash(seed: uint const)

uint32_hash returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+seed    +uint const   +
+--------+-------------+


|function-math-uint32_hash|

.. _function-_at_math_c__c_uint_noise_1D__hh_const_hh_int__hh_const_hh_uint:

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

.. _function-_at_math_c__c_uint_noise_2D__hh_const_hh_int2__hh_const_hh_uint:

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

.. _function-_at_math_c__c_uint_noise_3D__hh_const_hh_int3__hh_const_hh_uint:

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

  *  :ref:`clamp (t:double const;a:double const;b:double const) : double <function-_at_math_c__c_clamp__hh_const_hh_double__hh_const_hh_double__hh_const_hh_double>` 
  *  :ref:`clamp (t:float const;a:float const;b:float const) : float <function-_at_math_c__c_clamp__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`clamp (t:float2 const;a:float2 const;b:float2 const) : float2 <function-_at_math_c__c_clamp__hh_const_hh_float2__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`clamp (t:float3 const;a:float3 const;b:float3 const) : float3 <function-_at_math_c__c_clamp__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`clamp (t:float4 const;a:float4 const;b:float4 const) : float4 <function-_at_math_c__c_clamp__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`clamp (t:int const;a:int const;b:int const) : int <function-_at_math_c__c_clamp__hh_const_hh_int__hh_const_hh_int__hh_const_hh_int>` 
  *  :ref:`clamp (t:int2 const;a:int2 const;b:int2 const) : int2 <function-_at_math_c__c_clamp__hh_const_hh_int2__hh_const_hh_int2__hh_const_hh_int2>` 
  *  :ref:`clamp (t:int3 const;a:int3 const;b:int3 const) : int3 <function-_at_math_c__c_clamp__hh_const_hh_int3__hh_const_hh_int3__hh_const_hh_int3>` 
  *  :ref:`clamp (t:int4 const;a:int4 const;b:int4 const) : int4 <function-_at_math_c__c_clamp__hh_const_hh_int4__hh_const_hh_int4__hh_const_hh_int4>` 
  *  :ref:`clamp (t:int64 const;a:int64 const;b:int64 const) : int64 <function-_at_math_c__c_clamp__hh_const_hh_int64__hh_const_hh_int64__hh_const_hh_int64>` 
  *  :ref:`clamp (t:uint const;a:uint const;b:uint const) : uint <function-_at_math_c__c_clamp__hh_const_hh_uint__hh_const_hh_uint__hh_const_hh_uint>` 
  *  :ref:`clamp (t:uint64 const;a:uint64 const;b:uint64 const) : uint64 <function-_at_math_c__c_clamp__hh_const_hh_uint64__hh_const_hh_uint64__hh_const_hh_uint64>` 
  *  :ref:`lerp (a:float const;b:float const;t:float const) : float <function-_at_math_c__c_lerp__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`lerp (a:float2 const;b:float2 const;t:float2 const) : float2 <function-_at_math_c__c_lerp__hh_const_hh_float2__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`lerp (a:float3 const;b:float3 const;t:float3 const) : float3 <function-_at_math_c__c_lerp__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`lerp (a:float4 const;b:float4 const;t:float4 const) : float4 <function-_at_math_c__c_lerp__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`mad (a:float const;b:float const;c:float const) : float <function-_at_math_c__c_mad__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`mad (a:float2 const;b:float const;c:float2 const) : float2 <function-_at_math_c__c_mad__hh_const_hh_float2__hh_const_hh_float__hh_const_hh_float2>` 
  *  :ref:`mad (a:float2 const;b:float2 const;c:float2 const) : float2 <function-_at_math_c__c_mad__hh_const_hh_float2__hh_const_hh_float2__hh_const_hh_float2>` 
  *  :ref:`mad (a:float3 const;b:float const;c:float3 const) : float3 <function-_at_math_c__c_mad__hh_const_hh_float3__hh_const_hh_float__hh_const_hh_float3>` 
  *  :ref:`mad (a:float3 const;b:float3 const;c:float3 const) : float3 <function-_at_math_c__c_mad__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`mad (a:float4 const;b:float const;c:float4 const) : float4 <function-_at_math_c__c_mad__hh_const_hh_float4__hh_const_hh_float__hh_const_hh_float4>` 
  *  :ref:`mad (a:float4 const;b:float4 const;c:float4 const) : float4 <function-_at_math_c__c_mad__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4>` 

.. _function-_at_math_c__c_clamp__hh_const_hh_double__hh_const_hh_double__hh_const_hh_double:

.. das:function:: clamp(t: double const; a: double const; b: double const)

clamp returns double

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +double const +
+--------+-------------+
+a       +double const +
+--------+-------------+
+b       +double const +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float:

.. das:function:: clamp(t: float const; a: float const; b: float const)

clamp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float const  +
+--------+-------------+
+a       +float const  +
+--------+-------------+
+b       +float const  +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_float2__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: clamp(t: float2 const; a: float2 const; b: float2 const)

clamp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float2 const +
+--------+-------------+
+a       +float2 const +
+--------+-------------+
+b       +float2 const +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: clamp(t: float3 const; a: float3 const; b: float3 const)

clamp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float3 const +
+--------+-------------+
+a       +float3 const +
+--------+-------------+
+b       +float3 const +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: clamp(t: float4 const; a: float4 const; b: float4 const)

clamp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +float4 const +
+--------+-------------+
+a       +float4 const +
+--------+-------------+
+b       +float4 const +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_int__hh_const_hh_int__hh_const_hh_int:

.. das:function:: clamp(t: int const; a: int const; b: int const)

clamp returns int

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +int const    +
+--------+-------------+
+a       +int const    +
+--------+-------------+
+b       +int const    +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_int2__hh_const_hh_int2__hh_const_hh_int2:

.. das:function:: clamp(t: int2 const; a: int2 const; b: int2 const)

clamp returns int2

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +int2 const   +
+--------+-------------+
+a       +int2 const   +
+--------+-------------+
+b       +int2 const   +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_int3__hh_const_hh_int3__hh_const_hh_int3:

.. das:function:: clamp(t: int3 const; a: int3 const; b: int3 const)

clamp returns int3

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +int3 const   +
+--------+-------------+
+a       +int3 const   +
+--------+-------------+
+b       +int3 const   +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_int4__hh_const_hh_int4__hh_const_hh_int4:

.. das:function:: clamp(t: int4 const; a: int4 const; b: int4 const)

clamp returns int4

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +int4 const   +
+--------+-------------+
+a       +int4 const   +
+--------+-------------+
+b       +int4 const   +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_int64__hh_const_hh_int64__hh_const_hh_int64:

.. das:function:: clamp(t: int64 const; a: int64 const; b: int64 const)

clamp returns int64

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +int64 const  +
+--------+-------------+
+a       +int64 const  +
+--------+-------------+
+b       +int64 const  +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_uint__hh_const_hh_uint__hh_const_hh_uint:

.. das:function:: clamp(t: uint const; a: uint const; b: uint const)

clamp returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +uint const   +
+--------+-------------+
+a       +uint const   +
+--------+-------------+
+b       +uint const   +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_clamp__hh_const_hh_uint64__hh_const_hh_uint64__hh_const_hh_uint64:

.. das:function:: clamp(t: uint64 const; a: uint64 const; b: uint64 const)

clamp returns uint64

+--------+-------------+
+argument+argument type+
+========+=============+
+t       +uint64 const +
+--------+-------------+
+a       +uint64 const +
+--------+-------------+
+b       +uint64 const +
+--------+-------------+


|function-math-clamp|

.. _function-_at_math_c__c_lerp__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float:

.. das:function:: lerp(a: float const; b: float const; t: float const)

lerp returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float const  +
+--------+-------------+
+b       +float const  +
+--------+-------------+
+t       +float const  +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_lerp__hh_const_hh_float2__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: lerp(a: float2 const; b: float2 const; t: float2 const)

lerp returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float2 const +
+--------+-------------+
+b       +float2 const +
+--------+-------------+
+t       +float2 const +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_lerp__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: lerp(a: float3 const; b: float3 const; t: float3 const)

lerp returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float3 const +
+--------+-------------+
+b       +float3 const +
+--------+-------------+
+t       +float3 const +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_lerp__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: lerp(a: float4 const; b: float4 const; t: float4 const)

lerp returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float4 const +
+--------+-------------+
+b       +float4 const +
+--------+-------------+
+t       +float4 const +
+--------+-------------+


|function-math-lerp|

.. _function-_at_math_c__c_mad__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float:

.. das:function:: mad(a: float const; b: float const; c: float const)

mad returns float

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float const  +
+--------+-------------+
+b       +float const  +
+--------+-------------+
+c       +float const  +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad__hh_const_hh_float2__hh_const_hh_float__hh_const_hh_float2:

.. das:function:: mad(a: float2 const; b: float const; c: float2 const)

mad returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float2 const +
+--------+-------------+
+b       +float const  +
+--------+-------------+
+c       +float2 const +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad__hh_const_hh_float2__hh_const_hh_float2__hh_const_hh_float2:

.. das:function:: mad(a: float2 const; b: float2 const; c: float2 const)

mad returns float2

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float2 const +
+--------+-------------+
+b       +float2 const +
+--------+-------------+
+c       +float2 const +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad__hh_const_hh_float3__hh_const_hh_float__hh_const_hh_float3:

.. das:function:: mad(a: float3 const; b: float const; c: float3 const)

mad returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float3 const +
+--------+-------------+
+b       +float const  +
+--------+-------------+
+c       +float3 const +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad__hh_const_hh_float3__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: mad(a: float3 const; b: float3 const; c: float3 const)

mad returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float3 const +
+--------+-------------+
+b       +float3 const +
+--------+-------------+
+c       +float3 const +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad__hh_const_hh_float4__hh_const_hh_float__hh_const_hh_float4:

.. das:function:: mad(a: float4 const; b: float const; c: float4 const)

mad returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float4 const +
+--------+-------------+
+b       +float const  +
+--------+-------------+
+c       +float4 const +
+--------+-------------+


|function-math-mad|

.. _function-_at_math_c__c_mad__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: mad(a: float4 const; b: float4 const; c: float4 const)

mad returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +float4 const +
+--------+-------------+
+b       +float4 const +
+--------+-------------+
+c       +float4 const +
+--------+-------------+


|function-math-mad|

+++++++++++++++++
Matrix operations
+++++++++++++++++

  *  :ref:`\!= (x:math::float4x4 const implicit;y:math::float4x4 const implicit) : bool <function-_at_math_c__c__ex__eq___hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 
  *  :ref:`* (x:math::float3x4 const implicit;y:math::float3x4 const implicit) : math::float3x4 <function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4>` 
  *  :ref:`* (x:math::float3x4 const implicit;y:float3 const) : float3 <function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4__hh_const_hh_float3>` 
  *  :ref:`* (x:math::float4x4 const implicit;y:math::float4x4 const implicit) : math::float4x4 <function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 
  *  :ref:`* (x:math::float4x4 const implicit;y:float4 const) : float4 <function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh_float4>` 
  *  :ref:`== (x:math::float4x4 const implicit;y:math::float4x4 const implicit) : bool <function-_at_math_c__c__eq__eq___hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 

.. _function-_at_math_c__c__ex__eq___hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

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

.. _function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4:

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

.. _function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4__hh_const_hh_float3:

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

.. _function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

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

.. _function-_at_math_c__c_*__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh_float4:

.. das:function:: operator *(x: float4x4 const implicit; y: float4 const)

* returns float4

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+
+y       +float4 const                                                 +
+--------+-------------------------------------------------------------+


|function-math-*|

.. _function-_at_math_c__c__eq__eq___hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

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

  *  :ref:`compose (pos:float4 const;rot:float4 const;scale:float4 const) : math::float4x4 <function-_at_math_c__c_compose__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`decompose (mat:math::float4x4 const implicit;pos:float3& implicit;rot:float4& implicit;scale:float4& implicit) : void <function-_at_math_c__c_decompose__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_ref_hh__hh_implicit_hh_float3__hh_ref_hh__hh_implicit_hh_float4__hh_ref_hh__hh_implicit_hh_float4>` 
  *  :ref:`identity (x:math::float3x4 implicit) : void <function-_at_math_c__c_identity__hh_implicit_hh__hh_handle_hh_float3x4>` 
  *  :ref:`identity (x:math::float4x4 implicit) : void <function-_at_math_c__c_identity__hh_implicit_hh__hh_handle_hh_float4x4>` 
  *  :ref:`inverse (x:math::float3x4 const implicit) : math::float3x4 <function-_at_math_c__c_inverse__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4>` 
  *  :ref:`inverse (m:math::float4x4 const implicit) : math::float4x4 <function-_at_math_c__c_inverse__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 
  *  :ref:`look_at (eye:float4 const;at:float4 const;up:float4 const) : math::float4x4 <function-_at_math_c__c_look_at__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`orthonormal_inverse (m:math::float4x4 const implicit) : math::float4x4 <function-_at_math_c__c_orthonormal_inverse__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 
  *  :ref:`persp_forward (wk:float const;hk:float const;zn:float const;zf:float const) : math::float4x4 <function-_at_math_c__c_persp_forward__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`persp_reverse (wk:float const;hk:float const;zn:float const;zf:float const) : math::float4x4 <function-_at_math_c__c_persp_reverse__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float>` 
  *  :ref:`rotate (x:math::float3x4 const implicit;y:float3 const) : float3 <function-_at_math_c__c_rotate__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4__hh_const_hh_float3>` 
  *  :ref:`translation (xyz:float3 const) : math::float4x4 <function-_at_math_c__c_translation__hh_const_hh_float3>` 
  *  :ref:`transpose (x:math::float4x4 const implicit) : math::float4x4 <function-_at_math_c__c_transpose__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 

.. _function-_at_math_c__c_compose__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: compose(pos: float4 const; rot: float4 const; scale: float4 const)

compose returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+pos     +float4 const +
+--------+-------------+
+rot     +float4 const +
+--------+-------------+
+scale   +float4 const +
+--------+-------------+


|function-math-compose|

.. _function-_at_math_c__c_decompose__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4__hh_ref_hh__hh_implicit_hh_float3__hh_ref_hh__hh_implicit_hh_float4__hh_ref_hh__hh_implicit_hh_float4:

.. das:function:: decompose(mat: float4x4 const implicit; pos: float3& implicit; rot: float4& implicit; scale: float4& implicit)

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+mat     + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+
+pos     +float3& implicit                                             +
+--------+-------------------------------------------------------------+
+rot     +float4& implicit                                             +
+--------+-------------------------------------------------------------+
+scale   +float4& implicit                                             +
+--------+-------------------------------------------------------------+


|function-math-decompose|

.. _function-_at_math_c__c_identity__hh_implicit_hh__hh_handle_hh_float3x4:

.. das:function:: identity(x: float3x4 implicit)

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  implicit+
+--------+-------------------------------------------------------+


|function-math-identity|

.. _function-_at_math_c__c_identity__hh_implicit_hh__hh_handle_hh_float4x4:

.. das:function:: identity(x: float4x4 implicit)

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  implicit+
+--------+-------------------------------------------------------+


|function-math-identity|

.. _function-_at_math_c__c_inverse__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4:

.. das:function:: inverse(x: float3x4 const implicit)

inverse returns  :ref:`math::float3x4 <handle-math-float3x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float3x4 <handle-math-float3x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-inverse|

.. _function-_at_math_c__c_inverse__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

.. das:function:: inverse(m: float4x4 const implicit)

inverse returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+m       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-inverse|

.. _function-_at_math_c__c_look_at__hh_const_hh_float4__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: look_at(eye: float4 const; at: float4 const; up: float4 const)

look_at returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+eye     +float4 const +
+--------+-------------+
+at      +float4 const +
+--------+-------------+
+up      +float4 const +
+--------+-------------+


|function-math-look_at|

.. _function-_at_math_c__c_orthonormal_inverse__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

.. das:function:: orthonormal_inverse(m: float4x4 const implicit)

orthonormal_inverse returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+m       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-orthonormal_inverse|

.. _function-_at_math_c__c_persp_forward__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float:

.. das:function:: persp_forward(wk: float const; hk: float const; zn: float const; zf: float const)

persp_forward returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+wk      +float const  +
+--------+-------------+
+hk      +float const  +
+--------+-------------+
+zn      +float const  +
+--------+-------------+
+zf      +float const  +
+--------+-------------+


|function-math-persp_forward|

.. _function-_at_math_c__c_persp_reverse__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float__hh_const_hh_float:

.. das:function:: persp_reverse(wk: float const; hk: float const; zn: float const; zf: float const)

persp_reverse returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+wk      +float const  +
+--------+-------------+
+hk      +float const  +
+--------+-------------+
+zn      +float const  +
+--------+-------------+
+zf      +float const  +
+--------+-------------+


|function-math-persp_reverse|

.. _function-_at_math_c__c_rotate__hh_const_hh__hh_implicit_hh__hh_handle_hh_float3x4__hh_const_hh_float3:

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

.. _function-_at_math_c__c_translation__hh_const_hh_float3:

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


.. _function-_at_math_c__c_transpose__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

.. das:function:: transpose(x: float4x4 const implicit)

transpose returns  :ref:`math::float4x4 <handle-math-float4x4>` 

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+x       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-transpose|

+++++++++++++++++++++
Quaternion operations
+++++++++++++++++++++

  *  :ref:`quat_conjugate (q:float4 const) : float4 <function-_at_math_c__c_quat_conjugate__hh_const_hh_float4>` 
  *  :ref:`quat_mul (q1:float4 const;q2:float4 const) : float4 <function-_at_math_c__c_quat_mul__hh_const_hh_float4__hh_const_hh_float4>` 
  *  :ref:`quat_mul_vec (q:float4 const;v:float3 const) : float3 <function-_at_math_c__c_quat_mul_vec__hh_const_hh_float4__hh_const_hh_float3>` 
  *  :ref:`un_quat (m:math::float4x4 const implicit) : float4 <function-_at_math_c__c_un_quat__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4>` 
  *  :ref:`un_quat_from_unit_arc (v0:float3 const;v1:float3 const) : float4 <function-_at_math_c__c_un_quat_from_unit_arc__hh_const_hh_float3__hh_const_hh_float3>` 
  *  :ref:`un_quat_from_unit_vec_ang (v:float3 const;ang:float const) : float4 <function-_at_math_c__c_un_quat_from_unit_vec_ang__hh_const_hh_float3__hh_const_hh_float>` 

.. _function-_at_math_c__c_quat_conjugate__hh_const_hh_float4:

.. das:function:: quat_conjugate(q: float4 const)

quat_conjugate returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+q       +float4 const +
+--------+-------------+


|function-math-quat_conjugate|

.. _function-_at_math_c__c_quat_mul__hh_const_hh_float4__hh_const_hh_float4:

.. das:function:: quat_mul(q1: float4 const; q2: float4 const)

quat_mul returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+q1      +float4 const +
+--------+-------------+
+q2      +float4 const +
+--------+-------------+


|function-math-quat_mul|

.. _function-_at_math_c__c_quat_mul_vec__hh_const_hh_float4__hh_const_hh_float3:

.. das:function:: quat_mul_vec(q: float4 const; v: float3 const)

quat_mul_vec returns float3

+--------+-------------+
+argument+argument type+
+========+=============+
+q       +float4 const +
+--------+-------------+
+v       +float3 const +
+--------+-------------+


|function-math-quat_mul_vec|

.. _function-_at_math_c__c_un_quat__hh_const_hh__hh_implicit_hh__hh_handle_hh_float4x4:

.. das:function:: un_quat(m: float4x4 const implicit)

un_quat returns float4

+--------+-------------------------------------------------------------+
+argument+argument type                                                +
+========+=============================================================+
+m       + :ref:`math::float4x4 <handle-math-float4x4>`  const implicit+
+--------+-------------------------------------------------------------+


|function-math-un_quat|

.. _function-_at_math_c__c_un_quat_from_unit_arc__hh_const_hh_float3__hh_const_hh_float3:

.. das:function:: un_quat_from_unit_arc(v0: float3 const; v1: float3 const)

un_quat_from_unit_arc returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+v0      +float3 const +
+--------+-------------+
+v1      +float3 const +
+--------+-------------+


|function-math-un_quat_from_unit_arc|

.. _function-_at_math_c__c_un_quat_from_unit_vec_ang__hh_const_hh_float3__hh_const_hh_float:

.. das:function:: un_quat_from_unit_vec_ang(v: float3 const; ang: float const)

un_quat_from_unit_vec_ang returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+v       +float3 const +
+--------+-------------+
+ang     +float const  +
+--------+-------------+


|function-math-un_quat_from_unit_vec_ang|

+++++++++++++++++++++
Packing and unpacking
+++++++++++++++++++++

  *  :ref:`pack_float_to_byte (x:float4 const) : uint <function-_at_math_c__c_pack_float_to_byte__hh_const_hh_float4>` 
  *  :ref:`unpack_byte_to_float (x:uint const) : float4 <function-_at_math_c__c_unpack_byte_to_float__hh_const_hh_uint>` 

.. _function-_at_math_c__c_pack_float_to_byte__hh_const_hh_float4:

.. das:function:: pack_float_to_byte(x: float4 const)

pack_float_to_byte returns uint

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +float4 const +
+--------+-------------+


|function-math-pack_float_to_byte|

.. _function-_at_math_c__c_unpack_byte_to_float__hh_const_hh_uint:

.. das:function:: unpack_byte_to_float(x: uint const)

unpack_byte_to_float returns float4

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +uint const   +
+--------+-------------+


|function-math-unpack_byte_to_float|


