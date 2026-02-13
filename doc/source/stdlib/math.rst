
.. _stdlib_math:

============
Math library
============

The MATH module contains floating point math functions and constants
(trigonometry, exponentials, clamping, interpolation, noise, and vector/matrix operations).
Floating point math in general is not bit-precise: the compiler may optimize
permutations, replace divisions with multiplications, and some functions are
not bit-exact. Use ``double`` precision types when exact results are required.

All functions and symbols are in "math" module, use require to get access to it. ::

    require math

Example: ::

    require math

        [export]
        def main() {
            print("sin(PI/2) = {sin(PI / 2.0)}\n")
            print("cos(0)    = {cos(0.0)}\n")
            print("sqrt(16)  = {sqrt(16.0)}\n")
            print("abs(-5)   = {abs(-5)}\n")
            print("clamp(15, 0, 10) = {clamp(15, 0, 10)}\n")
            print("min(3, 7) = {min(3, 7)}\n")
            print("max(3, 7) = {max(3, 7)}\n")
            let v = float3(1, 0, 0)
            print("length = {length(v)}\n")
        }
        // output:
        // sin(PI/2) = 1
        // cos(0)    = 1
        // sqrt(16)  = 4
        // abs(-5)   = 5
        // clamp(15, 0, 10) = 10
        // min(3, 7) = 3
        // max(3, 7) = 7
        // length = 1

+++++++++
Constants
+++++++++

.. _global-math-PI:

.. das:attribute:: PI = 3.1415927f

The single-precision float constant pi (3.14159265...), representing the ratio of a circle's circumference to its diameter.
.. _global-math-DBL_PI:

.. das:attribute:: DBL_PI = 3.141592653589793lf

The double-precision constant pi (3.141592653589793...), representing the ratio of a circle's circumference to its diameter.
.. _global-math-FLT_EPSILON:

.. das:attribute:: FLT_EPSILON = 1.1920929e-07f

The smallest single-precision float value epsilon such that 1.0f + epsilon != 1.0f, approximately 1.1920929e-7.
.. _global-math-DBL_EPSILON:

.. das:attribute:: DBL_EPSILON = 2.220446049250313e-16lf

The smallest double-precision value epsilon such that 1.0 + epsilon != 1.0, approximately 2.2204460492503131e-16.
++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-math-float4x4:

.. das:attribute:: float4x4

floating point matrix with 4 rows and 4 columns

:Fields: * **x** : float4 - 0th row

         * **y** : float4 - 1st row

         * **z** : float4 - 2nd row

         * **w** : float4 - 3rd row


.. _handle-math-float3x4:

.. das:attribute:: float3x4

floating point matrix with 4 rows and 3 columns

:Fields: * **x** : float3 - 0th row

         * **y** : float3 - 1st row

         * **z** : float3 - 2nd row

         * **w** : float3 - 3rd row


.. _handle-math-float3x3:

.. das:attribute:: float3x3

floating point matrix with 3 rows and 3 columns

:Fields: * **x** : float3 - 0th row

         * **y** : float3 - 1st row

         * **z** : float3 - 2nd row


++++++++++++++++++++++++++++++++++++++++++
all numerics (uint*, int*, float*, double)
++++++++++++++++++++++++++++++++++++++++++

  *  :ref:`max (x: int; y: int) : int <function-math_max_int_int>` 
  *  :ref:`max (x: uint64; y: uint64) : uint64 <function-math_max_uint64_uint64>` 
  *  :ref:`max (x: int2; y: int2) : int2 <function-math_max_int2_int2>` 
  *  :ref:`max (x: double; y: double) : double <function-math_max_double_double>` 
  *  :ref:`max (x: int4; y: int4) : int4 <function-math_max_int4_int4>` 
  *  :ref:`max (x: int3; y: int3) : int3 <function-math_max_int3_int3>` 
  *  :ref:`max (x: float3; y: float3) : float3 <function-math_max_float3_float3>` 
  *  :ref:`max (x: int64; y: int64) : int64 <function-math_max_int64_int64>` 
  *  :ref:`max (x: uint2; y: uint2) : uint2 <function-math_max_uint2_uint2>` 
  *  :ref:`max (x: uint; y: uint) : uint <function-math_max_uint_uint>` 
  *  :ref:`max (x: uint3; y: uint3) : uint3 <function-math_max_uint3_uint3>` 
  *  :ref:`max (x: float4; y: float4) : float4 <function-math_max_float4_float4>` 
  *  :ref:`max (x: uint4; y: uint4) : uint4 <function-math_max_uint4_uint4>` 
  *  :ref:`max (x: float2; y: float2) : float2 <function-math_max_float2_float2>` 
  *  :ref:`max (x: float; y: float) : float <function-math_max_float_float>` 
  *  :ref:`min (x: uint3; y: uint3) : uint3 <function-math_min_uint3_uint3>` 
  *  :ref:`min (x: uint2; y: uint2) : uint2 <function-math_min_uint2_uint2>` 
  *  :ref:`min (x: int4; y: int4) : int4 <function-math_min_int4_int4>` 
  *  :ref:`min (x: int; y: int) : int <function-math_min_int_int>` 
  *  :ref:`min (x: float3; y: float3) : float3 <function-math_min_float3_float3>` 
  *  :ref:`min (x: float4; y: float4) : float4 <function-math_min_float4_float4>` 
  *  :ref:`min (x: float2; y: float2) : float2 <function-math_min_float2_float2>` 
  *  :ref:`min (x: float; y: float) : float <function-math_min_float_float>` 
  *  :ref:`min (x: int2; y: int2) : int2 <function-math_min_int2_int2>` 
  *  :ref:`min (x: int3; y: int3) : int3 <function-math_min_int3_int3>` 
  *  :ref:`min (x: uint; y: uint) : uint <function-math_min_uint_uint>` 
  *  :ref:`min (x: uint64; y: uint64) : uint64 <function-math_min_uint64_uint64>` 
  *  :ref:`min (x: uint4; y: uint4) : uint4 <function-math_min_uint4_uint4>` 
  *  :ref:`min (x: double; y: double) : double <function-math_min_double_double>` 
  *  :ref:`min (x: int64; y: int64) : int64 <function-math_min_int64_int64>` 


max
^^^

.. _function-math_max_int_int:

.. das:function:: max(x: int; y: int) : int

Returns the component-wise maximum of two values, supporting scalar double, float, int, int64, uint, uint64 and vector float2, float3, float4 types.

:Arguments: * **x** : int

            * **y** : int

.. _function-math_max_uint64_uint64:

.. das:function:: max(x: uint64; y: uint64) : uint64

.. _function-math_max_int2_int2:

.. das:function:: max(x: int2; y: int2) : int2

.. _function-math_max_double_double:

.. das:function:: max(x: double; y: double) : double

.. _function-math_max_int4_int4:

.. das:function:: max(x: int4; y: int4) : int4

.. _function-math_max_int3_int3:

.. das:function:: max(x: int3; y: int3) : int3

.. _function-math_max_float3_float3:

.. das:function:: max(x: float3; y: float3) : float3

.. _function-math_max_int64_int64:

.. das:function:: max(x: int64; y: int64) : int64

.. _function-math_max_uint2_uint2:

.. das:function:: max(x: uint2; y: uint2) : uint2

.. _function-math_max_uint_uint:

.. das:function:: max(x: uint; y: uint) : uint

.. _function-math_max_uint3_uint3:

.. das:function:: max(x: uint3; y: uint3) : uint3

.. _function-math_max_float4_float4:

.. das:function:: max(x: float4; y: float4) : float4

.. _function-math_max_uint4_uint4:

.. das:function:: max(x: uint4; y: uint4) : uint4

.. _function-math_max_float2_float2:

.. das:function:: max(x: float2; y: float2) : float2

.. _function-math_max_float_float:

.. das:function:: max(x: float; y: float) : float

----


min
^^^

.. _function-math_min_uint3_uint3:

.. das:function:: min(x: uint3; y: uint3) : uint3

Returns the component-wise minimum of two values, supporting scalar double, float, int, int64, uint, uint64 and vector float2, float3, float4 types.

:Arguments: * **x** : uint3

            * **y** : uint3

.. _function-math_min_uint2_uint2:

.. das:function:: min(x: uint2; y: uint2) : uint2

.. _function-math_min_int4_int4:

.. das:function:: min(x: int4; y: int4) : int4

.. _function-math_min_int_int:

.. das:function:: min(x: int; y: int) : int

.. _function-math_min_float3_float3:

.. das:function:: min(x: float3; y: float3) : float3

.. _function-math_min_float4_float4:

.. das:function:: min(x: float4; y: float4) : float4

.. _function-math_min_float2_float2:

.. das:function:: min(x: float2; y: float2) : float2

.. _function-math_min_float_float:

.. das:function:: min(x: float; y: float) : float

.. _function-math_min_int2_int2:

.. das:function:: min(x: int2; y: int2) : int2

.. _function-math_min_int3_int3:

.. das:function:: min(x: int3; y: int3) : int3

.. _function-math_min_uint_uint:

.. das:function:: min(x: uint; y: uint) : uint

.. _function-math_min_uint64_uint64:

.. das:function:: min(x: uint64; y: uint64) : uint64

.. _function-math_min_uint4_uint4:

.. das:function:: min(x: uint4; y: uint4) : uint4

.. _function-math_min_double_double:

.. das:function:: min(x: double; y: double) : double

.. _function-math_min_int64_int64:

.. das:function:: min(x: int64; y: int64) : int64

----

+++++++++++++++++
float* and double
+++++++++++++++++

  *  :ref:`abs (x: float3) : float3 <function-math_abs_float3>` 
  *  :ref:`abs (x: uint) : uint <function-math_abs_uint>` 
  *  :ref:`abs (x: uint64) : uint64 <function-math_abs_uint64>` 
  *  :ref:`abs (x: int2) : int2 <function-math_abs_int2>` 
  *  :ref:`abs (x: int4) : int4 <function-math_abs_int4>` 
  *  :ref:`abs (x: int3) : int3 <function-math_abs_int3>` 
  *  :ref:`abs (x: int64) : int64 <function-math_abs_int64>` 
  *  :ref:`abs (x: uint2) : uint2 <function-math_abs_uint2>` 
  *  :ref:`abs (x: int) : int <function-math_abs_int>` 
  *  :ref:`abs (x: uint4) : uint4 <function-math_abs_uint4>` 
  *  :ref:`abs (x: uint3) : uint3 <function-math_abs_uint3>` 
  *  :ref:`abs (x: double) : double <function-math_abs_double>` 
  *  :ref:`abs (x: float) : float <function-math_abs_float>` 
  *  :ref:`abs (x: float4) : float4 <function-math_abs_float4>` 
  *  :ref:`abs (x: float2) : float2 <function-math_abs_float2>` 
  *  :ref:`acos (x: float4) : float4 <function-math_acos_float4>` 
  *  :ref:`acos (x: float2) : float2 <function-math_acos_float2>` 
  *  :ref:`acos (x: float) : float <function-math_acos_float>` 
  *  :ref:`acos (x: float3) : float3 <function-math_acos_float3>` 
  *  :ref:`acos (x: double) : double <function-math_acos_double>` 
  *  :ref:`asin (x: float) : float <function-math_asin_float>` 
  *  :ref:`asin (x: double) : double <function-math_asin_double>` 
  *  :ref:`asin (x: float3) : float3 <function-math_asin_float3>` 
  *  :ref:`asin (x: float4) : float4 <function-math_asin_float4>` 
  *  :ref:`asin (x: float2) : float2 <function-math_asin_float2>` 
  *  :ref:`atan (x: float2) : float2 <function-math_atan_float2>` 
  *  :ref:`atan (x: float4) : float4 <function-math_atan_float4>` 
  *  :ref:`atan (x: double) : double <function-math_atan_double>` 
  *  :ref:`atan (x: float3) : float3 <function-math_atan_float3>` 
  *  :ref:`atan (x: float) : float <function-math_atan_float>` 
  *  :ref:`atan2 (y: float3; x: float3) : float3 <function-math_atan2_float3_float3>` 
  *  :ref:`atan2 (y: float2; x: float2) : float2 <function-math_atan2_float2_float2>` 
  *  :ref:`atan2 (y: float; x: float) : float <function-math_atan2_float_float>` 
  *  :ref:`atan2 (y: float4; x: float4) : float4 <function-math_atan2_float4_float4>` 
  *  :ref:`atan2 (y: double; x: double) : double <function-math_atan2_double_double>` 
  *  :ref:`ceil (x: float4) : float4 <function-math_ceil_float4>` 
  *  :ref:`ceil (x: float3) : float3 <function-math_ceil_float3>` 
  *  :ref:`ceil (x: float2) : float2 <function-math_ceil_float2>` 
  *  :ref:`ceil (x: float) : float <function-math_ceil_float>` 
  *  :ref:`cos (x: double) : double <function-math_cos_double>` 
  *  :ref:`cos (x: float4) : float4 <function-math_cos_float4>` 
  *  :ref:`cos (x: float3) : float3 <function-math_cos_float3>` 
  *  :ref:`cos (x: float) : float <function-math_cos_float>` 
  *  :ref:`cos (x: float2) : float2 <function-math_cos_float2>` 
  *  :ref:`exp (x: float4) : float4 <function-math_exp_float4>` 
  *  :ref:`exp (x: double) : double <function-math_exp_double>` 
  *  :ref:`exp (x: float2) : float2 <function-math_exp_float2>` 
  *  :ref:`exp (x: float) : float <function-math_exp_float>` 
  *  :ref:`exp (x: float3) : float3 <function-math_exp_float3>` 
  *  :ref:`exp2 (x: float4) : float4 <function-math_exp2_float4>` 
  *  :ref:`exp2 (x: float3) : float3 <function-math_exp2_float3>` 
  *  :ref:`exp2 (x: float2) : float2 <function-math_exp2_float2>` 
  *  :ref:`exp2 (x: double) : double <function-math_exp2_double>` 
  *  :ref:`exp2 (x: float) : float <function-math_exp2_float>` 
  *  :ref:`floor (x: float) : float <function-math_floor_float>` 
  *  :ref:`floor (x: float2) : float2 <function-math_floor_float2>` 
  *  :ref:`floor (x: float4) : float4 <function-math_floor_float4>` 
  *  :ref:`floor (x: float3) : float3 <function-math_floor_float3>` 
  *  :ref:`is_finite (x: float) : bool <function-math_is_finite_float>` 
  *  :ref:`is_finite (x: double) : bool <function-math_is_finite_double>` 
  *  :ref:`is_nan (x: float) : bool <function-math_is_nan_float>` 
  *  :ref:`is_nan (x: double) : bool <function-math_is_nan_double>` 
  *  :ref:`log (x: float4) : float4 <function-math_log_float4>` 
  *  :ref:`log (x: float3) : float3 <function-math_log_float3>` 
  *  :ref:`log (x: float2) : float2 <function-math_log_float2>` 
  *  :ref:`log (x: double) : double <function-math_log_double>` 
  *  :ref:`log (x: float) : float <function-math_log_float>` 
  *  :ref:`log2 (x: double) : double <function-math_log2_double>` 
  *  :ref:`log2 (x: float4) : float4 <function-math_log2_float4>` 
  *  :ref:`log2 (x: float3) : float3 <function-math_log2_float3>` 
  *  :ref:`log2 (x: float2) : float2 <function-math_log2_float2>` 
  *  :ref:`log2 (x: float) : float <function-math_log2_float>` 
  *  :ref:`pow (x: double; y: double) : double <function-math_pow_double_double>` 
  *  :ref:`pow (x: float4; y: float4) : float4 <function-math_pow_float4_float4>` 
  *  :ref:`pow (x: float3; y: float3) : float3 <function-math_pow_float3_float3>` 
  *  :ref:`pow (x: float; y: float) : float <function-math_pow_float_float>` 
  *  :ref:`pow (x: float2; y: float2) : float2 <function-math_pow_float2_float2>` 
  *  :ref:`rcp (x: double) : double <function-math_rcp_double>` 
  *  :ref:`rcp (x: float4) : float4 <function-math_rcp_float4>` 
  *  :ref:`rcp (x: float3) : float3 <function-math_rcp_float3>` 
  *  :ref:`rcp (x: float2) : float2 <function-math_rcp_float2>` 
  *  :ref:`rcp (x: float) : float <function-math_rcp_float>` 
  *  :ref:`safe_acos (x: double) : double <function-math_safe_acos_double>` 
  *  :ref:`safe_acos (x: float2) : float2 <function-math_safe_acos_float2>` 
  *  :ref:`safe_acos (x: float3) : float3 <function-math_safe_acos_float3>` 
  *  :ref:`safe_acos (x: float) : float <function-math_safe_acos_float>` 
  *  :ref:`safe_acos (x: float4) : float4 <function-math_safe_acos_float4>` 
  *  :ref:`safe_asin (x: float3) : float3 <function-math_safe_asin_float3>` 
  *  :ref:`safe_asin (x: float4) : float4 <function-math_safe_asin_float4>` 
  *  :ref:`safe_asin (x: double) : double <function-math_safe_asin_double>` 
  *  :ref:`safe_asin (x: float2) : float2 <function-math_safe_asin_float2>` 
  *  :ref:`safe_asin (x: float) : float <function-math_safe_asin_float>` 
  *  :ref:`saturate (x: float4) : float4 <function-math_saturate_float4>` 
  *  :ref:`saturate (x: float3) : float3 <function-math_saturate_float3>` 
  *  :ref:`saturate (x: float2) : float2 <function-math_saturate_float2>` 
  *  :ref:`saturate (x: float) : float <function-math_saturate_float>` 
  *  :ref:`sign (x: uint) : uint <function-math_sign_uint>` 
  *  :ref:`sign (x: uint2) : uint2 <function-math_sign_uint2>` 
  *  :ref:`sign (x: int4) : int4 <function-math_sign_int4>` 
  *  :ref:`sign (x: float4) : float4 <function-math_sign_float4>` 
  *  :ref:`sign (x: int64) : int64 <function-math_sign_int64>` 
  *  :ref:`sign (x: double) : double <function-math_sign_double>` 
  *  :ref:`sign (x: float3) : float3 <function-math_sign_float3>` 
  *  :ref:`sign (x: uint4) : uint4 <function-math_sign_uint4>` 
  *  :ref:`sign (x: float) : float <function-math_sign_float>` 
  *  :ref:`sign (x: float2) : float2 <function-math_sign_float2>` 
  *  :ref:`sign (x: uint64) : uint64 <function-math_sign_uint64>` 
  *  :ref:`sign (x: int3) : int3 <function-math_sign_int3>` 
  *  :ref:`sign (x: int) : int <function-math_sign_int>` 
  *  :ref:`sign (x: int2) : int2 <function-math_sign_int2>` 
  *  :ref:`sign (x: uint3) : uint3 <function-math_sign_uint3>` 
  *  :ref:`sin (x: double) : double <function-math_sin_double>` 
  *  :ref:`sin (x: float4) : float4 <function-math_sin_float4>` 
  *  :ref:`sin (x: float3) : float3 <function-math_sin_float3>` 
  *  :ref:`sin (x: float2) : float2 <function-math_sin_float2>` 
  *  :ref:`sin (x: float) : float <function-math_sin_float>` 
  *  :ref:`sincos (x: float; s: float& implicit; c: float& implicit) <function-math_sincos_float_float_implicit_float_implicit>` 
  *  :ref:`sincos (x: double; s: double& implicit; c: double& implicit) <function-math_sincos_double_double_implicit_double_implicit>` 
  *  :ref:`sqrt (x: double) : double <function-math_sqrt_double>` 
  *  :ref:`sqrt (x: float4) : float4 <function-math_sqrt_float4>` 
  *  :ref:`sqrt (x: float3) : float3 <function-math_sqrt_float3>` 
  *  :ref:`sqrt (x: float2) : float2 <function-math_sqrt_float2>` 
  *  :ref:`sqrt (x: float) : float <function-math_sqrt_float>` 
  *  :ref:`tan (x: double) : double <function-math_tan_double>` 
  *  :ref:`tan (x: float4) : float4 <function-math_tan_float4>` 
  *  :ref:`tan (x: float2) : float2 <function-math_tan_float2>` 
  *  :ref:`tan (x: float) : float <function-math_tan_float>` 
  *  :ref:`tan (x: float3) : float3 <function-math_tan_float3>` 


abs
^^^

.. _function-math_abs_float3:

.. das:function:: abs(x: float3) : float3

Returns the absolute value of the argument, computed component-wise for float2, float3, float4, int2, int3, and int4 vector types, and per-element for scalar float, double, int, and int64 types.

:Arguments: * **x** : float3

.. _function-math_abs_uint:

.. das:function:: abs(x: uint) : uint

.. _function-math_abs_uint64:

.. das:function:: abs(x: uint64) : uint64

.. _function-math_abs_int2:

.. das:function:: abs(x: int2) : int2

.. _function-math_abs_int4:

.. das:function:: abs(x: int4) : int4

.. _function-math_abs_int3:

.. das:function:: abs(x: int3) : int3

.. _function-math_abs_int64:

.. das:function:: abs(x: int64) : int64

.. _function-math_abs_uint2:

.. das:function:: abs(x: uint2) : uint2

.. _function-math_abs_int:

.. das:function:: abs(x: int) : int

.. _function-math_abs_uint4:

.. das:function:: abs(x: uint4) : uint4

.. _function-math_abs_uint3:

.. das:function:: abs(x: uint3) : uint3

.. _function-math_abs_double:

.. das:function:: abs(x: double) : double

.. _function-math_abs_float:

.. das:function:: abs(x: float) : float

.. _function-math_abs_float4:

.. das:function:: abs(x: float4) : float4

.. _function-math_abs_float2:

.. das:function:: abs(x: float2) : float2

----


acos
^^^^

.. _function-math_acos_float4:

.. das:function:: acos(x: float4) : float4

Returns the arccosine of x in radians; the input must be in the range [-1, 1] and the result is in the range [0, pi]; works with float and double.

:Arguments: * **x** : float4

.. _function-math_acos_float2:

.. das:function:: acos(x: float2) : float2

.. _function-math_acos_float:

.. das:function:: acos(x: float) : float

.. _function-math_acos_float3:

.. das:function:: acos(x: float3) : float3

.. _function-math_acos_double:

.. das:function:: acos(x: double) : double

----


asin
^^^^

.. _function-math_asin_float:

.. das:function:: asin(x: float) : float

Returns the arcsine of x in radians; the input must be in the range [-1, 1] and the result is in the range [-pi/2, pi/2]; works with float and double.

:Arguments: * **x** : float

.. _function-math_asin_double:

.. das:function:: asin(x: double) : double

.. _function-math_asin_float3:

.. das:function:: asin(x: float3) : float3

.. _function-math_asin_float4:

.. das:function:: asin(x: float4) : float4

.. _function-math_asin_float2:

.. das:function:: asin(x: float2) : float2

----


atan
^^^^

.. _function-math_atan_float2:

.. das:function:: atan(x: float2) : float2

Returns the arctangent of x in radians, with the result in the range [-pi/2, pi/2]; works with float and double.

:Arguments: * **x** : float2

.. _function-math_atan_float4:

.. das:function:: atan(x: float4) : float4

.. _function-math_atan_double:

.. das:function:: atan(x: double) : double

.. _function-math_atan_float3:

.. das:function:: atan(x: float3) : float3

.. _function-math_atan_float:

.. das:function:: atan(x: float) : float

----


atan2
^^^^^

.. _function-math_atan2_float3_float3:

.. das:function:: atan2(y: float3; x: float3) : float3

Returns the arctangent of y/x in radians, using the signs of both arguments to determine the correct quadrant; the result is in the range [-pi, pi]; works with float and double.

:Arguments: * **y** : float3

            * **x** : float3

.. _function-math_atan2_float2_float2:

.. das:function:: atan2(y: float2; x: float2) : float2

.. _function-math_atan2_float_float:

.. das:function:: atan2(y: float; x: float) : float

.. _function-math_atan2_float4_float4:

.. das:function:: atan2(y: float4; x: float4) : float4

.. _function-math_atan2_double_double:

.. das:function:: atan2(y: double; x: double) : double

----


ceil
^^^^

.. _function-math_ceil_float4:

.. das:function:: ceil(x: float4) : float4

Returns the smallest integral value not less than x (rounds toward positive infinity), computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float4

.. _function-math_ceil_float3:

.. das:function:: ceil(x: float3) : float3

.. _function-math_ceil_float2:

.. das:function:: ceil(x: float2) : float2

.. _function-math_ceil_float:

.. das:function:: ceil(x: float) : float

----


cos
^^^

.. _function-math_cos_double:

.. das:function:: cos(x: double) : double

Returns the cosine of x, where x is specified in radians; works with float and double.

:Arguments: * **x** : double

.. _function-math_cos_float4:

.. das:function:: cos(x: float4) : float4

.. _function-math_cos_float3:

.. das:function:: cos(x: float3) : float3

.. _function-math_cos_float:

.. das:function:: cos(x: float) : float

.. _function-math_cos_float2:

.. das:function:: cos(x: float2) : float2

----


exp
^^^

.. _function-math_exp_float4:

.. das:function:: exp(x: float4) : float4

Returns e raised to the power of x (the base-e exponential), computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float4

.. _function-math_exp_double:

.. das:function:: exp(x: double) : double

.. _function-math_exp_float2:

.. das:function:: exp(x: float2) : float2

.. _function-math_exp_float:

.. das:function:: exp(x: float) : float

.. _function-math_exp_float3:

.. das:function:: exp(x: float3) : float3

----


exp2
^^^^

.. _function-math_exp2_float4:

.. das:function:: exp2(x: float4) : float4

Returns 2 raised to the power of x, computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float4

.. _function-math_exp2_float3:

.. das:function:: exp2(x: float3) : float3

.. _function-math_exp2_float2:

.. das:function:: exp2(x: float2) : float2

.. _function-math_exp2_double:

.. das:function:: exp2(x: double) : double

.. _function-math_exp2_float:

.. das:function:: exp2(x: float) : float

----


floor
^^^^^

.. _function-math_floor_float:

.. das:function:: floor(x: float) : float

Returns the largest integral value not greater than x (rounds toward negative infinity), computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float

.. _function-math_floor_float2:

.. das:function:: floor(x: float2) : float2

.. _function-math_floor_float4:

.. das:function:: floor(x: float4) : float4

.. _function-math_floor_float3:

.. das:function:: floor(x: float3) : float3

----


is_finite
^^^^^^^^^

.. _function-math_is_finite_float:

.. das:function:: is_finite(x: float) : bool

Returns true if x is a finite value (not infinity and not NaN), checked component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float

.. _function-math_is_finite_double:

.. das:function:: is_finite(x: double) : bool

----


is_nan
^^^^^^

.. _function-math_is_nan_float:

.. das:function:: is_nan(x: float) : bool

Returns true if x is NaN (Not a Number), checked component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float

.. _function-math_is_nan_double:

.. das:function:: is_nan(x: double) : bool

----


log
^^^

.. _function-math_log_float4:

.. das:function:: log(x: float4) : float4

Returns the natural (base-e) logarithm of x; the input must be positive; computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float4

.. _function-math_log_float3:

.. das:function:: log(x: float3) : float3

.. _function-math_log_float2:

.. das:function:: log(x: float2) : float2

.. _function-math_log_double:

.. das:function:: log(x: double) : double

.. _function-math_log_float:

.. das:function:: log(x: float) : float

----


log2
^^^^

.. _function-math_log2_double:

.. das:function:: log2(x: double) : double

Returns the base-2 logarithm of x; the input must be positive; computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : double

.. _function-math_log2_float4:

.. das:function:: log2(x: float4) : float4

.. _function-math_log2_float3:

.. das:function:: log2(x: float3) : float3

.. _function-math_log2_float2:

.. das:function:: log2(x: float2) : float2

.. _function-math_log2_float:

.. das:function:: log2(x: float) : float

----


pow
^^^

.. _function-math_pow_double_double:

.. das:function:: pow(x: double; y: double) : double

Returns x raised to the power of y for scalar double, float, or vector float2, float3, float4 types; domain requires x >= 0 for non-integer y values.

:Arguments: * **x** : double

            * **y** : double

.. _function-math_pow_float4_float4:

.. das:function:: pow(x: float4; y: float4) : float4

.. _function-math_pow_float3_float3:

.. das:function:: pow(x: float3; y: float3) : float3

.. _function-math_pow_float_float:

.. das:function:: pow(x: float; y: float) : float

.. _function-math_pow_float2_float2:

.. das:function:: pow(x: float2; y: float2) : float2

----


rcp
^^^

.. _function-math_rcp_double:

.. das:function:: rcp(x: double) : double

Returns the reciprocal (1/x) of a scalar float or each component of a float2, float3, or float4 vector.

:Arguments: * **x** : double

.. _function-math_rcp_float4:

.. das:function:: rcp(x: float4) : float4

.. _function-math_rcp_float3:

.. das:function:: rcp(x: float3) : float3

.. _function-math_rcp_float2:

.. das:function:: rcp(x: float2) : float2

.. _function-math_rcp_float:

.. das:function:: rcp(x: float) : float

----


safe_acos
^^^^^^^^^

.. _function-math_safe_acos_double:

.. das:function:: safe_acos(x: double) : double

Returns the arccosine of x in radians, clamping the input to the valid domain [-1, 1] to prevent NaN results from out-of-range values.

:Arguments: * **x** : double

.. _function-math_safe_acos_float2:

.. das:function:: safe_acos(x: float2) : float2

.. _function-math_safe_acos_float3:

.. das:function:: safe_acos(x: float3) : float3

.. _function-math_safe_acos_float:

.. das:function:: safe_acos(x: float) : float

.. _function-math_safe_acos_float4:

.. das:function:: safe_acos(x: float4) : float4

----


safe_asin
^^^^^^^^^

.. _function-math_safe_asin_float3:

.. das:function:: safe_asin(x: float3) : float3

Returns the arcsine of x in radians, clamping the input to the valid domain [-1, 1] to prevent NaN results from out-of-range values.

:Arguments: * **x** : float3

.. _function-math_safe_asin_float4:

.. das:function:: safe_asin(x: float4) : float4

.. _function-math_safe_asin_double:

.. das:function:: safe_asin(x: double) : double

.. _function-math_safe_asin_float2:

.. das:function:: safe_asin(x: float2) : float2

.. _function-math_safe_asin_float:

.. das:function:: safe_asin(x: float) : float

----


saturate
^^^^^^^^

.. _function-math_saturate_float4:

.. das:function:: saturate(x: float4) : float4

Clamps the scalar double, float, or each component of a float2, float3, float4 vector to the [0, 1] range, returning 0 for values below 0 and 1 for values above 1.

:Arguments: * **x** : float4

.. _function-math_saturate_float3:

.. das:function:: saturate(x: float3) : float3

.. _function-math_saturate_float2:

.. das:function:: saturate(x: float2) : float2

.. _function-math_saturate_float:

.. das:function:: saturate(x: float) : float

----


sign
^^^^

.. _function-math_sign_uint:

.. das:function:: sign(x: uint) : uint

Returns the sign of x component-wise: -1 for negative, 0 for zero, or 1 for positive. For unsigned types, the result is 0 or 1.

:Arguments: * **x** : uint

.. _function-math_sign_uint2:

.. das:function:: sign(x: uint2) : uint2

.. _function-math_sign_int4:

.. das:function:: sign(x: int4) : int4

.. _function-math_sign_float4:

.. das:function:: sign(x: float4) : float4

.. _function-math_sign_int64:

.. das:function:: sign(x: int64) : int64

.. _function-math_sign_double:

.. das:function:: sign(x: double) : double

.. _function-math_sign_float3:

.. das:function:: sign(x: float3) : float3

.. _function-math_sign_uint4:

.. das:function:: sign(x: uint4) : uint4

.. _function-math_sign_float:

.. das:function:: sign(x: float) : float

.. _function-math_sign_float2:

.. das:function:: sign(x: float2) : float2

.. _function-math_sign_uint64:

.. das:function:: sign(x: uint64) : uint64

.. _function-math_sign_int3:

.. das:function:: sign(x: int3) : int3

.. _function-math_sign_int:

.. das:function:: sign(x: int) : int

.. _function-math_sign_int2:

.. das:function:: sign(x: int2) : int2

.. _function-math_sign_uint3:

.. das:function:: sign(x: uint3) : uint3

----


sin
^^^

.. _function-math_sin_double:

.. das:function:: sin(x: double) : double

Returns the sine of the angle x given in radians for double or float, with output in the range [-1, 1].

:Arguments: * **x** : double

.. _function-math_sin_float4:

.. das:function:: sin(x: float4) : float4

.. _function-math_sin_float3:

.. das:function:: sin(x: float3) : float3

.. _function-math_sin_float2:

.. das:function:: sin(x: float2) : float2

.. _function-math_sin_float:

.. das:function:: sin(x: float) : float

----


sincos
^^^^^^

.. _function-math_sincos_float_float_implicit_float_implicit:

.. das:function:: sincos(x: float; s: float& implicit; c: float& implicit)

Computes both the sine and cosine of the angle x in radians simultaneously, writing the results to output parameters s and c, for float or double types.

:Arguments: * **x** : float

            * **s** : float& implicit

            * **c** : float& implicit

.. _function-math_sincos_double_double_implicit_double_implicit:

.. das:function:: sincos(x: double; s: double& implicit; c: double& implicit)

----


sqrt
^^^^

.. _function-math_sqrt_double:

.. das:function:: sqrt(x: double) : double

Returns the square root of a scalar double, float, or each component of a float2, float3, or float4 vector; input must be non-negative.

:Arguments: * **x** : double

.. _function-math_sqrt_float4:

.. das:function:: sqrt(x: float4) : float4

.. _function-math_sqrt_float3:

.. das:function:: sqrt(x: float3) : float3

.. _function-math_sqrt_float2:

.. das:function:: sqrt(x: float2) : float2

.. _function-math_sqrt_float:

.. das:function:: sqrt(x: float) : float

----


tan
^^^

.. _function-math_tan_double:

.. das:function:: tan(x: double) : double

Returns the tangent of the angle x given in radians for double or float; undefined at odd multiples of pi/2.

:Arguments: * **x** : double

.. _function-math_tan_float4:

.. das:function:: tan(x: float4) : float4

.. _function-math_tan_float2:

.. das:function:: tan(x: float2) : float2

.. _function-math_tan_float:

.. das:function:: tan(x: float) : float

.. _function-math_tan_float3:

.. das:function:: tan(x: float3) : float3

----

+++++++++++
float* only
+++++++++++

  *  :ref:`atan2_est (y: float; x: float) : float <function-math_atan2_est_float_float>` 
  *  :ref:`atan2_est (y: float4; x: float4) : float4 <function-math_atan2_est_float4_float4>` 
  *  :ref:`atan2_est (y: float3; x: float3) : float3 <function-math_atan2_est_float3_float3>` 
  *  :ref:`atan2_est (y: float2; x: float2) : float2 <function-math_atan2_est_float2_float2>` 
  *  :ref:`atan_est (x: float2) : float2 <function-math_atan_est_float2>` 
  *  :ref:`atan_est (x: float4) : float4 <function-math_atan_est_float4>` 
  *  :ref:`atan_est (x: float) : float <function-math_atan_est_float>` 
  *  :ref:`atan_est (x: float3) : float3 <function-math_atan_est_float3>` 
  *  :ref:`ceili (x: float4) : int4 <function-math_ceili_float4>` 
  *  :ref:`ceili (x: float) : int <function-math_ceili_float>` 
  *  :ref:`ceili (x: double) : int <function-math_ceili_double>` 
  *  :ref:`ceili (x: float3) : int3 <function-math_ceili_float3>` 
  *  :ref:`ceili (x: float2) : int2 <function-math_ceili_float2>` 
  *  :ref:`float3x3 implicit- (x: float3x3 implicit) : float3x3 <function-math_-_float3x3_implicit>` 
  *  :ref:`float3x4 implicit- (x: float3x4 implicit) : float3x4 <function-math_-_float3x4_implicit>` 
  *  :ref:`float4x4 implicit- (x: float4x4 implicit) : float4x4 <function-math_-_float4x4_implicit>` 
  *  :ref:`floori (x: float) : int <function-math_floori_float>` 
  *  :ref:`floori (x: float2) : int2 <function-math_floori_float2>` 
  *  :ref:`floori (x: float4) : int4 <function-math_floori_float4>` 
  *  :ref:`floori (x: double) : int <function-math_floori_double>` 
  *  :ref:`floori (x: float3) : int3 <function-math_floori_float3>` 
  *  :ref:`fract (x: float3) : float3 <function-math_fract_float3>` 
  *  :ref:`fract (x: float4) : float4 <function-math_fract_float4>` 
  *  :ref:`fract (x: float2) : float2 <function-math_fract_float2>` 
  *  :ref:`fract (x: float) : float <function-math_fract_float>` 
  *  :ref:`rcp_est (x: float4) : float4 <function-math_rcp_est_float4>` 
  *  :ref:`rcp_est (x: float2) : float2 <function-math_rcp_est_float2>` 
  *  :ref:`rcp_est (x: float) : float <function-math_rcp_est_float>` 
  *  :ref:`rcp_est (x: float3) : float3 <function-math_rcp_est_float3>` 
  *  :ref:`round (x: float4) : float4 <function-math_round_float4>` 
  *  :ref:`round (x: float3) : float3 <function-math_round_float3>` 
  *  :ref:`round (x: float2) : float2 <function-math_round_float2>` 
  *  :ref:`round (x: float) : float <function-math_round_float>` 
  *  :ref:`roundi (x: float4) : int4 <function-math_roundi_float4>` 
  *  :ref:`roundi (x: float2) : int2 <function-math_roundi_float2>` 
  *  :ref:`roundi (x: float3) : int3 <function-math_roundi_float3>` 
  *  :ref:`roundi (x: double) : int <function-math_roundi_double>` 
  *  :ref:`roundi (x: float) : int <function-math_roundi_float>` 
  *  :ref:`rsqrt (x: float4) : float4 <function-math_rsqrt_float4>` 
  *  :ref:`rsqrt (x: float2) : float2 <function-math_rsqrt_float2>` 
  *  :ref:`rsqrt (x: float) : float <function-math_rsqrt_float>` 
  *  :ref:`rsqrt (x: float3) : float3 <function-math_rsqrt_float3>` 
  *  :ref:`rsqrt_est (x: float3) : float3 <function-math_rsqrt_est_float3>` 
  *  :ref:`rsqrt_est (x: float4) : float4 <function-math_rsqrt_est_float4>` 
  *  :ref:`rsqrt_est (x: float2) : float2 <function-math_rsqrt_est_float2>` 
  *  :ref:`rsqrt_est (x: float) : float <function-math_rsqrt_est_float>` 
  *  :ref:`trunci (x: double) : int <function-math_trunci_double>` 
  *  :ref:`trunci (x: float) : int <function-math_trunci_float>` 
  *  :ref:`trunci (x: float2) : int2 <function-math_trunci_float2>` 
  *  :ref:`trunci (x: float4) : int4 <function-math_trunci_float4>` 
  *  :ref:`trunci (x: float3) : int3 <function-math_trunci_float3>` 


atan2_est
^^^^^^^^^

.. _function-math_atan2_est_float_float:

.. das:function:: atan2_est(y: float; x: float) : float

Returns a fast estimated arctangent of y/x in radians, using the signs of both arguments to determine the correct quadrant; trades some precision for speed.

:Arguments: * **y** : float

            * **x** : float

.. _function-math_atan2_est_float4_float4:

.. das:function:: atan2_est(y: float4; x: float4) : float4

.. _function-math_atan2_est_float3_float3:

.. das:function:: atan2_est(y: float3; x: float3) : float3

.. _function-math_atan2_est_float2_float2:

.. das:function:: atan2_est(y: float2; x: float2) : float2

----


atan_est
^^^^^^^^

.. _function-math_atan_est_float2:

.. das:function:: atan_est(x: float2) : float2

Returns a fast estimated arctangent of x in radians, trading some precision for speed; the result approximates the range [-pi/2, pi/2].

:Arguments: * **x** : float2

.. _function-math_atan_est_float4:

.. das:function:: atan_est(x: float4) : float4

.. _function-math_atan_est_float:

.. das:function:: atan_est(x: float) : float

.. _function-math_atan_est_float3:

.. das:function:: atan_est(x: float3) : float3

----


ceili
^^^^^

.. _function-math_ceili_float4:

.. das:function:: ceili(x: float4) : int4

Returns the smallest integer not less than x (rounds toward positive infinity), converting the float argument to an int result.

:Arguments: * **x** : float4

.. _function-math_ceili_float:

.. das:function:: ceili(x: float) : int

.. _function-math_ceili_double:

.. das:function:: ceili(x: double) : int

.. _function-math_ceili_float3:

.. das:function:: ceili(x: float3) : int3

.. _function-math_ceili_float2:

.. das:function:: ceili(x: float2) : int2

----

.. _function-math_-_float3x3_implicit:

.. das:function:: float3x3 implicit-(x: float3x3 implicit) : float3x3

Returns the component-wise arithmetic negation of a matrix, flipping the sign of every element; works with float3x3, float3x4, and float4x4 matrix types.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math_-_float3x4_implicit:

.. das:function:: float3x4 implicit-(x: float3x4 implicit) : float3x4

Returns the component-wise arithmetic negation of a matrix, flipping the sign of every element; works with float3x3, float3x4, and float4x4 matrix types.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_-_float4x4_implicit:

.. das:function:: float4x4 implicit-(x: float4x4 implicit) : float4x4

Returns the component-wise arithmetic negation of a matrix, flipping the sign of every element; works with float3x3, float3x4, and float4x4 matrix types.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit


floori
^^^^^^

.. _function-math_floori_float:

.. das:function:: floori(x: float) : int

Returns the largest integer not greater than x (rounds toward negative infinity), converting the float argument to an int result.

:Arguments: * **x** : float

.. _function-math_floori_float2:

.. das:function:: floori(x: float2) : int2

.. _function-math_floori_float4:

.. das:function:: floori(x: float4) : int4

.. _function-math_floori_double:

.. das:function:: floori(x: double) : int

.. _function-math_floori_float3:

.. das:function:: floori(x: float3) : int3

----


fract
^^^^^

.. _function-math_fract_float3:

.. das:function:: fract(x: float3) : float3

Returns the fractional part of x (equivalent to x - floor(x)), computed component-wise for float2, float3, and float4 vector types; works with float and double scalars.

:Arguments: * **x** : float3

.. _function-math_fract_float4:

.. das:function:: fract(x: float4) : float4

.. _function-math_fract_float2:

.. das:function:: fract(x: float2) : float2

.. _function-math_fract_float:

.. das:function:: fract(x: float) : float

----


rcp_est
^^^^^^^

.. _function-math_rcp_est_float4:

.. das:function:: rcp_est(x: float4) : float4

Returns a fast hardware estimate of the reciprocal (1/x) of a scalar float or each component of a float2, float3, or float4 vector, trading precision for speed.

:Arguments: * **x** : float4

.. _function-math_rcp_est_float2:

.. das:function:: rcp_est(x: float2) : float2

.. _function-math_rcp_est_float:

.. das:function:: rcp_est(x: float) : float

.. _function-math_rcp_est_float3:

.. das:function:: rcp_est(x: float3) : float3

----


round
^^^^^

.. _function-math_round_float4:

.. das:function:: round(x: float4) : float4

Rounds each component of the scalar double, float, or vector float2, float3, float4 value x to the nearest integer, with halfway cases rounded to the nearest even value.

:Arguments: * **x** : float4

.. _function-math_round_float3:

.. das:function:: round(x: float3) : float3

.. _function-math_round_float2:

.. das:function:: round(x: float2) : float2

.. _function-math_round_float:

.. das:function:: round(x: float) : float

----


roundi
^^^^^^

.. _function-math_roundi_float4:

.. das:function:: roundi(x: float4) : int4

Rounds the float x to the nearest integer value and returns the result as an int.

:Arguments: * **x** : float4

.. _function-math_roundi_float2:

.. das:function:: roundi(x: float2) : int2

.. _function-math_roundi_float3:

.. das:function:: roundi(x: float3) : int3

.. _function-math_roundi_double:

.. das:function:: roundi(x: double) : int

.. _function-math_roundi_float:

.. das:function:: roundi(x: float) : int

----


rsqrt
^^^^^

.. _function-math_rsqrt_float4:

.. das:function:: rsqrt(x: float4) : float4

Returns the reciprocal square root (1/sqrt(x)) of a scalar float or each component of a float2, float3, or float4 vector.

:Arguments: * **x** : float4

.. _function-math_rsqrt_float2:

.. das:function:: rsqrt(x: float2) : float2

.. _function-math_rsqrt_float:

.. das:function:: rsqrt(x: float) : float

.. _function-math_rsqrt_float3:

.. das:function:: rsqrt(x: float3) : float3

----


rsqrt_est
^^^^^^^^^

.. _function-math_rsqrt_est_float3:

.. das:function:: rsqrt_est(x: float3) : float3

Returns a fast hardware estimate of the reciprocal square root (1/sqrt(x)) of a scalar float or each component of a float2, float3, or float4 vector, trading precision for speed.

:Arguments: * **x** : float3

.. _function-math_rsqrt_est_float4:

.. das:function:: rsqrt_est(x: float4) : float4

.. _function-math_rsqrt_est_float2:

.. das:function:: rsqrt_est(x: float2) : float2

.. _function-math_rsqrt_est_float:

.. das:function:: rsqrt_est(x: float) : float

----


trunci
^^^^^^

.. _function-math_trunci_double:

.. das:function:: trunci(x: double) : int

Truncates the float x toward zero to the nearest integer and returns the result as an int.

:Arguments: * **x** : double

.. _function-math_trunci_float:

.. das:function:: trunci(x: float) : int

.. _function-math_trunci_float2:

.. das:function:: trunci(x: float2) : int2

.. _function-math_trunci_float4:

.. das:function:: trunci(x: float4) : int4

.. _function-math_trunci_float3:

.. das:function:: trunci(x: float3) : int3

----

+++++++++++
float3 only
+++++++++++

  *  :ref:`cross (x: float3; y: float3) : float3 <function-math_cross_float3_float3>` 
  *  :ref:`distance (x: float2; y: float2) : float <function-math_distance_float2_float2>` 
  *  :ref:`distance (x: float4; y: float4) : float <function-math_distance_float4_float4>` 
  *  :ref:`distance (x: float3; y: float3) : float <function-math_distance_float3_float3>` 
  *  :ref:`distance_sq (x: float3; y: float3) : float <function-math_distance_sq_float3_float3>` 
  *  :ref:`distance_sq (x: float2; y: float2) : float <function-math_distance_sq_float2_float2>` 
  *  :ref:`distance_sq (x: float4; y: float4) : float <function-math_distance_sq_float4_float4>` 
  *  :ref:`inv_distance (x: float3; y: float3) : float <function-math_inv_distance_float3_float3>` 
  *  :ref:`inv_distance (x: float2; y: float2) : float <function-math_inv_distance_float2_float2>` 
  *  :ref:`inv_distance (x: float4; y: float4) : float <function-math_inv_distance_float4_float4>` 
  *  :ref:`inv_distance_sq (x: float4; y: float4) : float <function-math_inv_distance_sq_float4_float4>` 
  *  :ref:`inv_distance_sq (x: float2; y: float2) : float <function-math_inv_distance_sq_float2_float2>` 
  *  :ref:`inv_distance_sq (x: float3; y: float3) : float <function-math_inv_distance_sq_float3_float3>` 
  *  :ref:`reflect (v: float3; n: float3) : float3 <function-math_reflect_float3_float3>` 
  *  :ref:`reflect (v: float2; n: float2) : float2 <function-math_reflect_float2_float2>` 
  *  :ref:`refract (v: float2; n: float2; nint: float) : float2 <function-math_refract_float2_float2_float>` 
  *  :ref:`refract (v: float3; n: float3; nint: float) : float3 <function-math_refract_float3_float3_float>` 

.. _function-math_cross_float3_float3:

.. das:function:: cross(x: float3; y: float3) : float3

Returns the cross product of two float3 vectors, producing a float3 vector perpendicular to both inputs with magnitude equal to the area of the parallelogram they span.

:Arguments: * **x** : float3

            * **y** : float3


distance
^^^^^^^^

.. _function-math_distance_float2_float2:

.. das:function:: distance(x: float2; y: float2) : float

Returns the Euclidean distance between two vectors as a float scalar; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_distance_float4_float4:

.. das:function:: distance(x: float4; y: float4) : float

.. _function-math_distance_float3_float3:

.. das:function:: distance(x: float3; y: float3) : float

----


distance_sq
^^^^^^^^^^^

.. _function-math_distance_sq_float3_float3:

.. das:function:: distance_sq(x: float3; y: float3) : float

Returns the squared Euclidean distance between two vectors as a float scalar, avoiding the square root for faster distance comparisons; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_distance_sq_float2_float2:

.. das:function:: distance_sq(x: float2; y: float2) : float

.. _function-math_distance_sq_float4_float4:

.. das:function:: distance_sq(x: float4; y: float4) : float

----


inv_distance
^^^^^^^^^^^^

.. _function-math_inv_distance_float3_float3:

.. das:function:: inv_distance(x: float3; y: float3) : float

Returns the reciprocal of the Euclidean distance between two vectors (1 / distance(x, y)) as a float; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_inv_distance_float2_float2:

.. das:function:: inv_distance(x: float2; y: float2) : float

.. _function-math_inv_distance_float4_float4:

.. das:function:: inv_distance(x: float4; y: float4) : float

----


inv_distance_sq
^^^^^^^^^^^^^^^

.. _function-math_inv_distance_sq_float4_float4:

.. das:function:: inv_distance_sq(x: float4; y: float4) : float

Returns the reciprocal of the squared Euclidean distance between two vectors (1 / distance_sq(x, y)) as a float; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_inv_distance_sq_float2_float2:

.. das:function:: inv_distance_sq(x: float2; y: float2) : float

.. _function-math_inv_distance_sq_float3_float3:

.. das:function:: inv_distance_sq(x: float3; y: float3) : float

----


reflect
^^^^^^^

.. _function-math_reflect_float3_float3:

.. das:function:: reflect(v: float3; n: float3) : float3

Computes the reflection of float2 or float3 vector v off a surface with unit normal n, returning the reflected vector as v - 2*dot(v,n)*n.

:Arguments: * **v** : float3

            * **n** : float3

.. _function-math_reflect_float2_float2:

.. das:function:: reflect(v: float2; n: float2) : float2

----


refract
^^^^^^^

.. _function-math_refract_float2_float2_float:

.. das:function:: refract(v: float2; n: float2; nint: float) : float2

Computes the refraction direction of vector v through a surface with unit normal n using Snell's law with index of refraction ratio nint. Returns a zero vector if total internal reflection occurs.

:Arguments: * **v** : float2

            * **n** : float2

            * **nint** : float

.. _function-math_refract_float3_float3_float:

.. das:function:: refract(v: float3; n: float3; nint: float) : float3

----

++++++++++++++++++++++
float2, float3, float4
++++++++++++++++++++++

  *  :ref:`dot (x: float3; y: float3) : float <function-math_dot_float3_float3>` 
  *  :ref:`dot (x: float2; y: float2) : float <function-math_dot_float2_float2>` 
  *  :ref:`dot (x: float4; y: float4) : float <function-math_dot_float4_float4>` 
  *  :ref:`fast_normalize (x: float2) : float2 <function-math_fast_normalize_float2>` 
  *  :ref:`fast_normalize (x: float4) : float4 <function-math_fast_normalize_float4>` 
  *  :ref:`fast_normalize (x: float3) : float3 <function-math_fast_normalize_float3>` 
  *  :ref:`inv_length (x: float4) : float <function-math_inv_length_float4>` 
  *  :ref:`inv_length (x: float3) : float <function-math_inv_length_float3>` 
  *  :ref:`inv_length (x: float2) : float <function-math_inv_length_float2>` 
  *  :ref:`inv_length_sq (x: float4) : float <function-math_inv_length_sq_float4>` 
  *  :ref:`inv_length_sq (x: float2) : float <function-math_inv_length_sq_float2>` 
  *  :ref:`inv_length_sq (x: float3) : float <function-math_inv_length_sq_float3>` 
  *  :ref:`length (x: float3) : float <function-math_length_float3>` 
  *  :ref:`length (x: float2) : float <function-math_length_float2>` 
  *  :ref:`length (x: float4) : float <function-math_length_float4>` 
  *  :ref:`length_sq (x: float3) : float <function-math_length_sq_float3>` 
  *  :ref:`length_sq (x: float2) : float <function-math_length_sq_float2>` 
  *  :ref:`length_sq (x: float4) : float <function-math_length_sq_float4>` 
  *  :ref:`normalize (x: float2) : float2 <function-math_normalize_float2>` 
  *  :ref:`normalize (x: float3) : float3 <function-math_normalize_float3>` 
  *  :ref:`normalize (x: float4) : float4 <function-math_normalize_float4>` 


dot
^^^

.. _function-math_dot_float3_float3:

.. das:function:: dot(x: float3; y: float3) : float

Returns the dot product (scalar product) of two vectors as a float; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_dot_float2_float2:

.. das:function:: dot(x: float2; y: float2) : float

.. _function-math_dot_float4_float4:

.. das:function:: dot(x: float4; y: float4) : float

----


fast_normalize
^^^^^^^^^^^^^^

.. _function-math_fast_normalize_float2:

.. das:function:: fast_normalize(x: float2) : float2

Returns a unit-length vector in the same direction as x using a fast approximation; does not check for zero-length input; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float2

.. _function-math_fast_normalize_float4:

.. das:function:: fast_normalize(x: float4) : float4

.. _function-math_fast_normalize_float3:

.. das:function:: fast_normalize(x: float3) : float3

----


inv_length
^^^^^^^^^^

.. _function-math_inv_length_float4:

.. das:function:: inv_length(x: float4) : float

Returns the reciprocal of the length of the vector (1 / length(x)) as a float; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float4

.. _function-math_inv_length_float3:

.. das:function:: inv_length(x: float3) : float

.. _function-math_inv_length_float2:

.. das:function:: inv_length(x: float2) : float

----


inv_length_sq
^^^^^^^^^^^^^

.. _function-math_inv_length_sq_float4:

.. das:function:: inv_length_sq(x: float4) : float

Returns the reciprocal of the squared length of the vector (1 / length_sq(x)) as a float; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float4

.. _function-math_inv_length_sq_float2:

.. das:function:: inv_length_sq(x: float2) : float

.. _function-math_inv_length_sq_float3:

.. das:function:: inv_length_sq(x: float3) : float

----


length
^^^^^^

.. _function-math_length_float3:

.. das:function:: length(x: float3) : float

Returns the Euclidean length (magnitude) of the vector as a float; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float3

.. _function-math_length_float2:

.. das:function:: length(x: float2) : float

.. _function-math_length_float4:

.. das:function:: length(x: float4) : float

----


length_sq
^^^^^^^^^

.. _function-math_length_sq_float3:

.. das:function:: length_sq(x: float3) : float

Returns the squared Euclidean length of the vector as a float, equivalent to dot(x, x) and avoiding the square root for faster magnitude comparisons; works with float2, float3, and float4 vector types.

:Arguments: * **x** : float3

.. _function-math_length_sq_float2:

.. das:function:: length_sq(x: float2) : float

.. _function-math_length_sq_float4:

.. das:function:: length_sq(x: float4) : float

----


normalize
^^^^^^^^^

.. _function-math_normalize_float2:

.. das:function:: normalize(x: float2) : float2

Returns a unit-length vector with the same direction as the input float2, float3, or float4 vector; behavior is undefined if the input vector has zero length.

:Arguments: * **x** : float2

.. _function-math_normalize_float3:

.. das:function:: normalize(x: float3) : float3

.. _function-math_normalize_float4:

.. das:function:: normalize(x: float4) : float4

----

+++++++++++++++
Noise functions
+++++++++++++++

  *  :ref:`uint32_hash (seed: uint) : uint <function-math_uint32_hash_uint>` 
  *  :ref:`uint_noise_1D (position: int; seed: uint) : uint <function-math_uint_noise_1D_int_uint>` 
  *  :ref:`uint_noise_2D (position: int2; seed: uint) : uint <function-math_uint_noise_2D_int2_uint>` 
  *  :ref:`uint_noise_3D (position: int3; seed: uint) : uint <function-math_uint_noise_3D_int3_uint>` 

.. _function-math_uint32_hash_uint:

.. das:function:: uint32_hash(seed: uint) : uint

Returns a well-distributed uint hash of the input uint seed using an improved integer hash function suitable for hash tables and procedural generation.

:Arguments: * **seed** : uint

.. _function-math_uint_noise_1D_int_uint:

.. das:function:: uint_noise_1D(position: int; seed: uint) : uint

Generates a deterministic uint hash value from a 1D integer position and a uint seed, suitable for repeatable procedural noise.

:Arguments: * **position** : int

            * **seed** : uint

.. _function-math_uint_noise_2D_int2_uint:

.. das:function:: uint_noise_2D(position: int2; seed: uint) : uint

Generates a deterministic uint hash value from 2D integer coordinates (x, y) and a uint seed, suitable for repeatable procedural noise.

:Arguments: * **position** : int2

            * **seed** : uint

.. _function-math_uint_noise_3D_int3_uint:

.. das:function:: uint_noise_3D(position: int3; seed: uint) : uint

Generates a deterministic uint hash value from 3D integer coordinates (x, y, z) and a uint seed, suitable for repeatable procedural noise.

:Arguments: * **position** : int3

            * **seed** : uint

++++++++++++++
lerp/mad/clamp
++++++++++++++

  *  :ref:`clamp (t: float; a: float; b: float) : float <function-math_clamp_float_float_float>` 
  *  :ref:`clamp (t: int64; a: int64; b: int64) : int64 <function-math_clamp_int64_int64_int64>` 
  *  :ref:`clamp (t: int3; a: int3; b: int3) : int3 <function-math_clamp_int3_int3_int3>` 
  *  :ref:`clamp (t: uint3; a: uint3; b: uint3) : uint3 <function-math_clamp_uint3_uint3_uint3>` 
  *  :ref:`clamp (t: int; a: int; b: int) : int <function-math_clamp_int_int_int>` 
  *  :ref:`clamp (t: uint64; a: uint64; b: uint64) : uint64 <function-math_clamp_uint64_uint64_uint64>` 
  *  :ref:`clamp (t: int4; a: int4; b: int4) : int4 <function-math_clamp_int4_int4_int4>` 
  *  :ref:`clamp (t: int2; a: int2; b: int2) : int2 <function-math_clamp_int2_int2_int2>` 
  *  :ref:`clamp (t: float4; a: float4; b: float4) : float4 <function-math_clamp_float4_float4_float4>` 
  *  :ref:`clamp (t: float2; a: float2; b: float2) : float2 <function-math_clamp_float2_float2_float2>` 
  *  :ref:`clamp (t: uint4; a: uint4; b: uint4) : uint4 <function-math_clamp_uint4_uint4_uint4>` 
  *  :ref:`clamp (t: float3; a: float3; b: float3) : float3 <function-math_clamp_float3_float3_float3>` 
  *  :ref:`clamp (t: double; a: double; b: double) : double <function-math_clamp_double_double_double>` 
  *  :ref:`clamp (t: uint; a: uint; b: uint) : uint <function-math_clamp_uint_uint_uint>` 
  *  :ref:`clamp (t: uint2; a: uint2; b: uint2) : uint2 <function-math_clamp_uint2_uint2_uint2>` 
  *  :ref:`lerp (a: double; b: double; t: double) : double <function-math_lerp_double_double_double>` 
  *  :ref:`lerp (a: float4; b: float4; t: float4) : float4 <function-math_lerp_float4_float4_float4>` 
  *  :ref:`lerp (a: float3; b: float3; t: float3) : float3 <function-math_lerp_float3_float3_float3>` 
  *  :ref:`lerp (a: float2; b: float2; t: float2) : float2 <function-math_lerp_float2_float2_float2>` 
  *  :ref:`lerp (a: float; b: float; t: float) : float <function-math_lerp_float_float_float>` 
  *  :ref:`lerp (a: float3; b: float3; t: float) : float3 <function-math_lerp_float3_float3_float>` 
  *  :ref:`lerp (a: float4; b: float4; t: float) : float4 <function-math_lerp_float4_float4_float>` 
  *  :ref:`lerp (a: float2; b: float2; t: float) : float2 <function-math_lerp_float2_float2_float>` 
  *  :ref:`mad (a: uint4; b: uint4; c: uint4) : uint4 <function-math_mad_uint4_uint4_uint4>` 
  *  :ref:`mad (a: uint3; b: uint3; c: uint3) : uint3 <function-math_mad_uint3_uint3_uint3>` 
  *  :ref:`mad (a: uint2; b: uint; c: uint2) : uint2 <function-math_mad_uint2_uint_uint2>` 
  *  :ref:`mad (a: uint; b: uint; c: uint) : uint <function-math_mad_uint_uint_uint>` 
  *  :ref:`mad (a: int3; b: int; c: int3) : int3 <function-math_mad_int3_int_int3>` 
  *  :ref:`mad (a: int2; b: int; c: int2) : int2 <function-math_mad_int2_int_int2>` 
  *  :ref:`mad (a: int4; b: int; c: int4) : int4 <function-math_mad_int4_int_int4>` 
  *  :ref:`mad (a: uint2; b: uint2; c: uint2) : uint2 <function-math_mad_uint2_uint2_uint2>` 
  *  :ref:`mad (a: uint3; b: uint; c: uint3) : uint3 <function-math_mad_uint3_uint_uint3>` 
  *  :ref:`mad (a: int2; b: int2; c: int2) : int2 <function-math_mad_int2_int2_int2>` 
  *  :ref:`mad (a: float4; b: float; c: float4) : float4 <function-math_mad_float4_float_float4>` 
  *  :ref:`mad (a: float3; b: float; c: float3) : float3 <function-math_mad_float3_float_float3>` 
  *  :ref:`mad (a: int; b: int; c: int) : int <function-math_mad_int_int_int>` 
  *  :ref:`mad (a: int3; b: int3; c: int3) : int3 <function-math_mad_int3_int3_int3>` 
  *  :ref:`mad (a: float4; b: float4; c: float4) : float4 <function-math_mad_float4_float4_float4>` 
  *  :ref:`mad (a: double; b: double; c: double) : double <function-math_mad_double_double_double>` 
  *  :ref:`mad (a: float; b: float; c: float) : float <function-math_mad_float_float_float>` 
  *  :ref:`mad (a: float2; b: float2; c: float2) : float2 <function-math_mad_float2_float2_float2>` 
  *  :ref:`mad (a: float3; b: float3; c: float3) : float3 <function-math_mad_float3_float3_float3>` 
  *  :ref:`mad (a: float2; b: float; c: float2) : float2 <function-math_mad_float2_float_float2>` 
  *  :ref:`mad (a: int4; b: int4; c: int4) : int4 <function-math_mad_int4_int4_int4>` 
  *  :ref:`mad (a: uint4; b: uint; c: uint4) : uint4 <function-math_mad_uint4_uint_uint4>` 


clamp
^^^^^

.. _function-math_clamp_float_float_float:

.. das:function:: clamp(t: float; a: float; b: float) : float

Returns the value t clamped to the inclusive range [a, b], equivalent to min(max(t, a), b); works with float, double, float2, float3, float4, int, int64, uint, and uint64 types.

:Arguments: * **t** : float

            * **a** : float

            * **b** : float

.. _function-math_clamp_int64_int64_int64:

.. das:function:: clamp(t: int64; a: int64; b: int64) : int64

.. _function-math_clamp_int3_int3_int3:

.. das:function:: clamp(t: int3; a: int3; b: int3) : int3

.. _function-math_clamp_uint3_uint3_uint3:

.. das:function:: clamp(t: uint3; a: uint3; b: uint3) : uint3

.. _function-math_clamp_int_int_int:

.. das:function:: clamp(t: int; a: int; b: int) : int

.. _function-math_clamp_uint64_uint64_uint64:

.. das:function:: clamp(t: uint64; a: uint64; b: uint64) : uint64

.. _function-math_clamp_int4_int4_int4:

.. das:function:: clamp(t: int4; a: int4; b: int4) : int4

.. _function-math_clamp_int2_int2_int2:

.. das:function:: clamp(t: int2; a: int2; b: int2) : int2

.. _function-math_clamp_float4_float4_float4:

.. das:function:: clamp(t: float4; a: float4; b: float4) : float4

.. _function-math_clamp_float2_float2_float2:

.. das:function:: clamp(t: float2; a: float2; b: float2) : float2

.. _function-math_clamp_uint4_uint4_uint4:

.. das:function:: clamp(t: uint4; a: uint4; b: uint4) : uint4

.. _function-math_clamp_float3_float3_float3:

.. das:function:: clamp(t: float3; a: float3; b: float3) : float3

.. _function-math_clamp_double_double_double:

.. das:function:: clamp(t: double; a: double; b: double) : double

.. _function-math_clamp_uint_uint_uint:

.. das:function:: clamp(t: uint; a: uint; b: uint) : uint

.. _function-math_clamp_uint2_uint2_uint2:

.. das:function:: clamp(t: uint2; a: uint2; b: uint2) : uint2

----


lerp
^^^^

.. _function-math_lerp_double_double_double:

.. das:function:: lerp(a: double; b: double; t: double) : double

Performs linear interpolation between a and b using the factor t, returning a + (b - a) * t; when t is 0 the result is a, when t is 1 the result is b; works component-wise with float, double, float2, float3, and float4 types.

:Arguments: * **a** : double

            * **b** : double

            * **t** : double

.. _function-math_lerp_float4_float4_float4:

.. das:function:: lerp(a: float4; b: float4; t: float4) : float4

.. _function-math_lerp_float3_float3_float3:

.. das:function:: lerp(a: float3; b: float3; t: float3) : float3

.. _function-math_lerp_float2_float2_float2:

.. das:function:: lerp(a: float2; b: float2; t: float2) : float2

.. _function-math_lerp_float_float_float:

.. das:function:: lerp(a: float; b: float; t: float) : float

.. _function-math_lerp_float3_float3_float:

.. das:function:: lerp(a: float3; b: float3; t: float) : float3

.. _function-math_lerp_float4_float4_float:

.. das:function:: lerp(a: float4; b: float4; t: float) : float4

.. _function-math_lerp_float2_float2_float:

.. das:function:: lerp(a: float2; b: float2; t: float) : float2

----


mad
^^^

.. _function-math_mad_uint4_uint4_uint4:

.. das:function:: mad(a: uint4; b: uint4; c: uint4) : uint4

Computes the fused multiply-add operation `a * b + c`.


:Arguments: * **a** : uint4

            * **b** : uint4

            * **c** : uint4

.. _function-math_mad_uint3_uint3_uint3:

.. das:function:: mad(a: uint3; b: uint3; c: uint3) : uint3

.. _function-math_mad_uint2_uint_uint2:

.. das:function:: mad(a: uint2; b: uint; c: uint2) : uint2

.. _function-math_mad_uint_uint_uint:

.. das:function:: mad(a: uint; b: uint; c: uint) : uint

.. _function-math_mad_int3_int_int3:

.. das:function:: mad(a: int3; b: int; c: int3) : int3

.. _function-math_mad_int2_int_int2:

.. das:function:: mad(a: int2; b: int; c: int2) : int2

.. _function-math_mad_int4_int_int4:

.. das:function:: mad(a: int4; b: int; c: int4) : int4

.. _function-math_mad_uint2_uint2_uint2:

.. das:function:: mad(a: uint2; b: uint2; c: uint2) : uint2

.. _function-math_mad_uint3_uint_uint3:

.. das:function:: mad(a: uint3; b: uint; c: uint3) : uint3

.. _function-math_mad_int2_int2_int2:

.. das:function:: mad(a: int2; b: int2; c: int2) : int2

.. _function-math_mad_float4_float_float4:

.. das:function:: mad(a: float4; b: float; c: float4) : float4

.. _function-math_mad_float3_float_float3:

.. das:function:: mad(a: float3; b: float; c: float3) : float3

.. _function-math_mad_int_int_int:

.. das:function:: mad(a: int; b: int; c: int) : int

.. _function-math_mad_int3_int3_int3:

.. das:function:: mad(a: int3; b: int3; c: int3) : int3

.. _function-math_mad_float4_float4_float4:

.. das:function:: mad(a: float4; b: float4; c: float4) : float4

.. _function-math_mad_double_double_double:

.. das:function:: mad(a: double; b: double; c: double) : double

.. _function-math_mad_float_float_float:

.. das:function:: mad(a: float; b: float; c: float) : float

.. _function-math_mad_float2_float2_float2:

.. das:function:: mad(a: float2; b: float2; c: float2) : float2

.. _function-math_mad_float3_float3_float3:

.. das:function:: mad(a: float3; b: float3; c: float3) : float3

.. _function-math_mad_float2_float_float2:

.. das:function:: mad(a: float2; b: float; c: float2) : float2

.. _function-math_mad_int4_int4_int4:

.. das:function:: mad(a: int4; b: int4; c: int4) : int4

.. _function-math_mad_uint4_uint_uint4:

.. das:function:: mad(a: uint4; b: uint; c: uint4) : uint4

----

+++++++++++++++++++++
Matrix element access
+++++++++++++++++++++

  *  :ref:`float3x3 const implicit ==const.[] (m: float3x3 const implicit ==const; i: int) : float3 <function-math__dot__float3x3_const_implicit__eq__eq_const_int>` 
  *  :ref:`float3x3 const implicit ==const.[] (m: float3x3 const implicit ==const; i: uint) : float3 <function-math__dot__float3x3_const_implicit__eq__eq_const_uint>` 
  *  :ref:`float3x3 implicit ==const.[] (m: float3x3 implicit ==const; i: int) : float3& <function-math__dot__float3x3_implicit__eq__eq_const_int>` 
  *  :ref:`float3x3 implicit ==const.[] (m: float3x3 implicit ==const; i: uint) : float3& <function-math__dot__float3x3_implicit__eq__eq_const_uint>` 
  *  :ref:`float3x4 const implicit ==const.[] (m: float3x4 const implicit ==const; i: uint) : float3 <function-math__dot__float3x4_const_implicit__eq__eq_const_uint>` 
  *  :ref:`float3x4 const implicit ==const.[] (m: float3x4 const implicit ==const; i: int) : float3 <function-math__dot__float3x4_const_implicit__eq__eq_const_int>` 
  *  :ref:`float3x4 implicit ==const.[] (m: float3x4 implicit ==const; i: uint) : float3& <function-math__dot__float3x4_implicit__eq__eq_const_uint>` 
  *  :ref:`float3x4 implicit ==const.[] (m: float3x4 implicit ==const; i: int) : float3& <function-math__dot__float3x4_implicit__eq__eq_const_int>` 
  *  :ref:`float4x4 const implicit ==const.[] (m: float4x4 const implicit ==const; i: uint) : float4 <function-math__dot__float4x4_const_implicit__eq__eq_const_uint>` 
  *  :ref:`float4x4 const implicit ==const.[] (m: float4x4 const implicit ==const; i: int) : float4 <function-math__dot__float4x4_const_implicit__eq__eq_const_int>` 
  *  :ref:`float4x4 implicit ==const.[] (m: float4x4 implicit ==const; i: int) : float4& <function-math__dot__float4x4_implicit__eq__eq_const_int>` 
  *  :ref:`float4x4 implicit ==const.[] (m: float4x4 implicit ==const; i: uint) : float4& <function-math__dot__float4x4_implicit__eq__eq_const_uint>` 


float3x3 const implicit ==const.[]
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-math__dot__float3x3_const_implicit__eq__eq_const_int:

.. das:function:: float3x3 const implicit ==const.[](m: float3x3 const implicit ==const; i: int) : float3

Returns a reference to the row vector at the specified index of a float3x3, float3x4, or float4x4 matrix; the index can be int or uint and must be within the row count of the matrix.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : int

.. _function-math__dot__float3x3_const_implicit__eq__eq_const_uint:

.. das:function:: float3x3 const implicit ==const.[](m: float3x3 const implicit ==const; i: uint) : float3

----


float3x3 implicit ==const.[]
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-math__dot__float3x3_implicit__eq__eq_const_int:

.. das:function:: float3x3 implicit ==const.[](m: float3x3 implicit ==const; i: int) : float3&

Returns a reference to the row vector at the specified index of a float3x3, float3x4, or float4x4 matrix; the index can be int or uint and must be within the row count of the matrix.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : int

.. _function-math__dot__float3x3_implicit__eq__eq_const_uint:

.. das:function:: float3x3 implicit ==const.[](m: float3x3 implicit ==const; i: uint) : float3&

----


float3x4 const implicit ==const.[]
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-math__dot__float3x4_const_implicit__eq__eq_const_uint:

.. das:function:: float3x4 const implicit ==const.[](m: float3x4 const implicit ==const; i: uint) : float3

Returns a reference to the row vector at the specified index of a float3x3, float3x4, or float4x4 matrix; the index can be int or uint and must be within the row count of the matrix.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : uint

.. _function-math__dot__float3x4_const_implicit__eq__eq_const_int:

.. das:function:: float3x4 const implicit ==const.[](m: float3x4 const implicit ==const; i: int) : float3

----


float3x4 implicit ==const.[]
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-math__dot__float3x4_implicit__eq__eq_const_uint:

.. das:function:: float3x4 implicit ==const.[](m: float3x4 implicit ==const; i: uint) : float3&

Returns a reference to the row vector at the specified index of a float3x3, float3x4, or float4x4 matrix; the index can be int or uint and must be within the row count of the matrix.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : uint

.. _function-math__dot__float3x4_implicit__eq__eq_const_int:

.. das:function:: float3x4 implicit ==const.[](m: float3x4 implicit ==const; i: int) : float3&

----


float4x4 const implicit ==const.[]
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-math__dot__float4x4_const_implicit__eq__eq_const_uint:

.. das:function:: float4x4 const implicit ==const.[](m: float4x4 const implicit ==const; i: uint) : float4

Returns a reference to the row vector at the specified index of a float3x3, float3x4, or float4x4 matrix; the index can be int or uint and must be within the row count of the matrix.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : uint

.. _function-math__dot__float4x4_const_implicit__eq__eq_const_int:

.. das:function:: float4x4 const implicit ==const.[](m: float4x4 const implicit ==const; i: int) : float4

----


float4x4 implicit ==const.[]
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. _function-math__dot__float4x4_implicit__eq__eq_const_int:

.. das:function:: float4x4 implicit ==const.[](m: float4x4 implicit ==const; i: int) : float4&

Returns a reference to the row vector at the specified index of a float3x3, float3x4, or float4x4 matrix; the index can be int or uint and must be within the row count of the matrix.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : int

.. _function-math__dot__float4x4_implicit__eq__eq_const_uint:

.. das:function:: float4x4 implicit ==const.[](m: float4x4 implicit ==const; i: uint) : float4&

----

+++++++++++++++++
Matrix operations
+++++++++++++++++

  *  :ref:`float3x3 implicit\!= (x: float3x3 implicit; y: float3x3 implicit) : bool <function-math__ex__eq__float3x3_implicit_float3x3_implicit>` 
  *  :ref:`float3x3 implicit* (x: float3x3 implicit; y: float3x3 implicit) : float3x3 <function-math__st__float3x3_implicit_float3x3_implicit>` 
  *  :ref:`float3x3 implicit* (x: float3x3 implicit; y: float3) : float3 <function-math__st__float3x3_implicit_float3>` 
  *  :ref:`float3x3 implicit== (x: float3x3 implicit; y: float3x3 implicit) : bool <function-math__eq__eq__float3x3_implicit_float3x3_implicit>` 
  *  :ref:`float3x4 implicit\!= (x: float3x4 implicit; y: float3x4 implicit) : bool <function-math__ex__eq__float3x4_implicit_float3x4_implicit>` 
  *  :ref:`float3x4 implicit* (x: float3x4 implicit; y: float3x4 implicit) : float3x4 <function-math__st__float3x4_implicit_float3x4_implicit>` 
  *  :ref:`float3x4 implicit* (x: float3x4 implicit; y: float3) : float3 <function-math__st__float3x4_implicit_float3>` 
  *  :ref:`float3x4 implicit== (x: float3x4 implicit; y: float3x4 implicit) : bool <function-math__eq__eq__float3x4_implicit_float3x4_implicit>` 
  *  :ref:`float4x4 implicit\!= (x: float4x4 implicit; y: float4x4 implicit) : bool <function-math__ex__eq__float4x4_implicit_float4x4_implicit>` 
  *  :ref:`float4x4 implicit* (x: float4x4 implicit; y: float4) : float4 <function-math__st__float4x4_implicit_float4>` 
  *  :ref:`float4x4 implicit* (x: float4x4 implicit; y: float4x4 implicit) : float4x4 <function-math__st__float4x4_implicit_float4x4_implicit>` 
  *  :ref:`float4x4 implicit== (x: float4x4 implicit; y: float4x4 implicit) : bool <function-math__eq__eq__float4x4_implicit_float4x4_implicit>` 

.. _function-math__ex__eq__float3x3_implicit_float3x3_implicit:

.. das:function:: float3x3 implicit!=(x: float3x3 implicit; y: float3x3 implicit) : bool

Returns true if two float3x3 matrices are not equal, comparing all elements component-wise.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit


float3x3 implicit*
^^^^^^^^^^^^^^^^^^

.. _function-math__st__float3x3_implicit_float3x3_implicit:

.. das:function:: float3x3 implicit*(x: float3x3 implicit; y: float3x3 implicit) : float3x3

Multiplies two 3x3 matrices and returns the resulting 3x3 matrix.


:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math__st__float3x3_implicit_float3:

.. das:function:: float3x3 implicit*(x: float3x3 implicit; y: float3) : float3

----

.. _function-math__eq__eq__float3x3_implicit_float3x3_implicit:

.. das:function:: float3x3 implicit==(x: float3x3 implicit; y: float3x3 implicit) : bool

Returns true if two float3x3 matrices are exactly equal, comparing all elements component-wise.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math__ex__eq__float3x4_implicit_float3x4_implicit:

.. das:function:: float3x4 implicit!=(x: float3x4 implicit; y: float3x4 implicit) : bool

Returns true if two float3x4 matrices are not equal, comparing all elements component-wise.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit


float3x4 implicit*
^^^^^^^^^^^^^^^^^^

.. _function-math__st__float3x4_implicit_float3x4_implicit:

.. das:function:: float3x4 implicit*(x: float3x4 implicit; y: float3x4 implicit) : float3x4

Multiplies two 3x4 matrices and returns the resulting 3x4 matrix.


:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math__st__float3x4_implicit_float3:

.. das:function:: float3x4 implicit*(x: float3x4 implicit; y: float3) : float3

----

.. _function-math__eq__eq__float3x4_implicit_float3x4_implicit:

.. das:function:: float3x4 implicit==(x: float3x4 implicit; y: float3x4 implicit) : bool

Returns true if two float3x4 matrices are exactly equal, comparing all elements component-wise.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math__ex__eq__float4x4_implicit_float4x4_implicit:

.. das:function:: float4x4 implicit!=(x: float4x4 implicit; y: float4x4 implicit) : bool

Returns true if two float4x4 matrices are not equal, comparing all elements component-wise.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit


float4x4 implicit*
^^^^^^^^^^^^^^^^^^

.. _function-math__st__float4x4_implicit_float4:

.. das:function:: float4x4 implicit*(x: float4x4 implicit; y: float4) : float4

Transforms a float4 vector by a 4x4 matrix.


:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** : float4

.. _function-math__st__float4x4_implicit_float4x4_implicit:

.. das:function:: float4x4 implicit*(x: float4x4 implicit; y: float4x4 implicit) : float4x4

----

.. _function-math__eq__eq__float4x4_implicit_float4x4_implicit:

.. das:function:: float4x4 implicit==(x: float4x4 implicit; y: float4x4 implicit) : bool

Returns true if two float4x4 matrices are exactly equal, comparing all elements component-wise.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

+++++++++++++++++++
Matrix initializers
+++++++++++++++++++

  *  :ref:`float3x3 (arg0: float3x4 implicit) : float3x3 <function-math_float3x3_float3x4_implicit>` 
  *  :ref:`float3x3 () : float3x3 <function-math_float3x3>` 
  *  :ref:`float3x3 (arg0: float4x4 implicit) : float3x3 <function-math_float3x3_float4x4_implicit>` 
  *  :ref:`float3x4 (arg0: float4x4 implicit) : float3x4 <function-math_float3x4_float4x4_implicit>` 
  *  :ref:`float3x4 () : float3x4 <function-math_float3x4>` 
  *  :ref:`float4x4 (arg0: float3x4 implicit) : float4x4 <function-math_float4x4_float3x4_implicit>` 
  *  :ref:`float4x4 () : float4x4 <function-math_float4x4>` 
  *  :ref:`identity3x3 () : float3x3 <function-math_identity3x3>` 
  *  :ref:`identity3x4 () : float3x4 <function-math_identity3x4>` 
  *  :ref:`identity4x4 () : float4x4 <function-math_identity4x4>` 


float3x3
^^^^^^^^

.. _function-math_float3x3_float3x4_implicit:

.. das:function:: float3x3(arg0: float3x4 implicit) : float3x3

Extracts the upper-left 3x3 rotation part from a float3x4 transformation matrix, returning it as a float3x3.

:Arguments: * **arg0** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_float3x3:

.. das:function:: float3x3() : float3x3

.. _function-math_float3x3_float4x4_implicit:

.. das:function:: float3x3(arg0: float4x4 implicit) : float3x3

----


float3x4
^^^^^^^^

.. _function-math_float3x4_float4x4_implicit:

.. das:function:: float3x4(arg0: float4x4 implicit) : float3x4

Constructs a float3x4 transformation matrix from a float3x3 rotation matrix, with the translation component set to zero.

:Arguments: * **arg0** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_float3x4:

.. das:function:: float3x4() : float3x4

----


float4x4
^^^^^^^^

.. _function-math_float4x4_float3x4_implicit:

.. das:function:: float4x4(arg0: float3x4 implicit) : float4x4

Converts a float3x4 transformation matrix to a float4x4 matrix, filling the fourth row with [0, 0, 0, 1].

:Arguments: * **arg0** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_float4x4:

.. das:function:: float4x4() : float4x4

----

.. _function-math_identity3x3:

.. das:function:: identity3x3() : float3x3

Returns a float3x3 identity matrix with ones on the diagonal and zeros elsewhere.

.. _function-math_identity3x4:

.. das:function:: identity3x4() : float3x4

Returns a float3x4 identity transformation matrix with the rotation part set to identity and the translation set to zero.

.. _function-math_identity4x4:

.. das:function:: identity4x4() : float4x4

Returns a float4x4 identity matrix with ones on the diagonal and zeros elsewhere.

+++++++++++++++++++
Matrix manipulation
+++++++++++++++++++

  *  :ref:`compose (pos: float3; rot: float4; scale: float3) : float4x4 <function-math_compose_float3_float4_float3>` 
  *  :ref:`decompose (mat: float4x4 implicit; pos: float3& implicit; rot: float4& implicit; scale: float3& implicit) <function-math_decompose_float4x4_implicit_float3_implicit_float4_implicit_float3_implicit>` 
  *  :ref:`determinant (x: float3x4 implicit) : float <function-math_determinant_float3x4_implicit>` 
  *  :ref:`determinant (x: float4x4 implicit) : float <function-math_determinant_float4x4_implicit>` 
  *  :ref:`determinant (x: float3x3 implicit) : float <function-math_determinant_float3x3_implicit>` 
  *  :ref:`identity (x: float3x4 implicit) <function-math_identity_float3x4_implicit>` 
  *  :ref:`identity (x: float4x4 implicit) <function-math_identity_float4x4_implicit>` 
  *  :ref:`identity (x: float3x3 implicit) <function-math_identity_float3x3_implicit>` 
  *  :ref:`inverse (x: float3x4 implicit) : float3x4 <function-math_inverse_float3x4_implicit>` 
  *  :ref:`inverse (m: float4x4 implicit) : float4x4 <function-math_inverse_float4x4_implicit>` 
  *  :ref:`look_at (eye: float3; at: float3; up: float3) : float4x4 <function-math_look_at_float3_float3_float3>` 
  *  :ref:`orthonormal_inverse (m: float3x3 implicit) : float3x3 <function-math_orthonormal_inverse_float3x3_implicit>` 
  *  :ref:`orthonormal_inverse (m: float3x4 implicit) : float3x4 <function-math_orthonormal_inverse_float3x4_implicit>` 
  *  :ref:`persp_forward (wk: float; hk: float; zn: float; zf: float) : float4x4 <function-math_persp_forward_float_float_float_float>` 
  *  :ref:`persp_reverse (wk: float; hk: float; zn: float; zf: float) : float4x4 <function-math_persp_reverse_float_float_float_float>` 
  *  :ref:`rotate (x: float3x4 implicit; y: float3) : float3 <function-math_rotate_float3x4_implicit_float3>` 
  *  :ref:`translation (xyz: float3) : float4x4 <function-math_translation_float3>` 
  *  :ref:`transpose (x: float4x4 implicit) : float4x4 <function-math_transpose_float4x4_implicit>` 

.. _function-math_compose_float3_float4_float3:

.. das:function:: compose(pos: float3; rot: float4; scale: float3) : float4x4

Constructs a float4x4 transformation matrix from a float3 translation position, a float4 quaternion rotation, and a float3 scale.

:Arguments: * **pos** : float3

            * **rot** : float4

            * **scale** : float3

.. _function-math_decompose_float4x4_implicit_float3_implicit_float4_implicit_float3_implicit:

.. das:function:: decompose(mat: float4x4 implicit; pos: float3& implicit; rot: float4& implicit; scale: float3& implicit)

Decomposes a float4x4 transformation matrix into its float3 translation position, float4 quaternion rotation, and float3 scale components., writing the results into the output arguments rot and pos.

:Arguments: * **mat** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **pos** : float3& implicit

            * **rot** : float4& implicit

            * **scale** : float3& implicit


determinant
^^^^^^^^^^^

.. _function-math_determinant_float3x4_implicit:

.. das:function:: determinant(x: float3x4 implicit) : float

Returns the determinant of a float3x4 matrix as a float scalar; a zero determinant indicates the matrix is singular and non-invertible.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_determinant_float4x4_implicit:

.. das:function:: determinant(x: float4x4 implicit) : float

.. _function-math_determinant_float3x3_implicit:

.. das:function:: determinant(x: float3x3 implicit) : float

----


identity
^^^^^^^^

.. _function-math_identity_float3x4_implicit:

.. das:function:: identity(x: float3x4 implicit)

Sets the given float3x4 matrix to the identity transformation (rotation part is the identity matrix, translation is zero) and returns it.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_identity_float4x4_implicit:

.. das:function:: identity(x: float4x4 implicit)

.. _function-math_identity_float3x3_implicit:

.. das:function:: identity(x: float3x3 implicit)

----


inverse
^^^^^^^

.. _function-math_inverse_float3x4_implicit:

.. das:function:: inverse(x: float3x4 implicit) : float3x4

Returns the inverse of a matrix, such that multiplying the original by its inverse yields the identity; works with float3x4 and float4x4 matrix types.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_inverse_float4x4_implicit:

.. das:function:: inverse(m: float4x4 implicit) : float4x4

----

.. _function-math_look_at_float3_float3_float3:

.. das:function:: look_at(eye: float3; at: float3; up: float3) : float4x4

Constructs a float4x4 look-at view transformation matrix from eye position, target position, and up vector. from an eye position, a target point to look at, and an up direction vector.

:Arguments: * **eye** : float3

            * **at** : float3

            * **up** : float3


orthonormal_inverse
^^^^^^^^^^^^^^^^^^^

.. _function-math_orthonormal_inverse_float3x3_implicit:

.. das:function:: orthonormal_inverse(m: float3x3 implicit) : float3x3

Returns the inverse of a float3x3 orthonormal matrix (each axis is unit length and mutually perpendicular), computed more efficiently than a general matrix inverse.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math_orthonormal_inverse_float3x4_implicit:

.. das:function:: orthonormal_inverse(m: float3x4 implicit) : float3x4

----

.. _function-math_persp_forward_float_float_float_float:

.. das:function:: persp_forward(wk: float; hk: float; zn: float; zf: float) : float4x4

Returns a forward (standard) perspective projection float4x4 matrix constructed from horizontal scale wk, vertical scale hk, near plane zn, and far plane zf.

:Arguments: * **wk** : float

            * **hk** : float

            * **zn** : float

            * **zf** : float

.. _function-math_persp_reverse_float_float_float_float:

.. das:function:: persp_reverse(wk: float; hk: float; zn: float; zf: float) : float4x4

Returns a reverse-depth perspective projection float4x4 matrix constructed from horizontal scale wk, vertical scale hk, near plane zn, and far plane zf, mapping the far plane to 0 and the near plane to 1.

:Arguments: * **wk** : float

            * **hk** : float

            * **zn** : float

            * **zf** : float

.. _function-math_rotate_float3x4_implicit_float3:

.. das:function:: rotate(x: float3x4 implicit; y: float3) : float3

Rotates a float3 vector v by the 3x3 rotation part of the float3x4 matrix m, ignoring the translation component.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** : float3

.. _function-math_translation_float3:

.. das:function:: translation(xyz: float3) : float4x4

Constructs a float4x4 matrix representing a pure translation by the given float3 offset. by the float3 offset xyz, with the rotation part set to identity.

:Arguments: * **xyz** : float3

.. _function-math_transpose_float4x4_implicit:

.. das:function:: transpose(x: float4x4 implicit) : float4x4

Returns the transpose of a float3x3 or float4x4 matrix, swapping rows and columns.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

+++++++++++++++++++++
Quaternion operations
+++++++++++++++++++++

  *  :ref:`euler_from_quat (angles: float4) : float3 <function-math_euler_from_quat_float4>` 
  *  :ref:`quat (m: float4x4 implicit) : float4 <function-math_quat_float4x4_implicit>` 
  *  :ref:`quat (m: float3x3 implicit) : float4 <function-math_quat_float3x3_implicit>` 
  *  :ref:`quat (m: float3x4 implicit) : float4 <function-math_quat_float3x4_implicit>` 
  *  :ref:`quat_conjugate (q: float4) : float4 <function-math_quat_conjugate_float4>` 
  *  :ref:`quat_from_euler (x: float; y: float; z: float) : float4 <function-math_quat_from_euler_float_float_float>` 
  *  :ref:`quat_from_euler (angles: float3) : float4 <function-math_quat_from_euler_float3>` 
  *  :ref:`quat_from_unit_arc (v0: float3; v1: float3) : float4 <function-math_quat_from_unit_arc_float3_float3>` 
  *  :ref:`quat_from_unit_vec_ang (v: float3; ang: float) : float4 <function-math_quat_from_unit_vec_ang_float3_float>` 
  *  :ref:`quat_mul (q1: float4; q2: float4) : float4 <function-math_quat_mul_float4_float4>` 
  *  :ref:`quat_mul_vec (q: float4; v: float3) : float3 <function-math_quat_mul_vec_float4_float3>` 
  *  :ref:`quat_slerp (t: float; a: float4; b: float4) : float4 <function-math_quat_slerp_float_float4_float4>` 

.. _function-math_euler_from_quat_float4:

.. das:function:: euler_from_quat(angles: float4) : float3

Converts a float4 quaternion to Euler angles, returning a float3 representing rotation around the x, y, and z axes in radians.

:Arguments: * **angles** : float4


quat
^^^^

.. _function-math_quat_float4x4_implicit:

.. das:function:: quat(m: float4x4 implicit) : float4

Extracts the rotation part of a float3x4 matrix and returns it as a float4 quaternion in (x, y, z, w) format.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_quat_float3x3_implicit:

.. das:function:: quat(m: float3x3 implicit) : float4

.. _function-math_quat_float3x4_implicit:

.. das:function:: quat(m: float3x4 implicit) : float4

----

.. _function-math_quat_conjugate_float4:

.. das:function:: quat_conjugate(q: float4) : float4

Returns the conjugate of the float4 quaternion q by negating its xyz components, which for unit quaternions equals the inverse rotation.

:Arguments: * **q** : float4


quat_from_euler
^^^^^^^^^^^^^^^

.. _function-math_quat_from_euler_float_float_float:

.. das:function:: quat_from_euler(x: float; y: float; z: float) : float4

Creates a float4 quaternion from a float3 of Euler angles (pitch, yaw, roll) given in radians.

:Arguments: * **x** : float

            * **y** : float

            * **z** : float

.. _function-math_quat_from_euler_float3:

.. das:function:: quat_from_euler(angles: float3) : float4

----

.. _function-math_quat_from_unit_arc_float3_float3:

.. das:function:: quat_from_unit_arc(v0: float3; v1: float3) : float4

Creates a float4 quaternion representing the shortest rotation arc from unit-length float3 vector v0 to unit-length float3 vector v1; both input vectors must be normalized.

:Arguments: * **v0** : float3

            * **v1** : float3

.. _function-math_quat_from_unit_vec_ang_float3_float:

.. das:function:: quat_from_unit_vec_ang(v: float3; ang: float) : float4

Creates a float4 quaternion representing a rotation of ang radians around the unit-length float3 axis vector v.

:Arguments: * **v** : float3

            * **ang** : float

.. _function-math_quat_mul_float4_float4:

.. das:function:: quat_mul(q1: float4; q2: float4) : float4

Returns the float4 quaternion product of q1 and q2, representing the combined rotation of q2 followed by q1.

:Arguments: * **q1** : float4

            * **q2** : float4

.. _function-math_quat_mul_vec_float4_float3:

.. das:function:: quat_mul_vec(q: float4; v: float3) : float3

Rotates a float3 vector v by the float4 quaternion q and returns the resulting float3 vector.

:Arguments: * **q** : float4

            * **v** : float3

.. _function-math_quat_slerp_float_float4_float4:

.. das:function:: quat_slerp(t: float; a: float4; b: float4) : float4

Performs spherical linear interpolation between float4 quaternions a and b by factor t in the range [0,1], returning a smoothly interpolated float4 quaternion.

:Arguments: * **t** : float

            * **a** : float4

            * **b** : float4

+++++++++++++++++++++
Packing and unpacking
+++++++++++++++++++++

  *  :ref:`pack_float_to_byte (x: float4) : uint <function-math_pack_float_to_byte_float4>` 
  *  :ref:`unpack_byte_to_float (x: uint) : float4 <function-math_unpack_byte_to_float_uint>` 

.. _function-math_pack_float_to_byte_float4:

.. das:function:: pack_float_to_byte(x: float4) : uint

Packs a float4 vector into a single uint by converting each component to an 8-bit byte value, mapping the XYZW components to the four bytes of the result.

:Arguments: * **x** : float4

.. _function-math_unpack_byte_to_float_uint:

.. das:function:: unpack_byte_to_float(x: uint) : float4

Unpacks the four bytes of a uint into a float4 vector, converting each 8-bit byte value to its corresponding floating-point component.

:Arguments: * **x** : uint


