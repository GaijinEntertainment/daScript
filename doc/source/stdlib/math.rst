
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

The ratio of a circle's circumference to its diameter. π

.. _global-math-DBL_PI:

.. das:attribute:: DBL_PI = 3.141592653589793lf

The ratio of a circle's circumference to its diameter. π

.. _global-math-FLT_EPSILON:

.. das:attribute:: FLT_EPSILON = 1.1920929e-07f

the difference between 1 and the smallest floating point number of type float that is greater than 1.

.. _global-math-DBL_EPSILON:

.. das:attribute:: DBL_EPSILON = 2.220446049250313e-16lf

the difference between 1 and the smallest double precision floating point number of type double that is greater than 1.

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

  *  :ref:`min (x: int; y: int) : int <function-math_min_int_int>` 
  *  :ref:`max (x: int; y: int) : int <function-math_max_int_int>` 
  *  :ref:`min (x: int2; y: int2) : int2 <function-math_min_int2_int2>` 
  *  :ref:`max (x: int2; y: int2) : int2 <function-math_max_int2_int2>` 
  *  :ref:`min (x: int3; y: int3) : int3 <function-math_min_int3_int3>` 
  *  :ref:`max (x: int3; y: int3) : int3 <function-math_max_int3_int3>` 
  *  :ref:`min (x: int4; y: int4) : int4 <function-math_min_int4_int4>` 
  *  :ref:`max (x: int4; y: int4) : int4 <function-math_max_int4_int4>` 
  *  :ref:`min (x: uint; y: uint) : uint <function-math_min_uint_uint>` 
  *  :ref:`max (x: uint; y: uint) : uint <function-math_max_uint_uint>` 
  *  :ref:`min (x: uint2; y: uint2) : uint2 <function-math_min_uint2_uint2>` 
  *  :ref:`max (x: uint2; y: uint2) : uint2 <function-math_max_uint2_uint2>` 
  *  :ref:`min (x: uint3; y: uint3) : uint3 <function-math_min_uint3_uint3>` 
  *  :ref:`max (x: uint3; y: uint3) : uint3 <function-math_max_uint3_uint3>` 
  *  :ref:`min (x: uint4; y: uint4) : uint4 <function-math_min_uint4_uint4>` 
  *  :ref:`max (x: uint4; y: uint4) : uint4 <function-math_max_uint4_uint4>` 
  *  :ref:`min (x: float; y: float) : float <function-math_min_float_float>` 
  *  :ref:`max (x: float; y: float) : float <function-math_max_float_float>` 
  *  :ref:`min (x: float2; y: float2) : float2 <function-math_min_float2_float2>` 
  *  :ref:`max (x: float2; y: float2) : float2 <function-math_max_float2_float2>` 
  *  :ref:`min (x: float3; y: float3) : float3 <function-math_min_float3_float3>` 
  *  :ref:`max (x: float3; y: float3) : float3 <function-math_max_float3_float3>` 
  *  :ref:`min (x: float4; y: float4) : float4 <function-math_min_float4_float4>` 
  *  :ref:`max (x: float4; y: float4) : float4 <function-math_max_float4_float4>` 
  *  :ref:`min (x: double; y: double) : double <function-math_min_double_double>` 
  *  :ref:`max (x: double; y: double) : double <function-math_max_double_double>` 
  *  :ref:`min (x: int64; y: int64) : int64 <function-math_min_int64_int64>` 
  *  :ref:`max (x: int64; y: int64) : int64 <function-math_max_int64_int64>` 
  *  :ref:`min (x: uint64; y: uint64) : uint64 <function-math_min_uint64_uint64>` 
  *  :ref:`max (x: uint64; y: uint64) : uint64 <function-math_max_uint64_uint64>` 

.. _function-math_min_int_int:

.. das:function:: min(x: int; y: int) : int

returns the minimum of x and y

:Arguments: * **x** : int

            * **y** : int

.. _function-math_max_int_int:

.. das:function:: max(x: int; y: int) : int

returns the maximum of x and y

:Arguments: * **x** : int

            * **y** : int

.. _function-math_min_int2_int2:

.. das:function:: min(x: int2; y: int2) : int2

returns the minimum of x and y

:Arguments: * **x** : int2

            * **y** : int2

.. _function-math_max_int2_int2:

.. das:function:: max(x: int2; y: int2) : int2

returns the maximum of x and y

:Arguments: * **x** : int2

            * **y** : int2

.. _function-math_min_int3_int3:

.. das:function:: min(x: int3; y: int3) : int3

returns the minimum of x and y

:Arguments: * **x** : int3

            * **y** : int3

.. _function-math_max_int3_int3:

.. das:function:: max(x: int3; y: int3) : int3

returns the maximum of x and y

:Arguments: * **x** : int3

            * **y** : int3

.. _function-math_min_int4_int4:

.. das:function:: min(x: int4; y: int4) : int4

returns the minimum of x and y

:Arguments: * **x** : int4

            * **y** : int4

.. _function-math_max_int4_int4:

.. das:function:: max(x: int4; y: int4) : int4

returns the maximum of x and y

:Arguments: * **x** : int4

            * **y** : int4

.. _function-math_min_uint_uint:

.. das:function:: min(x: uint; y: uint) : uint

returns the minimum of x and y

:Arguments: * **x** : uint

            * **y** : uint

.. _function-math_max_uint_uint:

.. das:function:: max(x: uint; y: uint) : uint

returns the maximum of x and y

:Arguments: * **x** : uint

            * **y** : uint

.. _function-math_min_uint2_uint2:

.. das:function:: min(x: uint2; y: uint2) : uint2

returns the minimum of x and y

:Arguments: * **x** : uint2

            * **y** : uint2

.. _function-math_max_uint2_uint2:

.. das:function:: max(x: uint2; y: uint2) : uint2

returns the maximum of x and y

:Arguments: * **x** : uint2

            * **y** : uint2

.. _function-math_min_uint3_uint3:

.. das:function:: min(x: uint3; y: uint3) : uint3

returns the minimum of x and y

:Arguments: * **x** : uint3

            * **y** : uint3

.. _function-math_max_uint3_uint3:

.. das:function:: max(x: uint3; y: uint3) : uint3

returns the maximum of x and y

:Arguments: * **x** : uint3

            * **y** : uint3

.. _function-math_min_uint4_uint4:

.. das:function:: min(x: uint4; y: uint4) : uint4

returns the minimum of x and y

:Arguments: * **x** : uint4

            * **y** : uint4

.. _function-math_max_uint4_uint4:

.. das:function:: max(x: uint4; y: uint4) : uint4

returns the maximum of x and y

:Arguments: * **x** : uint4

            * **y** : uint4

.. _function-math_min_float_float:

.. das:function:: min(x: float; y: float) : float

returns the minimum of x and y

:Arguments: * **x** : float

            * **y** : float

.. _function-math_max_float_float:

.. das:function:: max(x: float; y: float) : float

returns the maximum of x and y

:Arguments: * **x** : float

            * **y** : float

.. _function-math_min_float2_float2:

.. das:function:: min(x: float2; y: float2) : float2

returns the minimum of x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_max_float2_float2:

.. das:function:: max(x: float2; y: float2) : float2

returns the maximum of x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_min_float3_float3:

.. das:function:: min(x: float3; y: float3) : float3

returns the minimum of x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_max_float3_float3:

.. das:function:: max(x: float3; y: float3) : float3

returns the maximum of x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_min_float4_float4:

.. das:function:: min(x: float4; y: float4) : float4

returns the minimum of x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_max_float4_float4:

.. das:function:: max(x: float4; y: float4) : float4

returns the maximum of x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_min_double_double:

.. das:function:: min(x: double; y: double) : double

returns the minimum of x and y

:Arguments: * **x** : double

            * **y** : double

.. _function-math_max_double_double:

.. das:function:: max(x: double; y: double) : double

returns the maximum of x and y

:Arguments: * **x** : double

            * **y** : double

.. _function-math_min_int64_int64:

.. das:function:: min(x: int64; y: int64) : int64

returns the minimum of x and y

:Arguments: * **x** : int64

            * **y** : int64

.. _function-math_max_int64_int64:

.. das:function:: max(x: int64; y: int64) : int64

returns the maximum of x and y

:Arguments: * **x** : int64

            * **y** : int64

.. _function-math_min_uint64_uint64:

.. das:function:: min(x: uint64; y: uint64) : uint64

returns the minimum of x and y

:Arguments: * **x** : uint64

            * **y** : uint64

.. _function-math_max_uint64_uint64:

.. das:function:: max(x: uint64; y: uint64) : uint64

returns the maximum of x and y

:Arguments: * **x** : uint64

            * **y** : uint64

+++++++++++++++++
float* and double
+++++++++++++++++

  *  :ref:`sin (x: float) : float <function-math_sin_float>` 
  *  :ref:`cos (x: float) : float <function-math_cos_float>` 
  *  :ref:`tan (x: float) : float <function-math_tan_float>` 
  *  :ref:`sin (x: float2) : float2 <function-math_sin_float2>` 
  *  :ref:`cos (x: float2) : float2 <function-math_cos_float2>` 
  *  :ref:`tan (x: float2) : float2 <function-math_tan_float2>` 
  *  :ref:`sin (x: float3) : float3 <function-math_sin_float3>` 
  *  :ref:`cos (x: float3) : float3 <function-math_cos_float3>` 
  *  :ref:`tan (x: float3) : float3 <function-math_tan_float3>` 
  *  :ref:`sin (x: float4) : float4 <function-math_sin_float4>` 
  *  :ref:`cos (x: float4) : float4 <function-math_cos_float4>` 
  *  :ref:`tan (x: float4) : float4 <function-math_tan_float4>` 
  *  :ref:`exp (x: float) : float <function-math_exp_float>` 
  *  :ref:`log (x: float) : float <function-math_log_float>` 
  *  :ref:`exp2 (x: float) : float <function-math_exp2_float>` 
  *  :ref:`log2 (x: float) : float <function-math_log2_float>` 
  *  :ref:`rcp (x: float) : float <function-math_rcp_float>` 
  *  :ref:`pow (x: float; y: float) : float <function-math_pow_float_float>` 
  *  :ref:`exp (x: float2) : float2 <function-math_exp_float2>` 
  *  :ref:`log (x: float2) : float2 <function-math_log_float2>` 
  *  :ref:`exp2 (x: float2) : float2 <function-math_exp2_float2>` 
  *  :ref:`log2 (x: float2) : float2 <function-math_log2_float2>` 
  *  :ref:`rcp (x: float2) : float2 <function-math_rcp_float2>` 
  *  :ref:`pow (x: float2; y: float2) : float2 <function-math_pow_float2_float2>` 
  *  :ref:`exp (x: float3) : float3 <function-math_exp_float3>` 
  *  :ref:`log (x: float3) : float3 <function-math_log_float3>` 
  *  :ref:`exp2 (x: float3) : float3 <function-math_exp2_float3>` 
  *  :ref:`log2 (x: float3) : float3 <function-math_log2_float3>` 
  *  :ref:`rcp (x: float3) : float3 <function-math_rcp_float3>` 
  *  :ref:`pow (x: float3; y: float3) : float3 <function-math_pow_float3_float3>` 
  *  :ref:`exp (x: float4) : float4 <function-math_exp_float4>` 
  *  :ref:`log (x: float4) : float4 <function-math_log_float4>` 
  *  :ref:`exp2 (x: float4) : float4 <function-math_exp2_float4>` 
  *  :ref:`log2 (x: float4) : float4 <function-math_log2_float4>` 
  *  :ref:`rcp (x: float4) : float4 <function-math_rcp_float4>` 
  *  :ref:`pow (x: float4; y: float4) : float4 <function-math_pow_float4_float4>` 
  *  :ref:`floor (x: float) : float <function-math_floor_float>` 
  *  :ref:`ceil (x: float) : float <function-math_ceil_float>` 
  *  :ref:`sqrt (x: float) : float <function-math_sqrt_float>` 
  *  :ref:`saturate (x: float) : float <function-math_saturate_float>` 
  *  :ref:`floor (x: float2) : float2 <function-math_floor_float2>` 
  *  :ref:`ceil (x: float2) : float2 <function-math_ceil_float2>` 
  *  :ref:`sqrt (x: float2) : float2 <function-math_sqrt_float2>` 
  *  :ref:`saturate (x: float2) : float2 <function-math_saturate_float2>` 
  *  :ref:`floor (x: float3) : float3 <function-math_floor_float3>` 
  *  :ref:`ceil (x: float3) : float3 <function-math_ceil_float3>` 
  *  :ref:`sqrt (x: float3) : float3 <function-math_sqrt_float3>` 
  *  :ref:`saturate (x: float3) : float3 <function-math_saturate_float3>` 
  *  :ref:`floor (x: float4) : float4 <function-math_floor_float4>` 
  *  :ref:`ceil (x: float4) : float4 <function-math_ceil_float4>` 
  *  :ref:`sqrt (x: float4) : float4 <function-math_sqrt_float4>` 
  *  :ref:`saturate (x: float4) : float4 <function-math_saturate_float4>` 
  *  :ref:`abs (x: int) : int <function-math_abs_int>` 
  *  :ref:`sign (x: int) : int <function-math_sign_int>` 
  *  :ref:`abs (x: int2) : int2 <function-math_abs_int2>` 
  *  :ref:`sign (x: int2) : int2 <function-math_sign_int2>` 
  *  :ref:`abs (x: int3) : int3 <function-math_abs_int3>` 
  *  :ref:`sign (x: int3) : int3 <function-math_sign_int3>` 
  *  :ref:`abs (x: int4) : int4 <function-math_abs_int4>` 
  *  :ref:`sign (x: int4) : int4 <function-math_sign_int4>` 
  *  :ref:`abs (x: uint) : uint <function-math_abs_uint>` 
  *  :ref:`sign (x: uint) : uint <function-math_sign_uint>` 
  *  :ref:`abs (x: uint2) : uint2 <function-math_abs_uint2>` 
  *  :ref:`sign (x: uint2) : uint2 <function-math_sign_uint2>` 
  *  :ref:`abs (x: uint3) : uint3 <function-math_abs_uint3>` 
  *  :ref:`sign (x: uint3) : uint3 <function-math_sign_uint3>` 
  *  :ref:`abs (x: uint4) : uint4 <function-math_abs_uint4>` 
  *  :ref:`sign (x: uint4) : uint4 <function-math_sign_uint4>` 
  *  :ref:`abs (x: float) : float <function-math_abs_float>` 
  *  :ref:`sign (x: float) : float <function-math_sign_float>` 
  *  :ref:`abs (x: float2) : float2 <function-math_abs_float2>` 
  *  :ref:`sign (x: float2) : float2 <function-math_sign_float2>` 
  *  :ref:`abs (x: float3) : float3 <function-math_abs_float3>` 
  *  :ref:`sign (x: float3) : float3 <function-math_sign_float3>` 
  *  :ref:`abs (x: float4) : float4 <function-math_abs_float4>` 
  *  :ref:`sign (x: float4) : float4 <function-math_sign_float4>` 
  *  :ref:`abs (x: double) : double <function-math_abs_double>` 
  *  :ref:`sign (x: double) : double <function-math_sign_double>` 
  *  :ref:`abs (x: int64) : int64 <function-math_abs_int64>` 
  *  :ref:`sign (x: int64) : int64 <function-math_sign_int64>` 
  *  :ref:`abs (x: uint64) : uint64 <function-math_abs_uint64>` 
  *  :ref:`sign (x: uint64) : uint64 <function-math_sign_uint64>` 
  *  :ref:`is_nan (x: float) : bool <function-math_is_nan_float>` 
  *  :ref:`is_finite (x: float) : bool <function-math_is_finite_float>` 
  *  :ref:`is_nan (x: double) : bool <function-math_is_nan_double>` 
  *  :ref:`is_finite (x: double) : bool <function-math_is_finite_double>` 
  *  :ref:`sqrt (x: double) : double <function-math_sqrt_double>` 
  *  :ref:`exp (x: double) : double <function-math_exp_double>` 
  *  :ref:`rcp (x: double) : double <function-math_rcp_double>` 
  *  :ref:`log (x: double) : double <function-math_log_double>` 
  *  :ref:`pow (x: double; y: double) : double <function-math_pow_double_double>` 
  *  :ref:`exp2 (x: double) : double <function-math_exp2_double>` 
  *  :ref:`log2 (x: double) : double <function-math_log2_double>` 
  *  :ref:`sin (x: double) : double <function-math_sin_double>` 
  *  :ref:`cos (x: double) : double <function-math_cos_double>` 
  *  :ref:`asin (x: double) : double <function-math_asin_double>` 
  *  :ref:`acos (x: double) : double <function-math_acos_double>` 
  *  :ref:`safe_asin (x: double) : double <function-math_safe_asin_double>` 
  *  :ref:`safe_acos (x: double) : double <function-math_safe_acos_double>` 
  *  :ref:`tan (x: double) : double <function-math_tan_double>` 
  *  :ref:`atan (x: double) : double <function-math_atan_double>` 
  *  :ref:`atan2 (y: double; x: double) : double <function-math_atan2_double_double>` 
  *  :ref:`sincos (x: float; s: float& implicit; c: float& implicit) <function-math_sincos_float_float_implicit_float_implicit>` 
  *  :ref:`sincos (x: double; s: double& implicit; c: double& implicit) <function-math_sincos_double_double_implicit_double_implicit>` 
  *  :ref:`asin (x: float) : float <function-math_asin_float>` 
  *  :ref:`acos (x: float) : float <function-math_acos_float>` 
  *  :ref:`safe_asin (x: float) : float <function-math_safe_asin_float>` 
  *  :ref:`safe_acos (x: float) : float <function-math_safe_acos_float>` 
  *  :ref:`atan (x: float) : float <function-math_atan_float>` 
  *  :ref:`atan2 (y: float; x: float) : float <function-math_atan2_float_float>` 
  *  :ref:`asin (x: float2) : float2 <function-math_asin_float2>` 
  *  :ref:`asin (x: float3) : float3 <function-math_asin_float3>` 
  *  :ref:`asin (x: float4) : float4 <function-math_asin_float4>` 
  *  :ref:`acos (x: float2) : float2 <function-math_acos_float2>` 
  *  :ref:`acos (x: float3) : float3 <function-math_acos_float3>` 
  *  :ref:`acos (x: float4) : float4 <function-math_acos_float4>` 
  *  :ref:`safe_asin (x: float2) : float2 <function-math_safe_asin_float2>` 
  *  :ref:`safe_asin (x: float3) : float3 <function-math_safe_asin_float3>` 
  *  :ref:`safe_asin (x: float4) : float4 <function-math_safe_asin_float4>` 
  *  :ref:`safe_acos (x: float2) : float2 <function-math_safe_acos_float2>` 
  *  :ref:`safe_acos (x: float3) : float3 <function-math_safe_acos_float3>` 
  *  :ref:`safe_acos (x: float4) : float4 <function-math_safe_acos_float4>` 
  *  :ref:`atan (x: float2) : float2 <function-math_atan_float2>` 
  *  :ref:`atan (x: float3) : float3 <function-math_atan_float3>` 
  *  :ref:`atan (x: float4) : float4 <function-math_atan_float4>` 
  *  :ref:`atan2 (y: float2; x: float2) : float2 <function-math_atan2_float2_float2>` 
  *  :ref:`atan2 (y: float3; x: float3) : float3 <function-math_atan2_float3_float3>` 
  *  :ref:`atan2 (y: float4; x: float4) : float4 <function-math_atan2_float4_float4>` 

.. _function-math_sin_float:

.. das:function:: sin(x: float) : float

returns the sine of x

:Arguments: * **x** : float

.. _function-math_cos_float:

.. das:function:: cos(x: float) : float

returns the cosine of x

:Arguments: * **x** : float

.. _function-math_tan_float:

.. das:function:: tan(x: float) : float

returns the tangent of x

:Arguments: * **x** : float

.. _function-math_sin_float2:

.. das:function:: sin(x: float2) : float2

returns the sine of x

:Arguments: * **x** : float2

.. _function-math_cos_float2:

.. das:function:: cos(x: float2) : float2

returns the cosine of x

:Arguments: * **x** : float2

.. _function-math_tan_float2:

.. das:function:: tan(x: float2) : float2

returns the tangent of x

:Arguments: * **x** : float2

.. _function-math_sin_float3:

.. das:function:: sin(x: float3) : float3

returns the sine of x

:Arguments: * **x** : float3

.. _function-math_cos_float3:

.. das:function:: cos(x: float3) : float3

returns the cosine of x

:Arguments: * **x** : float3

.. _function-math_tan_float3:

.. das:function:: tan(x: float3) : float3

returns the tangent of x

:Arguments: * **x** : float3

.. _function-math_sin_float4:

.. das:function:: sin(x: float4) : float4

returns the sine of x

:Arguments: * **x** : float4

.. _function-math_cos_float4:

.. das:function:: cos(x: float4) : float4

returns the cosine of x

:Arguments: * **x** : float4

.. _function-math_tan_float4:

.. das:function:: tan(x: float4) : float4

returns the tangent of x

:Arguments: * **x** : float4

.. _function-math_exp_float:

.. das:function:: exp(x: float) : float

returns the e^x value of x

:Arguments: * **x** : float

.. _function-math_log_float:

.. das:function:: log(x: float) : float

returns the natural logarithm of x

:Arguments: * **x** : float

.. _function-math_exp2_float:

.. das:function:: exp2(x: float) : float

returns the 2^x value of x

:Arguments: * **x** : float

.. _function-math_log2_float:

.. das:function:: log2(x: float) : float

returns the logarithm base-2 of x

:Arguments: * **x** : float

.. _function-math_rcp_float:

.. das:function:: rcp(x: float) : float

Returns the reciprocal (1/x) of the argument. For vector types, computes per-component reciprocal.
:Arguments: * **x** : float

.. _function-math_pow_float_float:

.. das:function:: pow(x: float; y: float) : float

returns x raised to the power of y

:Arguments: * **x** : float

            * **y** : float

.. _function-math_exp_float2:

.. das:function:: exp(x: float2) : float2

returns the e^x value of x

:Arguments: * **x** : float2

.. _function-math_log_float2:

.. das:function:: log(x: float2) : float2

returns the natural logarithm of x

:Arguments: * **x** : float2

.. _function-math_exp2_float2:

.. das:function:: exp2(x: float2) : float2

returns the 2^x value of x

:Arguments: * **x** : float2

.. _function-math_log2_float2:

.. das:function:: log2(x: float2) : float2

returns the logarithm base-2 of x

:Arguments: * **x** : float2

.. _function-math_rcp_float2:

.. das:function:: rcp(x: float2) : float2

Returns the reciprocal (1/x) of the argument. For vector types, computes per-component reciprocal.
:Arguments: * **x** : float2

.. _function-math_pow_float2_float2:

.. das:function:: pow(x: float2; y: float2) : float2

returns x raised to the power of y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_exp_float3:

.. das:function:: exp(x: float3) : float3

returns the e^x value of x

:Arguments: * **x** : float3

.. _function-math_log_float3:

.. das:function:: log(x: float3) : float3

returns the natural logarithm of x

:Arguments: * **x** : float3

.. _function-math_exp2_float3:

.. das:function:: exp2(x: float3) : float3

returns the 2^x value of x

:Arguments: * **x** : float3

.. _function-math_log2_float3:

.. das:function:: log2(x: float3) : float3

returns the logarithm base-2 of x

:Arguments: * **x** : float3

.. _function-math_rcp_float3:

.. das:function:: rcp(x: float3) : float3

Returns the reciprocal (1/x) of the argument. For vector types, computes per-component reciprocal.
:Arguments: * **x** : float3

.. _function-math_pow_float3_float3:

.. das:function:: pow(x: float3; y: float3) : float3

returns x raised to the power of y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_exp_float4:

.. das:function:: exp(x: float4) : float4

returns the e^x value of x

:Arguments: * **x** : float4

.. _function-math_log_float4:

.. das:function:: log(x: float4) : float4

returns the natural logarithm of x

:Arguments: * **x** : float4

.. _function-math_exp2_float4:

.. das:function:: exp2(x: float4) : float4

returns the 2^x value of x

:Arguments: * **x** : float4

.. _function-math_log2_float4:

.. das:function:: log2(x: float4) : float4

returns the logarithm base-2 of x

:Arguments: * **x** : float4

.. _function-math_rcp_float4:

.. das:function:: rcp(x: float4) : float4

Returns the reciprocal (1/x) of the argument. For vector types, computes per-component reciprocal.
:Arguments: * **x** : float4

.. _function-math_pow_float4_float4:

.. das:function:: pow(x: float4; y: float4) : float4

returns x raised to the power of y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_floor_float:

.. das:function:: floor(x: float) : float

returns a float value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float

.. _function-math_ceil_float:

.. das:function:: ceil(x: float) : float

returns a float value representing the smallest integer (type is still float) that is greater than or equal to arg0

:Arguments: * **x** : float

.. _function-math_sqrt_float:

.. das:function:: sqrt(x: float) : float

returns the square root of x

:Arguments: * **x** : float

.. _function-math_saturate_float:

.. das:function:: saturate(x: float) : float

returns a clamped to [0..1] inclusive range x

:Arguments: * **x** : float

.. _function-math_floor_float2:

.. das:function:: floor(x: float2) : float2

returns a float value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float2

.. _function-math_ceil_float2:

.. das:function:: ceil(x: float2) : float2

returns a float value representing the smallest integer (type is still float) that is greater than or equal to arg0

:Arguments: * **x** : float2

.. _function-math_sqrt_float2:

.. das:function:: sqrt(x: float2) : float2

returns the square root of x

:Arguments: * **x** : float2

.. _function-math_saturate_float2:

.. das:function:: saturate(x: float2) : float2

returns a clamped to [0..1] inclusive range x

:Arguments: * **x** : float2

.. _function-math_floor_float3:

.. das:function:: floor(x: float3) : float3

returns a float value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float3

.. _function-math_ceil_float3:

.. das:function:: ceil(x: float3) : float3

returns a float value representing the smallest integer (type is still float) that is greater than or equal to arg0

:Arguments: * **x** : float3

.. _function-math_sqrt_float3:

.. das:function:: sqrt(x: float3) : float3

returns the square root of x

:Arguments: * **x** : float3

.. _function-math_saturate_float3:

.. das:function:: saturate(x: float3) : float3

returns a clamped to [0..1] inclusive range x

:Arguments: * **x** : float3

.. _function-math_floor_float4:

.. das:function:: floor(x: float4) : float4

returns a float value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float4

.. _function-math_ceil_float4:

.. das:function:: ceil(x: float4) : float4

returns a float value representing the smallest integer (type is still float) that is greater than or equal to arg0

:Arguments: * **x** : float4

.. _function-math_sqrt_float4:

.. das:function:: sqrt(x: float4) : float4

returns the square root of x

:Arguments: * **x** : float4

.. _function-math_saturate_float4:

.. das:function:: saturate(x: float4) : float4

returns a clamped to [0..1] inclusive range x

:Arguments: * **x** : float4

.. _function-math_abs_int:

.. das:function:: abs(x: int) : int

returns the absolute value of x

:Arguments: * **x** : int

.. _function-math_sign_int:

.. das:function:: sign(x: int) : int

returns sign of x, or 0 if x == 0

:Arguments: * **x** : int

.. _function-math_abs_int2:

.. das:function:: abs(x: int2) : int2

returns the absolute value of x

:Arguments: * **x** : int2

.. _function-math_sign_int2:

.. das:function:: sign(x: int2) : int2

returns sign of x, or 0 if x == 0

:Arguments: * **x** : int2

.. _function-math_abs_int3:

.. das:function:: abs(x: int3) : int3

returns the absolute value of x

:Arguments: * **x** : int3

.. _function-math_sign_int3:

.. das:function:: sign(x: int3) : int3

returns sign of x, or 0 if x == 0

:Arguments: * **x** : int3

.. _function-math_abs_int4:

.. das:function:: abs(x: int4) : int4

returns the absolute value of x

:Arguments: * **x** : int4

.. _function-math_sign_int4:

.. das:function:: sign(x: int4) : int4

returns sign of x, or 0 if x == 0

:Arguments: * **x** : int4

.. _function-math_abs_uint:

.. das:function:: abs(x: uint) : uint

returns the absolute value of x

:Arguments: * **x** : uint

.. _function-math_sign_uint:

.. das:function:: sign(x: uint) : uint

returns sign of x, or 0 if x == 0

:Arguments: * **x** : uint

.. _function-math_abs_uint2:

.. das:function:: abs(x: uint2) : uint2

returns the absolute value of x

:Arguments: * **x** : uint2

.. _function-math_sign_uint2:

.. das:function:: sign(x: uint2) : uint2

returns sign of x, or 0 if x == 0

:Arguments: * **x** : uint2

.. _function-math_abs_uint3:

.. das:function:: abs(x: uint3) : uint3

returns the absolute value of x

:Arguments: * **x** : uint3

.. _function-math_sign_uint3:

.. das:function:: sign(x: uint3) : uint3

returns sign of x, or 0 if x == 0

:Arguments: * **x** : uint3

.. _function-math_abs_uint4:

.. das:function:: abs(x: uint4) : uint4

returns the absolute value of x

:Arguments: * **x** : uint4

.. _function-math_sign_uint4:

.. das:function:: sign(x: uint4) : uint4

returns sign of x, or 0 if x == 0

:Arguments: * **x** : uint4

.. _function-math_abs_float:

.. das:function:: abs(x: float) : float

returns the absolute value of x

:Arguments: * **x** : float

.. _function-math_sign_float:

.. das:function:: sign(x: float) : float

returns sign of x, or 0 if x == 0

:Arguments: * **x** : float

.. _function-math_abs_float2:

.. das:function:: abs(x: float2) : float2

returns the absolute value of x

:Arguments: * **x** : float2

.. _function-math_sign_float2:

.. das:function:: sign(x: float2) : float2

returns sign of x, or 0 if x == 0

:Arguments: * **x** : float2

.. _function-math_abs_float3:

.. das:function:: abs(x: float3) : float3

returns the absolute value of x

:Arguments: * **x** : float3

.. _function-math_sign_float3:

.. das:function:: sign(x: float3) : float3

returns sign of x, or 0 if x == 0

:Arguments: * **x** : float3

.. _function-math_abs_float4:

.. das:function:: abs(x: float4) : float4

returns the absolute value of x

:Arguments: * **x** : float4

.. _function-math_sign_float4:

.. das:function:: sign(x: float4) : float4

returns sign of x, or 0 if x == 0

:Arguments: * **x** : float4

.. _function-math_abs_double:

.. das:function:: abs(x: double) : double

returns the absolute value of x

:Arguments: * **x** : double

.. _function-math_sign_double:

.. das:function:: sign(x: double) : double

returns sign of x, or 0 if x == 0

:Arguments: * **x** : double

.. _function-math_abs_int64:

.. das:function:: abs(x: int64) : int64

returns the absolute value of x

:Arguments: * **x** : int64

.. _function-math_sign_int64:

.. das:function:: sign(x: int64) : int64

returns sign of x, or 0 if x == 0

:Arguments: * **x** : int64

.. _function-math_abs_uint64:

.. das:function:: abs(x: uint64) : uint64

returns the absolute value of x

:Arguments: * **x** : uint64

.. _function-math_sign_uint64:

.. das:function:: sign(x: uint64) : uint64

returns sign of x, or 0 if x == 0

:Arguments: * **x** : uint64

.. _function-math_is_nan_float:

.. das:function:: is_nan(x: float) : bool

Returns true if `x` is NaN (not a number)

:Arguments: * **x** : float

.. _function-math_is_finite_float:

.. das:function:: is_finite(x: float) : bool

Returns true if `x` is not a negative or positive infinity

:Arguments: * **x** : float

.. _function-math_is_nan_double:

.. das:function:: is_nan(x: double) : bool

Returns true if `x` is NaN (not a number)

:Arguments: * **x** : double

.. _function-math_is_finite_double:

.. das:function:: is_finite(x: double) : bool

Returns true if `x` is not a negative or positive infinity

:Arguments: * **x** : double

.. _function-math_sqrt_double:

.. das:function:: sqrt(x: double) : double

returns the square root of x

:Arguments: * **x** : double

.. _function-math_exp_double:

.. das:function:: exp(x: double) : double

returns the e^x value of x

:Arguments: * **x** : double

.. _function-math_rcp_double:

.. das:function:: rcp(x: double) : double

Returns the reciprocal (1/x) of the argument. For vector types, computes per-component reciprocal.
:Arguments: * **x** : double

.. _function-math_log_double:

.. das:function:: log(x: double) : double

returns the natural logarithm of x

:Arguments: * **x** : double

.. _function-math_pow_double_double:

.. das:function:: pow(x: double; y: double) : double

returns x raised to the power of y

:Arguments: * **x** : double

            * **y** : double

.. _function-math_exp2_double:

.. das:function:: exp2(x: double) : double

returns the 2^x value of x

:Arguments: * **x** : double

.. _function-math_log2_double:

.. das:function:: log2(x: double) : double

returns the logarithm base-2 of x

:Arguments: * **x** : double

.. _function-math_sin_double:

.. das:function:: sin(x: double) : double

returns the sine of x

:Arguments: * **x** : double

.. _function-math_cos_double:

.. das:function:: cos(x: double) : double

returns the cosine of x

:Arguments: * **x** : double

.. _function-math_asin_double:

.. das:function:: asin(x: double) : double

returns the arcsine of x

:Arguments: * **x** : double

.. _function-math_acos_double:

.. das:function:: acos(x: double) : double

returns the arccosine of x

:Arguments: * **x** : double

.. _function-math_safe_asin_double:

.. das:function:: safe_asin(x: double) : double

Returns the arcsine of `x`, clamping `x` to the [-1..1] range to avoid NaN results.

:Arguments: * **x** : double

.. _function-math_safe_acos_double:

.. das:function:: safe_acos(x: double) : double

Returns the arc cosine of x, clamped to the range [-1, 1].

:Arguments: * **x** : double

.. _function-math_tan_double:

.. das:function:: tan(x: double) : double

returns the tangent of x

:Arguments: * **x** : double

.. _function-math_atan_double:

.. das:function:: atan(x: double) : double

returns the arctangent of x

:Arguments: * **x** : double

.. _function-math_atan2_double_double:

.. das:function:: atan2(y: double; x: double) : double

returns the arctangent of y/x

:Arguments: * **y** : double

            * **x** : double

.. _function-math_sincos_float_float_implicit_float_implicit:

.. das:function:: sincos(x: float; s: float& implicit; c: float& implicit)

returns oth sine and cosine of x

:Arguments: * **x** : float

            * **s** : float& implicit

            * **c** : float& implicit

.. _function-math_sincos_double_double_implicit_double_implicit:

.. das:function:: sincos(x: double; s: double& implicit; c: double& implicit)

returns oth sine and cosine of x

:Arguments: * **x** : double

            * **s** : double& implicit

            * **c** : double& implicit

.. _function-math_asin_float:

.. das:function:: asin(x: float) : float

returns the arcsine of x

:Arguments: * **x** : float

.. _function-math_acos_float:

.. das:function:: acos(x: float) : float

returns the arccosine of x

:Arguments: * **x** : float

.. _function-math_safe_asin_float:

.. das:function:: safe_asin(x: float) : float

Returns the arcsine of `x`, clamping `x` to the [-1..1] range to avoid NaN results.

:Arguments: * **x** : float

.. _function-math_safe_acos_float:

.. das:function:: safe_acos(x: float) : float

Returns the arc cosine of x, clamped to the range [-1, 1].

:Arguments: * **x** : float

.. _function-math_atan_float:

.. das:function:: atan(x: float) : float

returns the arctangent of x

:Arguments: * **x** : float

.. _function-math_atan2_float_float:

.. das:function:: atan2(y: float; x: float) : float

returns the arctangent of y/x

:Arguments: * **y** : float

            * **x** : float

.. _function-math_asin_float2:

.. das:function:: asin(x: float2) : float2

returns the arcsine of x

:Arguments: * **x** : float2

.. _function-math_asin_float3:

.. das:function:: asin(x: float3) : float3

returns the arcsine of x

:Arguments: * **x** : float3

.. _function-math_asin_float4:

.. das:function:: asin(x: float4) : float4

returns the arcsine of x

:Arguments: * **x** : float4

.. _function-math_acos_float2:

.. das:function:: acos(x: float2) : float2

returns the arccosine of x

:Arguments: * **x** : float2

.. _function-math_acos_float3:

.. das:function:: acos(x: float3) : float3

returns the arccosine of x

:Arguments: * **x** : float3

.. _function-math_acos_float4:

.. das:function:: acos(x: float4) : float4

returns the arccosine of x

:Arguments: * **x** : float4

.. _function-math_safe_asin_float2:

.. das:function:: safe_asin(x: float2) : float2

Returns the arcsine of `x`, clamping `x` to the [-1..1] range to avoid NaN results.

:Arguments: * **x** : float2

.. _function-math_safe_asin_float3:

.. das:function:: safe_asin(x: float3) : float3

Returns the arcsine of `x`, clamping `x` to the [-1..1] range to avoid NaN results.

:Arguments: * **x** : float3

.. _function-math_safe_asin_float4:

.. das:function:: safe_asin(x: float4) : float4

Returns the arcsine of `x`, clamping `x` to the [-1..1] range to avoid NaN results.

:Arguments: * **x** : float4

.. _function-math_safe_acos_float2:

.. das:function:: safe_acos(x: float2) : float2

returns the arc cosine of x, clamped to the range [-1, 1].

:Arguments: * **x** : float2

.. _function-math_safe_acos_float3:

.. das:function:: safe_acos(x: float3) : float3

Returns the arc cosine of x, clamped to the range [-1, 1].

:Arguments: * **x** : float3

.. _function-math_safe_acos_float4:

.. das:function:: safe_acos(x: float4) : float4

Returns the arc cosine of x, clamped to the range [-1, 1].

:Arguments: * **x** : float4

.. _function-math_atan_float2:

.. das:function:: atan(x: float2) : float2

returns the arctangent of x

:Arguments: * **x** : float2

.. _function-math_atan_float3:

.. das:function:: atan(x: float3) : float3

returns the arctangent of x

:Arguments: * **x** : float3

.. _function-math_atan_float4:

.. das:function:: atan(x: float4) : float4

returns the arctangent of x

:Arguments: * **x** : float4

.. _function-math_atan2_float2_float2:

.. das:function:: atan2(y: float2; x: float2) : float2

returns the arctangent of y/x

:Arguments: * **y** : float2

            * **x** : float2

.. _function-math_atan2_float3_float3:

.. das:function:: atan2(y: float3; x: float3) : float3

returns the arctangent of y/x

:Arguments: * **y** : float3

            * **x** : float3

.. _function-math_atan2_float4_float4:

.. das:function:: atan2(y: float4; x: float4) : float4

returns the arctangent of y/x

:Arguments: * **y** : float4

            * **x** : float4

+++++++++++
float* only
+++++++++++

  *  :ref:`rcp_est (x: float) : float <function-math_rcp_est_float>` 
  *  :ref:`rcp_est (x: float2) : float2 <function-math_rcp_est_float2>` 
  *  :ref:`rcp_est (x: float3) : float3 <function-math_rcp_est_float3>` 
  *  :ref:`rcp_est (x: float4) : float4 <function-math_rcp_est_float4>` 
  *  :ref:`fract (x: float) : float <function-math_fract_float>` 
  *  :ref:`round (x: float) : float <function-math_round_float>` 
  *  :ref:`rsqrt (x: float) : float <function-math_rsqrt_float>` 
  *  :ref:`rsqrt_est (x: float) : float <function-math_rsqrt_est_float>` 
  *  :ref:`fract (x: float2) : float2 <function-math_fract_float2>` 
  *  :ref:`round (x: float2) : float2 <function-math_round_float2>` 
  *  :ref:`rsqrt (x: float2) : float2 <function-math_rsqrt_float2>` 
  *  :ref:`rsqrt_est (x: float2) : float2 <function-math_rsqrt_est_float2>` 
  *  :ref:`fract (x: float3) : float3 <function-math_fract_float3>` 
  *  :ref:`round (x: float3) : float3 <function-math_round_float3>` 
  *  :ref:`rsqrt (x: float3) : float3 <function-math_rsqrt_float3>` 
  *  :ref:`rsqrt_est (x: float3) : float3 <function-math_rsqrt_est_float3>` 
  *  :ref:`fract (x: float4) : float4 <function-math_fract_float4>` 
  *  :ref:`round (x: float4) : float4 <function-math_round_float4>` 
  *  :ref:`rsqrt (x: float4) : float4 <function-math_rsqrt_float4>` 
  *  :ref:`rsqrt_est (x: float4) : float4 <function-math_rsqrt_est_float4>` 
  *  :ref:`atan_est (x: float) : float <function-math_atan_est_float>` 
  *  :ref:`atan2_est (y: float; x: float) : float <function-math_atan2_est_float_float>` 
  *  :ref:`atan_est (x: float2) : float2 <function-math_atan_est_float2>` 
  *  :ref:`atan_est (x: float3) : float3 <function-math_atan_est_float3>` 
  *  :ref:`atan_est (x: float4) : float4 <function-math_atan_est_float4>` 
  *  :ref:`atan2_est (y: float2; x: float2) : float2 <function-math_atan2_est_float2_float2>` 
  *  :ref:`atan2_est (y: float3; x: float3) : float3 <function-math_atan2_est_float3_float3>` 
  *  :ref:`atan2_est (y: float4; x: float4) : float4 <function-math_atan2_est_float4_float4>` 
  *  :ref:`floori (x: float) : int <function-math_floori_float>` 
  *  :ref:`ceili (x: float) : int <function-math_ceili_float>` 
  *  :ref:`roundi (x: float) : int <function-math_roundi_float>` 
  *  :ref:`trunci (x: float) : int <function-math_trunci_float>` 
  *  :ref:`floori (x: double) : int <function-math_floori_double>` 
  *  :ref:`ceili (x: double) : int <function-math_ceili_double>` 
  *  :ref:`roundi (x: double) : int <function-math_roundi_double>` 
  *  :ref:`trunci (x: double) : int <function-math_trunci_double>` 
  *  :ref:`floori (x: float2) : int2 <function-math_floori_float2>` 
  *  :ref:`ceili (x: float2) : int2 <function-math_ceili_float2>` 
  *  :ref:`roundi (x: float2) : int2 <function-math_roundi_float2>` 
  *  :ref:`trunci (x: float2) : int2 <function-math_trunci_float2>` 
  *  :ref:`floori (x: float3) : int3 <function-math_floori_float3>` 
  *  :ref:`ceili (x: float3) : int3 <function-math_ceili_float3>` 
  *  :ref:`roundi (x: float3) : int3 <function-math_roundi_float3>` 
  *  :ref:`trunci (x: float3) : int3 <function-math_trunci_float3>` 
  *  :ref:`floori (x: float4) : int4 <function-math_floori_float4>` 
  *  :ref:`ceili (x: float4) : int4 <function-math_ceili_float4>` 
  *  :ref:`roundi (x: float4) : int4 <function-math_roundi_float4>` 
  *  :ref:`trunci (x: float4) : int4 <function-math_trunci_float4>` 
  *  :ref:`float4x4 implicit- (x: float4x4 implicit) : float4x4 <function-math_-_float4x4_implicit>` 
  *  :ref:`float3x4 implicit- (x: float3x4 implicit) : float3x4 <function-math_-_float3x4_implicit>` 
  *  :ref:`float3x3 implicit- (x: float3x3 implicit) : float3x3 <function-math_-_float3x3_implicit>` 

.. _function-math_rcp_est_float:

.. das:function:: rcp_est(x: float) : float

returns the fast approximation 1/x

:Arguments: * **x** : float

.. _function-math_rcp_est_float2:

.. das:function:: rcp_est(x: float2) : float2

returns the fast approximation 1/x

:Arguments: * **x** : float2

.. _function-math_rcp_est_float3:

.. das:function:: rcp_est(x: float3) : float3

returns the fast approximation 1/x

:Arguments: * **x** : float3

.. _function-math_rcp_est_float4:

.. das:function:: rcp_est(x: float4) : float4

returns the fast approximation 1/x

:Arguments: * **x** : float4

.. _function-math_fract_float:

.. das:function:: fract(x: float) : float

returns a fraction part of x

:Arguments: * **x** : float

.. _function-math_round_float:

.. das:function:: round(x: float) : float

Returns the nearest integer to `x`.

:Arguments: * **x** : float

.. _function-math_rsqrt_float:

.. das:function:: rsqrt(x: float) : float

Returns the reciprocal square root (1/sqrt(x)) of the argument. For vector types, computes per-component reciprocal square root.
:Arguments: * **x** : float

.. _function-math_rsqrt_est_float:

.. das:function:: rsqrt_est(x: float) : float

returns the fast approximation 1/sqrt(x)

:Arguments: * **x** : float

.. _function-math_fract_float2:

.. das:function:: fract(x: float2) : float2

returns a fraction part of x

:Arguments: * **x** : float2

.. _function-math_round_float2:

.. das:function:: round(x: float2) : float2

Returns the nearest integer to `x`.

:Arguments: * **x** : float2

.. _function-math_rsqrt_float2:

.. das:function:: rsqrt(x: float2) : float2

Returns the reciprocal square root (1/sqrt(x)) of the argument. For vector types, computes per-component reciprocal square root.
:Arguments: * **x** : float2

.. _function-math_rsqrt_est_float2:

.. das:function:: rsqrt_est(x: float2) : float2

returns the fast approximation 1/sqrt(x)

:Arguments: * **x** : float2

.. _function-math_fract_float3:

.. das:function:: fract(x: float3) : float3

returns a fraction part of x

:Arguments: * **x** : float3

.. _function-math_round_float3:

.. das:function:: round(x: float3) : float3

Returns the nearest integer to `x`.

:Arguments: * **x** : float3

.. _function-math_rsqrt_float3:

.. das:function:: rsqrt(x: float3) : float3

Returns the reciprocal square root (1/sqrt(x)) of the argument. For vector types, computes per-component reciprocal square root.
:Arguments: * **x** : float3

.. _function-math_rsqrt_est_float3:

.. das:function:: rsqrt_est(x: float3) : float3

returns the fast approximation 1/sqrt(x)

:Arguments: * **x** : float3

.. _function-math_fract_float4:

.. das:function:: fract(x: float4) : float4

returns a fraction part of x

:Arguments: * **x** : float4

.. _function-math_round_float4:

.. das:function:: round(x: float4) : float4

Returns the nearest integer to `x`.

:Arguments: * **x** : float4

.. _function-math_rsqrt_float4:

.. das:function:: rsqrt(x: float4) : float4

Returns the reciprocal square root (1/sqrt(x)) of the argument. For vector types, computes per-component reciprocal square root.
:Arguments: * **x** : float4

.. _function-math_rsqrt_est_float4:

.. das:function:: rsqrt_est(x: float4) : float4

returns the fast approximation 1/sqrt(x)

:Arguments: * **x** : float4

.. _function-math_atan_est_float:

.. das:function:: atan_est(x: float) : float

Fast estimation for the `atan`.

:Arguments: * **x** : float

.. _function-math_atan2_est_float_float:

.. das:function:: atan2_est(y: float; x: float) : float

returns the fast approximation of arctangent of y/x

:Arguments: * **y** : float

            * **x** : float

.. _function-math_atan_est_float2:

.. das:function:: atan_est(x: float2) : float2

Fast estimation for the `atan`.

:Arguments: * **x** : float2

.. _function-math_atan_est_float3:

.. das:function:: atan_est(x: float3) : float3

Fast estimation for the `atan`.

:Arguments: * **x** : float3

.. _function-math_atan_est_float4:

.. das:function:: atan_est(x: float4) : float4

Fast estimation for the `atan`.

:Arguments: * **x** : float4

.. _function-math_atan2_est_float2_float2:

.. das:function:: atan2_est(y: float2; x: float2) : float2

returns the fast approximation of arctangent of y/x

:Arguments: * **y** : float2

            * **x** : float2

.. _function-math_atan2_est_float3_float3:

.. das:function:: atan2_est(y: float3; x: float3) : float3

returns the fast approximation of arctangent of y/x

:Arguments: * **y** : float3

            * **x** : float3

.. _function-math_atan2_est_float4_float4:

.. das:function:: atan2_est(y: float4; x: float4) : float4

returns the fast approximation of arctangent of y/x

:Arguments: * **y** : float4

            * **x** : float4

.. _function-math_floori_float:

.. das:function:: floori(x: float) : int

returns a integer value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float

.. _function-math_ceili_float:

.. das:function:: ceili(x: float) : int

returns a value representing the smallest integer (integer type!) that is greater than or equal to arg0

:Arguments: * **x** : float

.. _function-math_roundi_float:

.. das:function:: roundi(x: float) : int

returns a integer value representing the integer that is closest to x

:Arguments: * **x** : float

.. _function-math_trunci_float:

.. das:function:: trunci(x: float) : int

returns a integer value representing the float without fraction part of x

:Arguments: * **x** : float

.. _function-math_floori_double:

.. das:function:: floori(x: double) : int

returns a integer value representing the largest integer that is less than or equal to x

:Arguments: * **x** : double

.. _function-math_ceili_double:

.. das:function:: ceili(x: double) : int

returns a value representing the smallest integer (integer type!) that is greater than or equal to arg0

:Arguments: * **x** : double

.. _function-math_roundi_double:

.. das:function:: roundi(x: double) : int

returns a integer value representing the integer that is closest to x

:Arguments: * **x** : double

.. _function-math_trunci_double:

.. das:function:: trunci(x: double) : int

returns a integer value representing the float without fraction part of x

:Arguments: * **x** : double

.. _function-math_floori_float2:

.. das:function:: floori(x: float2) : int2

returns a integer value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float2

.. _function-math_ceili_float2:

.. das:function:: ceili(x: float2) : int2

returns a value representing the smallest integer (integer type!) that is greater than or equal to arg0

:Arguments: * **x** : float2

.. _function-math_roundi_float2:

.. das:function:: roundi(x: float2) : int2

returns a integer value representing the integer that is closest to x

:Arguments: * **x** : float2

.. _function-math_trunci_float2:

.. das:function:: trunci(x: float2) : int2

returns a integer value representing the float without fraction part of x

:Arguments: * **x** : float2

.. _function-math_floori_float3:

.. das:function:: floori(x: float3) : int3

returns a integer value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float3

.. _function-math_ceili_float3:

.. das:function:: ceili(x: float3) : int3

returns a value representing the smallest integer (integer type!) that is greater than or equal to arg0

:Arguments: * **x** : float3

.. _function-math_roundi_float3:

.. das:function:: roundi(x: float3) : int3

returns a integer value representing the integer that is closest to x

:Arguments: * **x** : float3

.. _function-math_trunci_float3:

.. das:function:: trunci(x: float3) : int3

returns a integer value representing the float without fraction part of x

:Arguments: * **x** : float3

.. _function-math_floori_float4:

.. das:function:: floori(x: float4) : int4

returns a integer value representing the largest integer that is less than or equal to x

:Arguments: * **x** : float4

.. _function-math_ceili_float4:

.. das:function:: ceili(x: float4) : int4

returns a value representing the smallest integer (integer type!) that is greater than or equal to arg0

:Arguments: * **x** : float4

.. _function-math_roundi_float4:

.. das:function:: roundi(x: float4) : int4

returns a integer value representing the integer that is closest to x

:Arguments: * **x** : float4

.. _function-math_trunci_float4:

.. das:function:: trunci(x: float4) : int4

returns a integer value representing the float without fraction part of x

:Arguments: * **x** : float4

.. _function-math_-_float4x4_implicit:

.. das:function:: float4x4 implicit-(x: float4x4 implicit) : float4x4

Returns the arithmetic negation of the argument (unary minus). Works with scalar and vector types.
:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_-_float3x4_implicit:

.. das:function:: float3x4 implicit-(x: float3x4 implicit) : float3x4

Returns the arithmetic negation of the argument (unary minus). Works with scalar and vector types.
:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_-_float3x3_implicit:

.. das:function:: float3x3 implicit-(x: float3x3 implicit) : float3x3

Returns the arithmetic negation of the argument (unary minus). Works with scalar and vector types.
:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

+++++++++++
float3 only
+++++++++++

  *  :ref:`cross (x: float3; y: float3) : float3 <function-math_cross_float3_float3>` 
  *  :ref:`distance (x: float2; y: float2) : float <function-math_distance_float2_float2>` 
  *  :ref:`distance_sq (x: float2; y: float2) : float <function-math_distance_sq_float2_float2>` 
  *  :ref:`inv_distance (x: float2; y: float2) : float <function-math_inv_distance_float2_float2>` 
  *  :ref:`inv_distance_sq (x: float2; y: float2) : float <function-math_inv_distance_sq_float2_float2>` 
  *  :ref:`distance (x: float3; y: float3) : float <function-math_distance_float3_float3>` 
  *  :ref:`distance_sq (x: float3; y: float3) : float <function-math_distance_sq_float3_float3>` 
  *  :ref:`inv_distance (x: float3; y: float3) : float <function-math_inv_distance_float3_float3>` 
  *  :ref:`inv_distance_sq (x: float3; y: float3) : float <function-math_inv_distance_sq_float3_float3>` 
  *  :ref:`distance (x: float4; y: float4) : float <function-math_distance_float4_float4>` 
  *  :ref:`distance_sq (x: float4; y: float4) : float <function-math_distance_sq_float4_float4>` 
  *  :ref:`inv_distance (x: float4; y: float4) : float <function-math_inv_distance_float4_float4>` 
  *  :ref:`inv_distance_sq (x: float4; y: float4) : float <function-math_inv_distance_sq_float4_float4>` 
  *  :ref:`reflect (v: float3; n: float3) : float3 <function-math_reflect_float3_float3>` 
  *  :ref:`reflect (v: float2; n: float2) : float2 <function-math_reflect_float2_float2>` 
  *  :ref:`refract (v: float3; n: float3; nint: float) : float3 <function-math_refract_float3_float3_float>` 
  *  :ref:`refract (v: float2; n: float2; nint: float) : float2 <function-math_refract_float2_float2_float>` 

.. _function-math_cross_float3_float3:

.. das:function:: cross(x: float3; y: float3) : float3

returns vector representing cross product between x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_distance_float2_float2:

.. das:function:: distance(x: float2; y: float2) : float

returns a non-negative value representing distance between x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_distance_sq_float2_float2:

.. das:function:: distance_sq(x: float2; y: float2) : float

returns a non-negative value representing squared distance between x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_inv_distance_float2_float2:

.. das:function:: inv_distance(x: float2; y: float2) : float

returns a non-negative value representing 1/distance between x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_inv_distance_sq_float2_float2:

.. das:function:: inv_distance_sq(x: float2; y: float2) : float

returns a non-negative value representing 1/squared distance between x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_distance_float3_float3:

.. das:function:: distance(x: float3; y: float3) : float

returns a non-negative value representing distance between x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_distance_sq_float3_float3:

.. das:function:: distance_sq(x: float3; y: float3) : float

returns a non-negative value representing squared distance between x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_inv_distance_float3_float3:

.. das:function:: inv_distance(x: float3; y: float3) : float

returns a non-negative value representing 1/distance between x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_inv_distance_sq_float3_float3:

.. das:function:: inv_distance_sq(x: float3; y: float3) : float

returns a non-negative value representing 1/squared distance between x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_distance_float4_float4:

.. das:function:: distance(x: float4; y: float4) : float

returns a non-negative value representing distance between x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_distance_sq_float4_float4:

.. das:function:: distance_sq(x: float4; y: float4) : float

returns a non-negative value representing squared distance between x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_inv_distance_float4_float4:

.. das:function:: inv_distance(x: float4; y: float4) : float

returns a non-negative value representing 1/distance between x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_inv_distance_sq_float4_float4:

.. das:function:: inv_distance_sq(x: float4; y: float4) : float

returns a non-negative value representing 1/squared distance between x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_reflect_float3_float3:

.. das:function:: reflect(v: float3; n: float3) : float3

see function-math-reflect.rst for details

:Arguments: * **v** : float3

            * **n** : float3

.. _function-math_reflect_float2_float2:

.. das:function:: reflect(v: float2; n: float2) : float2

see function-math-reflect.rst for details

:Arguments: * **v** : float2

            * **n** : float2

.. _function-math_refract_float3_float3_float:

.. das:function:: refract(v: float3; n: float3; nint: float) : float3

see function-math-refract.rst for details

:Arguments: * **v** : float3

            * **n** : float3

            * **nint** : float

.. _function-math_refract_float2_float2_float:

.. das:function:: refract(v: float2; n: float2; nint: float) : float2

see function-math-refract.rst for details

:Arguments: * **v** : float2

            * **n** : float2

            * **nint** : float

++++++++++++++++++++++
float2, float3, float4
++++++++++++++++++++++

  *  :ref:`dot (x: float2; y: float2) : float <function-math_dot_float2_float2>` 
  *  :ref:`dot (x: float3; y: float3) : float <function-math_dot_float3_float3>` 
  *  :ref:`dot (x: float4; y: float4) : float <function-math_dot_float4_float4>` 
  *  :ref:`fast_normalize (x: float2) : float2 <function-math_fast_normalize_float2>` 
  *  :ref:`fast_normalize (x: float3) : float3 <function-math_fast_normalize_float3>` 
  *  :ref:`fast_normalize (x: float4) : float4 <function-math_fast_normalize_float4>` 
  *  :ref:`normalize (x: float2) : float2 <function-math_normalize_float2>` 
  *  :ref:`normalize (x: float3) : float3 <function-math_normalize_float3>` 
  *  :ref:`normalize (x: float4) : float4 <function-math_normalize_float4>` 
  *  :ref:`length (x: float2) : float <function-math_length_float2>` 
  *  :ref:`length (x: float3) : float <function-math_length_float3>` 
  *  :ref:`length (x: float4) : float <function-math_length_float4>` 
  *  :ref:`inv_length (x: float2) : float <function-math_inv_length_float2>` 
  *  :ref:`inv_length (x: float3) : float <function-math_inv_length_float3>` 
  *  :ref:`inv_length (x: float4) : float <function-math_inv_length_float4>` 
  *  :ref:`inv_length_sq (x: float2) : float <function-math_inv_length_sq_float2>` 
  *  :ref:`inv_length_sq (x: float3) : float <function-math_inv_length_sq_float3>` 
  *  :ref:`inv_length_sq (x: float4) : float <function-math_inv_length_sq_float4>` 
  *  :ref:`length_sq (x: float2) : float <function-math_length_sq_float2>` 
  *  :ref:`length_sq (x: float3) : float <function-math_length_sq_float3>` 
  *  :ref:`length_sq (x: float4) : float <function-math_length_sq_float4>` 

.. _function-math_dot_float2_float2:

.. das:function:: dot(x: float2; y: float2) : float

returns scalar representing dot product between x and y

:Arguments: * **x** : float2

            * **y** : float2

.. _function-math_dot_float3_float3:

.. das:function:: dot(x: float3; y: float3) : float

returns scalar representing dot product between x and y

:Arguments: * **x** : float3

            * **y** : float3

.. _function-math_dot_float4_float4:

.. das:function:: dot(x: float4; y: float4) : float

returns scalar representing dot product between x and y

:Arguments: * **x** : float4

            * **y** : float4

.. _function-math_fast_normalize_float2:

.. das:function:: fast_normalize(x: float2) : float2

returns the fast approximation of normalized x, or nan if length(x) is 0

:Arguments: * **x** : float2

.. _function-math_fast_normalize_float3:

.. das:function:: fast_normalize(x: float3) : float3

returns the fast approximation of normalized x, or nan if length(x) is 0

:Arguments: * **x** : float3

.. _function-math_fast_normalize_float4:

.. das:function:: fast_normalize(x: float4) : float4

returns the fast approximation of normalized x, or nan if length(x) is 0

:Arguments: * **x** : float4

.. _function-math_normalize_float2:

.. das:function:: normalize(x: float2) : float2

returns normalized x, or nan if length(x) is 0

:Arguments: * **x** : float2

.. _function-math_normalize_float3:

.. das:function:: normalize(x: float3) : float3

returns normalized x, or nan if length(x) is 0

:Arguments: * **x** : float3

.. _function-math_normalize_float4:

.. das:function:: normalize(x: float4) : float4

returns normalized x, or nan if length(x) is 0

:Arguments: * **x** : float4

.. _function-math_length_float2:

.. das:function:: length(x: float2) : float

returns a non-negative value representing magnitude of x

:Arguments: * **x** : float2

.. _function-math_length_float3:

.. das:function:: length(x: float3) : float

returns a non-negative value representing magnitude of x

:Arguments: * **x** : float3

.. _function-math_length_float4:

.. das:function:: length(x: float4) : float

returns a non-negative value representing magnitude of x

:Arguments: * **x** : float4

.. _function-math_inv_length_float2:

.. das:function:: inv_length(x: float2) : float

returns a non-negative value representing 1/magnitude of x

:Arguments: * **x** : float2

.. _function-math_inv_length_float3:

.. das:function:: inv_length(x: float3) : float

returns a non-negative value representing 1/magnitude of x

:Arguments: * **x** : float3

.. _function-math_inv_length_float4:

.. das:function:: inv_length(x: float4) : float

returns a non-negative value representing 1/magnitude of x

:Arguments: * **x** : float4

.. _function-math_inv_length_sq_float2:

.. das:function:: inv_length_sq(x: float2) : float

returns a non-negative value representing 1/squared magnitude of x

:Arguments: * **x** : float2

.. _function-math_inv_length_sq_float3:

.. das:function:: inv_length_sq(x: float3) : float

returns a non-negative value representing 1/squared magnitude of x

:Arguments: * **x** : float3

.. _function-math_inv_length_sq_float4:

.. das:function:: inv_length_sq(x: float4) : float

returns a non-negative value representing 1/squared magnitude of x

:Arguments: * **x** : float4

.. _function-math_length_sq_float2:

.. das:function:: length_sq(x: float2) : float

returns a non-negative value representing squared magnitude of x

:Arguments: * **x** : float2

.. _function-math_length_sq_float3:

.. das:function:: length_sq(x: float3) : float

returns a non-negative value representing squared magnitude of x

:Arguments: * **x** : float3

.. _function-math_length_sq_float4:

.. das:function:: length_sq(x: float4) : float

returns a non-negative value representing squared magnitude of x

:Arguments: * **x** : float4

+++++++++++++++
Noise functions
+++++++++++++++

  *  :ref:`uint32_hash (seed: uint) : uint <function-math_uint32_hash_uint>` 
  *  :ref:`uint_noise_1D (position: int; seed: uint) : uint <function-math_uint_noise_1D_int_uint>` 
  *  :ref:`uint_noise_2D (position: int2; seed: uint) : uint <function-math_uint_noise_2D_int2_uint>` 
  *  :ref:`uint_noise_3D (position: int3; seed: uint) : uint <function-math_uint_noise_3D_int3_uint>` 

.. _function-math_uint32_hash_uint:

.. das:function:: uint32_hash(seed: uint) : uint

returns hashed value of seed

:Arguments: * **seed** : uint

.. _function-math_uint_noise_1D_int_uint:

.. das:function:: uint_noise_1D(position: int; seed: uint) : uint

returns noise value of position in the seeded sequence

:Arguments: * **position** : int

            * **seed** : uint

.. _function-math_uint_noise_2D_int2_uint:

.. das:function:: uint_noise_2D(position: int2; seed: uint) : uint

returns noise value of position in the seeded sequence

:Arguments: * **position** : int2

            * **seed** : uint

.. _function-math_uint_noise_3D_int3_uint:

.. das:function:: uint_noise_3D(position: int3; seed: uint) : uint

returns noise value of position in the seeded sequence

:Arguments: * **position** : int3

            * **seed** : uint

++++++++++++++
lerp/mad/clamp
++++++++++++++

  *  :ref:`mad (a: float; b: float; c: float) : float <function-math_mad_float_float_float>` 
  *  :ref:`lerp (a: float; b: float; t: float) : float <function-math_lerp_float_float_float>` 
  *  :ref:`mad (a: float2; b: float2; c: float2) : float2 <function-math_mad_float2_float2_float2>` 
  *  :ref:`lerp (a: float2; b: float2; t: float2) : float2 <function-math_lerp_float2_float2_float2>` 
  *  :ref:`mad (a: float3; b: float3; c: float3) : float3 <function-math_mad_float3_float3_float3>` 
  *  :ref:`lerp (a: float3; b: float3; t: float3) : float3 <function-math_lerp_float3_float3_float3>` 
  *  :ref:`mad (a: float4; b: float4; c: float4) : float4 <function-math_mad_float4_float4_float4>` 
  *  :ref:`lerp (a: float4; b: float4; t: float4) : float4 <function-math_lerp_float4_float4_float4>` 
  *  :ref:`mad (a: float2; b: float; c: float2) : float2 <function-math_mad_float2_float_float2>` 
  *  :ref:`mad (a: float3; b: float; c: float3) : float3 <function-math_mad_float3_float_float3>` 
  *  :ref:`mad (a: float4; b: float; c: float4) : float4 <function-math_mad_float4_float_float4>` 
  *  :ref:`mad (a: int; b: int; c: int) : int <function-math_mad_int_int_int>` 
  *  :ref:`mad (a: int2; b: int2; c: int2) : int2 <function-math_mad_int2_int2_int2>` 
  *  :ref:`mad (a: int3; b: int3; c: int3) : int3 <function-math_mad_int3_int3_int3>` 
  *  :ref:`mad (a: int4; b: int4; c: int4) : int4 <function-math_mad_int4_int4_int4>` 
  *  :ref:`mad (a: int2; b: int; c: int2) : int2 <function-math_mad_int2_int_int2>` 
  *  :ref:`mad (a: int3; b: int; c: int3) : int3 <function-math_mad_int3_int_int3>` 
  *  :ref:`mad (a: int4; b: int; c: int4) : int4 <function-math_mad_int4_int_int4>` 
  *  :ref:`mad (a: uint; b: uint; c: uint) : uint <function-math_mad_uint_uint_uint>` 
  *  :ref:`mad (a: uint2; b: uint2; c: uint2) : uint2 <function-math_mad_uint2_uint2_uint2>` 
  *  :ref:`mad (a: uint3; b: uint3; c: uint3) : uint3 <function-math_mad_uint3_uint3_uint3>` 
  *  :ref:`mad (a: uint4; b: uint4; c: uint4) : uint4 <function-math_mad_uint4_uint4_uint4>` 
  *  :ref:`mad (a: uint2; b: uint; c: uint2) : uint2 <function-math_mad_uint2_uint_uint2>` 
  *  :ref:`mad (a: uint3; b: uint; c: uint3) : uint3 <function-math_mad_uint3_uint_uint3>` 
  *  :ref:`mad (a: uint4; b: uint; c: uint4) : uint4 <function-math_mad_uint4_uint_uint4>` 
  *  :ref:`mad (a: double; b: double; c: double) : double <function-math_mad_double_double_double>` 
  *  :ref:`lerp (a: double; b: double; t: double) : double <function-math_lerp_double_double_double>` 
  *  :ref:`clamp (t: int; a: int; b: int) : int <function-math_clamp_int_int_int>` 
  *  :ref:`clamp (t: int2; a: int2; b: int2) : int2 <function-math_clamp_int2_int2_int2>` 
  *  :ref:`clamp (t: int3; a: int3; b: int3) : int3 <function-math_clamp_int3_int3_int3>` 
  *  :ref:`clamp (t: int4; a: int4; b: int4) : int4 <function-math_clamp_int4_int4_int4>` 
  *  :ref:`clamp (t: uint; a: uint; b: uint) : uint <function-math_clamp_uint_uint_uint>` 
  *  :ref:`clamp (t: uint2; a: uint2; b: uint2) : uint2 <function-math_clamp_uint2_uint2_uint2>` 
  *  :ref:`clamp (t: uint3; a: uint3; b: uint3) : uint3 <function-math_clamp_uint3_uint3_uint3>` 
  *  :ref:`clamp (t: uint4; a: uint4; b: uint4) : uint4 <function-math_clamp_uint4_uint4_uint4>` 
  *  :ref:`clamp (t: float; a: float; b: float) : float <function-math_clamp_float_float_float>` 
  *  :ref:`clamp (t: float2; a: float2; b: float2) : float2 <function-math_clamp_float2_float2_float2>` 
  *  :ref:`clamp (t: float3; a: float3; b: float3) : float3 <function-math_clamp_float3_float3_float3>` 
  *  :ref:`clamp (t: float4; a: float4; b: float4) : float4 <function-math_clamp_float4_float4_float4>` 
  *  :ref:`clamp (t: double; a: double; b: double) : double <function-math_clamp_double_double_double>` 
  *  :ref:`clamp (t: int64; a: int64; b: int64) : int64 <function-math_clamp_int64_int64_int64>` 
  *  :ref:`clamp (t: uint64; a: uint64; b: uint64) : uint64 <function-math_clamp_uint64_uint64_uint64>` 
  *  :ref:`lerp (a: float2; b: float2; t: float) : float2 <function-math_lerp_float2_float2_float>` 
  *  :ref:`lerp (a: float3; b: float3; t: float) : float3 <function-math_lerp_float3_float3_float>` 
  *  :ref:`lerp (a: float4; b: float4; t: float) : float4 <function-math_lerp_float4_float4_float>` 

.. _function-math_mad_float_float_float:

.. das:function:: mad(a: float; b: float; c: float) : float

returns vector or scalar representing a * b + c

:Arguments: * **a** : float

            * **b** : float

            * **c** : float

.. _function-math_lerp_float_float_float:

.. das:function:: lerp(a: float; b: float; t: float) : float

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float

            * **b** : float

            * **t** : float

.. _function-math_mad_float2_float2_float2:

.. das:function:: mad(a: float2; b: float2; c: float2) : float2

returns vector or scalar representing a * b + c

:Arguments: * **a** : float2

            * **b** : float2

            * **c** : float2

.. _function-math_lerp_float2_float2_float2:

.. das:function:: lerp(a: float2; b: float2; t: float2) : float2

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float2

            * **b** : float2

            * **t** : float2

.. _function-math_mad_float3_float3_float3:

.. das:function:: mad(a: float3; b: float3; c: float3) : float3

returns vector or scalar representing a * b + c

:Arguments: * **a** : float3

            * **b** : float3

            * **c** : float3

.. _function-math_lerp_float3_float3_float3:

.. das:function:: lerp(a: float3; b: float3; t: float3) : float3

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float3

            * **b** : float3

            * **t** : float3

.. _function-math_mad_float4_float4_float4:

.. das:function:: mad(a: float4; b: float4; c: float4) : float4

returns vector or scalar representing a * b + c

:Arguments: * **a** : float4

            * **b** : float4

            * **c** : float4

.. _function-math_lerp_float4_float4_float4:

.. das:function:: lerp(a: float4; b: float4; t: float4) : float4

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float4

            * **b** : float4

            * **t** : float4

.. _function-math_mad_float2_float_float2:

.. das:function:: mad(a: float2; b: float; c: float2) : float2

returns vector or scalar representing a * b + c

:Arguments: * **a** : float2

            * **b** : float

            * **c** : float2

.. _function-math_mad_float3_float_float3:

.. das:function:: mad(a: float3; b: float; c: float3) : float3

returns vector or scalar representing a * b + c

:Arguments: * **a** : float3

            * **b** : float

            * **c** : float3

.. _function-math_mad_float4_float_float4:

.. das:function:: mad(a: float4; b: float; c: float4) : float4

returns vector or scalar representing a * b + c

:Arguments: * **a** : float4

            * **b** : float

            * **c** : float4

.. _function-math_mad_int_int_int:

.. das:function:: mad(a: int; b: int; c: int) : int

returns vector or scalar representing a * b + c

:Arguments: * **a** : int

            * **b** : int

            * **c** : int

.. _function-math_mad_int2_int2_int2:

.. das:function:: mad(a: int2; b: int2; c: int2) : int2

returns vector or scalar representing a * b + c

:Arguments: * **a** : int2

            * **b** : int2

            * **c** : int2

.. _function-math_mad_int3_int3_int3:

.. das:function:: mad(a: int3; b: int3; c: int3) : int3

returns vector or scalar representing a * b + c

:Arguments: * **a** : int3

            * **b** : int3

            * **c** : int3

.. _function-math_mad_int4_int4_int4:

.. das:function:: mad(a: int4; b: int4; c: int4) : int4

returns vector or scalar representing a * b + c

:Arguments: * **a** : int4

            * **b** : int4

            * **c** : int4

.. _function-math_mad_int2_int_int2:

.. das:function:: mad(a: int2; b: int; c: int2) : int2

returns vector or scalar representing a * b + c

:Arguments: * **a** : int2

            * **b** : int

            * **c** : int2

.. _function-math_mad_int3_int_int3:

.. das:function:: mad(a: int3; b: int; c: int3) : int3

returns vector or scalar representing a * b + c

:Arguments: * **a** : int3

            * **b** : int

            * **c** : int3

.. _function-math_mad_int4_int_int4:

.. das:function:: mad(a: int4; b: int; c: int4) : int4

returns vector or scalar representing a * b + c

:Arguments: * **a** : int4

            * **b** : int

            * **c** : int4

.. _function-math_mad_uint_uint_uint:

.. das:function:: mad(a: uint; b: uint; c: uint) : uint

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint

            * **b** : uint

            * **c** : uint

.. _function-math_mad_uint2_uint2_uint2:

.. das:function:: mad(a: uint2; b: uint2; c: uint2) : uint2

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint2

            * **b** : uint2

            * **c** : uint2

.. _function-math_mad_uint3_uint3_uint3:

.. das:function:: mad(a: uint3; b: uint3; c: uint3) : uint3

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint3

            * **b** : uint3

            * **c** : uint3

.. _function-math_mad_uint4_uint4_uint4:

.. das:function:: mad(a: uint4; b: uint4; c: uint4) : uint4

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint4

            * **b** : uint4

            * **c** : uint4

.. _function-math_mad_uint2_uint_uint2:

.. das:function:: mad(a: uint2; b: uint; c: uint2) : uint2

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint2

            * **b** : uint

            * **c** : uint2

.. _function-math_mad_uint3_uint_uint3:

.. das:function:: mad(a: uint3; b: uint; c: uint3) : uint3

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint3

            * **b** : uint

            * **c** : uint3

.. _function-math_mad_uint4_uint_uint4:

.. das:function:: mad(a: uint4; b: uint; c: uint4) : uint4

returns vector or scalar representing a * b + c

:Arguments: * **a** : uint4

            * **b** : uint

            * **c** : uint4

.. _function-math_mad_double_double_double:

.. das:function:: mad(a: double; b: double; c: double) : double

returns vector or scalar representing a * b + c

:Arguments: * **a** : double

            * **b** : double

            * **c** : double

.. _function-math_lerp_double_double_double:

.. das:function:: lerp(a: double; b: double; t: double) : double

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : double

            * **b** : double

            * **t** : double

.. _function-math_clamp_int_int_int:

.. das:function:: clamp(t: int; a: int; b: int) : int

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : int

            * **a** : int

            * **b** : int

.. _function-math_clamp_int2_int2_int2:

.. das:function:: clamp(t: int2; a: int2; b: int2) : int2

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : int2

            * **a** : int2

            * **b** : int2

.. _function-math_clamp_int3_int3_int3:

.. das:function:: clamp(t: int3; a: int3; b: int3) : int3

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : int3

            * **a** : int3

            * **b** : int3

.. _function-math_clamp_int4_int4_int4:

.. das:function:: clamp(t: int4; a: int4; b: int4) : int4

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : int4

            * **a** : int4

            * **b** : int4

.. _function-math_clamp_uint_uint_uint:

.. das:function:: clamp(t: uint; a: uint; b: uint) : uint

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : uint

            * **a** : uint

            * **b** : uint

.. _function-math_clamp_uint2_uint2_uint2:

.. das:function:: clamp(t: uint2; a: uint2; b: uint2) : uint2

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : uint2

            * **a** : uint2

            * **b** : uint2

.. _function-math_clamp_uint3_uint3_uint3:

.. das:function:: clamp(t: uint3; a: uint3; b: uint3) : uint3

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : uint3

            * **a** : uint3

            * **b** : uint3

.. _function-math_clamp_uint4_uint4_uint4:

.. das:function:: clamp(t: uint4; a: uint4; b: uint4) : uint4

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : uint4

            * **a** : uint4

            * **b** : uint4

.. _function-math_clamp_float_float_float:

.. das:function:: clamp(t: float; a: float; b: float) : float

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : float

            * **a** : float

            * **b** : float

.. _function-math_clamp_float2_float2_float2:

.. das:function:: clamp(t: float2; a: float2; b: float2) : float2

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : float2

            * **a** : float2

            * **b** : float2

.. _function-math_clamp_float3_float3_float3:

.. das:function:: clamp(t: float3; a: float3; b: float3) : float3

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : float3

            * **a** : float3

            * **b** : float3

.. _function-math_clamp_float4_float4_float4:

.. das:function:: clamp(t: float4; a: float4; b: float4) : float4

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : float4

            * **a** : float4

            * **b** : float4

.. _function-math_clamp_double_double_double:

.. das:function:: clamp(t: double; a: double; b: double) : double

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : double

            * **a** : double

            * **b** : double

.. _function-math_clamp_int64_int64_int64:

.. das:function:: clamp(t: int64; a: int64; b: int64) : int64

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : int64

            * **a** : int64

            * **b** : int64

.. _function-math_clamp_uint64_uint64_uint64:

.. das:function:: clamp(t: uint64; a: uint64; b: uint64) : uint64

returns vector or scalar representing min(max(t, a), b)

:Arguments: * **t** : uint64

            * **a** : uint64

            * **b** : uint64

.. _function-math_lerp_float2_float2_float:

.. das:function:: lerp(a: float2; b: float2; t: float) : float2

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float2

            * **b** : float2

            * **t** : float

.. _function-math_lerp_float3_float3_float:

.. das:function:: lerp(a: float3; b: float3; t: float) : float3

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float3

            * **b** : float3

            * **t** : float

.. _function-math_lerp_float4_float4_float:

.. das:function:: lerp(a: float4; b: float4; t: float) : float4

returns vector or scalar representing a + (b - a) * t

:Arguments: * **a** : float4

            * **b** : float4

            * **t** : float

+++++++++++++++++
Matrix operations
+++++++++++++++++

  *  :ref:`float4x4 implicit* (x: float4x4 implicit; y: float4x4 implicit) : float4x4 <function-math__st__float4x4_implicit_float4x4_implicit>` 
  *  :ref:`float4x4 implicit== (x: float4x4 implicit; y: float4x4 implicit) : bool <function-math__eq__eq__float4x4_implicit_float4x4_implicit>` 
  *  :ref:`float4x4 implicit\!= (x: float4x4 implicit; y: float4x4 implicit) : bool <function-math__ex__eq__float4x4_implicit_float4x4_implicit>` 
  *  :ref:`float3x4 implicit* (x: float3x4 implicit; y: float3x4 implicit) : float3x4 <function-math__st__float3x4_implicit_float3x4_implicit>` 
  *  :ref:`float3x4 implicit* (x: float3x4 implicit; y: float3) : float3 <function-math__st__float3x4_implicit_float3>` 
  *  :ref:`float4x4 implicit* (x: float4x4 implicit; y: float4) : float4 <function-math__st__float4x4_implicit_float4>` 
  *  :ref:`float3x4 implicit== (x: float3x4 implicit; y: float3x4 implicit) : bool <function-math__eq__eq__float3x4_implicit_float3x4_implicit>` 
  *  :ref:`float3x4 implicit\!= (x: float3x4 implicit; y: float3x4 implicit) : bool <function-math__ex__eq__float3x4_implicit_float3x4_implicit>` 
  *  :ref:`float3x3 implicit* (x: float3x3 implicit; y: float3x3 implicit) : float3x3 <function-math__st__float3x3_implicit_float3x3_implicit>` 
  *  :ref:`float3x3 implicit* (x: float3x3 implicit; y: float3) : float3 <function-math__st__float3x3_implicit_float3>` 
  *  :ref:`float3x3 implicit== (x: float3x3 implicit; y: float3x3 implicit) : bool <function-math__eq__eq__float3x3_implicit_float3x3_implicit>` 
  *  :ref:`float3x3 implicit\!= (x: float3x3 implicit; y: float3x3 implicit) : bool <function-math__ex__eq__float3x3_implicit_float3x3_implicit>` 

.. _function-math__st__float4x4_implicit_float4x4_implicit:

.. das:function:: float4x4 implicit*(x: float4x4 implicit; y: float4x4 implicit) : float4x4

Multiplies a float4x4 matrix by a float4x4 matrix.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math__eq__eq__float4x4_implicit_float4x4_implicit:

.. das:function:: float4x4 implicit==(x: float4x4 implicit; y: float4x4 implicit) : bool

Compares two float4x4 matrices for equality.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math__ex__eq__float4x4_implicit_float4x4_implicit:

.. das:function:: float4x4 implicit!=(x: float4x4 implicit; y: float4x4 implicit) : bool

Compares two float4x4 matrices for inequality.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math__st__float3x4_implicit_float3x4_implicit:

.. das:function:: float3x4 implicit*(x: float3x4 implicit; y: float3x4 implicit) : float3x4

Multiplies a float3x4 matrix by a float3x4 matrix.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math__st__float3x4_implicit_float3:

.. das:function:: float3x4 implicit*(x: float3x4 implicit; y: float3) : float3

Multiplies a float3x3 matrix by a float3 vector.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** : float3

.. _function-math__st__float4x4_implicit_float4:

.. das:function:: float4x4 implicit*(x: float4x4 implicit; y: float4) : float4

Multiplies a float4x4 matrix by a float4 vector.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** : float4

.. _function-math__eq__eq__float3x4_implicit_float3x4_implicit:

.. das:function:: float3x4 implicit==(x: float3x4 implicit; y: float3x4 implicit) : bool

Compares two float3x4 matrices for equality.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math__ex__eq__float3x4_implicit_float3x4_implicit:

.. das:function:: float3x4 implicit!=(x: float3x4 implicit; y: float3x4 implicit) : bool

Compares two float3x4 matrices for inequality.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math__st__float3x3_implicit_float3x3_implicit:

.. das:function:: float3x3 implicit*(x: float3x3 implicit; y: float3x3 implicit) : float3x3

Multiplies a float3x3 matrix by a float3x3 matrix.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math__st__float3x3_implicit_float3:

.. das:function:: float3x3 implicit*(x: float3x3 implicit; y: float3) : float3

Multiplies a float3x3 matrix by a float3 vector.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** : float3

.. _function-math__eq__eq__float3x3_implicit_float3x3_implicit:

.. das:function:: float3x3 implicit==(x: float3x3 implicit; y: float3x3 implicit) : bool

Compares two float3x3 matrices for equality.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math__ex__eq__float3x3_implicit_float3x3_implicit:

.. das:function:: float3x3 implicit!=(x: float3x3 implicit; y: float3x3 implicit) : bool

Compares two float3x3 matrices for inequality.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

+++++++++++++++++++
Matrix initializers
+++++++++++++++++++

  *  :ref:`float3x3 () : float3x3 <function-math_float3x3>` 
  *  :ref:`float3x4 () : float3x4 <function-math_float3x4>` 
  *  :ref:`float4x4 () : float4x4 <function-math_float4x4>` 
  *  :ref:`float4x4 (arg0: float3x4 implicit) : float4x4 <function-math_float4x4_float3x4_implicit>` 
  *  :ref:`identity4x4 () : float4x4 <function-math_identity4x4>` 
  *  :ref:`float3x4 (arg0: float4x4 implicit) : float3x4 <function-math_float3x4_float4x4_implicit>` 
  *  :ref:`identity3x4 () : float3x4 <function-math_identity3x4>` 
  *  :ref:`float3x3 (arg0: float4x4 implicit) : float3x3 <function-math_float3x3_float4x4_implicit>` 
  *  :ref:`float3x3 (arg0: float3x4 implicit) : float3x3 <function-math_float3x3_float3x4_implicit>` 
  *  :ref:`identity3x3 () : float3x3 <function-math_identity3x3>` 

.. _function-math_float3x3:

.. das:function:: float3x3() : float3x3

Returns empty matrix, where each component is 0.

.. _function-math_float3x4:

.. das:function:: float3x4() : float3x4

Returns empty matrix, where each component is 0.

.. _function-math_float4x4:

.. das:function:: float4x4() : float4x4

Returns empty matrix, where each component is 0.

.. _function-math_float4x4_float3x4_implicit:

.. das:function:: float4x4(arg0: float3x4 implicit) : float4x4

Returns empty matrix, where each component is 0.

:Arguments: * **arg0** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_identity4x4:

.. das:function:: identity4x4() : float4x4

Returns identity matrix, where diagonal is 1 and every other component is 0.

.. _function-math_float3x4_float4x4_implicit:

.. das:function:: float3x4(arg0: float4x4 implicit) : float3x4

Returns empty matrix, where each component is 0.

:Arguments: * **arg0** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_identity3x4:

.. das:function:: identity3x4() : float3x4

Returns identity matrix, where diagonal is 1 and every other component is 0.

.. _function-math_float3x3_float4x4_implicit:

.. das:function:: float3x3(arg0: float4x4 implicit) : float3x3

Returns empty matrix, where each component is 0.

:Arguments: * **arg0** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_float3x3_float3x4_implicit:

.. das:function:: float3x3(arg0: float3x4 implicit) : float3x3

Returns empty matrix, where each component is 0.

:Arguments: * **arg0** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_identity3x3:

.. das:function:: identity3x3() : float3x3

Returns identity matrix, where diagonal is 1 and every other component is 0.

+++++++++++++++++++
Matrix manipulation
+++++++++++++++++++

  *  :ref:`identity (x: float4x4 implicit) <function-math_identity_float4x4_implicit>` 
  *  :ref:`translation (xyz: float3) : float4x4 <function-math_translation_float3>` 
  *  :ref:`transpose (x: float4x4 implicit) : float4x4 <function-math_transpose_float4x4_implicit>` 
  *  :ref:`persp_forward (wk: float; hk: float; zn: float; zf: float) : float4x4 <function-math_persp_forward_float_float_float_float>` 
  *  :ref:`persp_reverse (wk: float; hk: float; zn: float; zf: float) : float4x4 <function-math_persp_reverse_float_float_float_float>` 
  *  :ref:`look_at (eye: float3; at: float3; up: float3) : float4x4 <function-math_look_at_float3_float3_float3>` 
  *  :ref:`compose (pos: float3; rot: float4; scale: float3) : float4x4 <function-math_compose_float3_float4_float3>` 
  *  :ref:`decompose (mat: float4x4 implicit; pos: float3& implicit; rot: float4& implicit; scale: float3& implicit) <function-math_decompose_float4x4_implicit_float3_implicit_float4_implicit_float3_implicit>` 
  *  :ref:`identity (x: float3x4 implicit) <function-math_identity_float3x4_implicit>` 
  *  :ref:`inverse (x: float3x4 implicit) : float3x4 <function-math_inverse_float3x4_implicit>` 
  *  :ref:`inverse (m: float4x4 implicit) : float4x4 <function-math_inverse_float4x4_implicit>` 
  *  :ref:`orthonormal_inverse (m: float3x3 implicit) : float3x3 <function-math_orthonormal_inverse_float3x3_implicit>` 
  *  :ref:`orthonormal_inverse (m: float3x4 implicit) : float3x4 <function-math_orthonormal_inverse_float3x4_implicit>` 
  *  :ref:`rotate (x: float3x4 implicit; y: float3) : float3 <function-math_rotate_float3x4_implicit_float3>` 
  *  :ref:`identity (x: float3x3 implicit) <function-math_identity_float3x3_implicit>` 

.. _function-math_identity_float4x4_implicit:

.. das:function:: identity(x: float4x4 implicit)

Returns identity matrix, where diagonal is 1 and every other component is 0.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_translation_float3:

.. das:function:: translation(xyz: float3) : float4x4

Returns the arc cosine of x, clamped to the range [-1, 1].

:Arguments: * **xyz** : float3

.. _function-math_transpose_float4x4_implicit:

.. das:function:: transpose(x: float4x4 implicit) : float4x4

Transposes the specified input matrix x.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_persp_forward_float_float_float_float:

.. das:function:: persp_forward(wk: float; hk: float; zn: float; zf: float) : float4x4

Perspective matrix, zn - 0, zf - 1

:Arguments: * **wk** : float

            * **hk** : float

            * **zn** : float

            * **zf** : float

.. _function-math_persp_reverse_float_float_float_float:

.. das:function:: persp_reverse(wk: float; hk: float; zn: float; zf: float) : float4x4

Perspective matrix, zn - 1, zf - 0

:Arguments: * **wk** : float

            * **hk** : float

            * **zn** : float

            * **zf** : float

.. _function-math_look_at_float3_float3_float3:

.. das:function:: look_at(eye: float3; at: float3; up: float3) : float4x4

Look-at matrix with the origin at `eye`, looking at `at`, with `up` as up direction.

:Arguments: * **eye** : float3

            * **at** : float3

            * **up** : float3

.. _function-math_compose_float3_float4_float3:

.. das:function:: compose(pos: float3; rot: float4; scale: float3) : float4x4

Compose transformation out of translation, rotation and scale.

:Arguments: * **pos** : float3

            * **rot** : float4

            * **scale** : float3

.. _function-math_decompose_float4x4_implicit_float3_implicit_float4_implicit_float3_implicit:

.. das:function:: decompose(mat: float4x4 implicit; pos: float3& implicit; rot: float4& implicit; scale: float3& implicit)

Decompose transformation into translation, rotation and scale.

:Arguments: * **mat** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **pos** : float3& implicit

            * **rot** : float4& implicit

            * **scale** : float3& implicit

.. _function-math_identity_float3x4_implicit:

.. das:function:: identity(x: float3x4 implicit)

Returns identity matrix, where diagonal is 1 and every other component is 0.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_inverse_float3x4_implicit:

.. das:function:: inverse(x: float3x4 implicit) : float3x4

Returns the inverse of the matrix x.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_inverse_float4x4_implicit:

.. das:function:: inverse(m: float4x4 implicit) : float4x4

Returns the inverse of the matrix x.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_orthonormal_inverse_float3x3_implicit:

.. das:function:: orthonormal_inverse(m: float3x3 implicit) : float3x3

Fast `inverse` for the orthonormal matrix.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math_orthonormal_inverse_float3x4_implicit:

.. das:function:: orthonormal_inverse(m: float3x4 implicit) : float3x4

Fast `inverse` for the orthonormal matrix.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_rotate_float3x4_implicit_float3:

.. das:function:: rotate(x: float3x4 implicit; y: float3) : float3

Rotates vector y by 3x4 matrix x. Only 3x3 portion of x is multiplied by y.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** : float3

.. _function-math_identity_float3x3_implicit:

.. das:function:: identity(x: float3x3 implicit)

Returns identity matrix, where diagonal is 1 and every other component is 0.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

+++++++++++++++++++++
Quaternion operations
+++++++++++++++++++++

  *  :ref:`quat_from_unit_arc (v0: float3; v1: float3) : float4 <function-math_quat_from_unit_arc_float3_float3>` 
  *  :ref:`quat_from_unit_vec_ang (v: float3; ang: float) : float4 <function-math_quat_from_unit_vec_ang_float3_float>` 
  *  :ref:`quat_mul (q1: float4; q2: float4) : float4 <function-math_quat_mul_float4_float4>` 
  *  :ref:`quat_mul_vec (q: float4; v: float3) : float3 <function-math_quat_mul_vec_float4_float3>` 
  *  :ref:`quat_conjugate (q: float4) : float4 <function-math_quat_conjugate_float4>` 

.. _function-math_quat_from_unit_arc_float3_float3:

.. das:function:: quat_from_unit_arc(v0: float3; v1: float3) : float4

Quaternion which represents rotation from `v0` to `v1`, both arguments need to be normalized

:Arguments: * **v0** : float3

            * **v1** : float3

.. _function-math_quat_from_unit_vec_ang_float3_float:

.. das:function:: quat_from_unit_vec_ang(v: float3; ang: float) : float4

Quaternion which represents rotation for `ang` radians around vector `v`. `v` needs to be normalized

:Arguments: * **v** : float3

            * **ang** : float

.. _function-math_quat_mul_float4_float4:

.. das:function:: quat_mul(q1: float4; q2: float4) : float4

Quaternion which is multiplication of `q1` and `q2`

:Arguments: * **q1** : float4

            * **q2** : float4

.. _function-math_quat_mul_vec_float4_float3:

.. das:function:: quat_mul_vec(q: float4; v: float3) : float3

Transform vector `v` by quaternion `q`

:Arguments: * **q** : float4

            * **v** : float3

.. _function-math_quat_conjugate_float4:

.. das:function:: quat_conjugate(q: float4) : float4

Quaternion which is conjugate of `q`

:Arguments: * **q** : float4

+++++++++++++++++++++
Packing and unpacking
+++++++++++++++++++++

  *  :ref:`pack_float_to_byte (x: float4) : uint <function-math_pack_float_to_byte_float4>` 
  *  :ref:`unpack_byte_to_float (x: uint) : float4 <function-math_unpack_byte_to_float_uint>` 

.. _function-math_pack_float_to_byte_float4:

.. das:function:: pack_float_to_byte(x: float4) : uint

Packs float4 vector `v` to byte4 vector and returns it as uint. Each component is clamped to [0..255] range.

:Arguments: * **x** : float4

.. _function-math_unpack_byte_to_float_uint:

.. das:function:: unpack_byte_to_float(x: uint) : float4

Unpacks byte4 vector to float4 vector.

:Arguments: * **x** : uint

+++++++++++++
Uncategorized
+++++++++++++

.. _function-math__dot__float4x4_implicit__eq__eq_const_int:

.. das:function:: float4x4 implicit ==const.[](m: float4x4 implicit ==const; i: int) : float4&

Returns the element of the 4x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : int

.. _function-math__dot__float4x4_const_implicit__eq__eq_const_int:

.. das:function:: float4x4 const implicit ==const.[](m: float4x4 const implicit ==const; i: int) : float4

Returns the element of the 4x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : int

.. _function-math__dot__float4x4_implicit__eq__eq_const_uint:

.. das:function:: float4x4 implicit ==const.[](m: float4x4 implicit ==const; i: uint) : float4&

Returns the element of the 4x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : uint

.. _function-math__dot__float4x4_const_implicit__eq__eq_const_uint:

.. das:function:: float4x4 const implicit ==const.[](m: float4x4 const implicit ==const; i: uint) : float4

Returns the element of the 4x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : uint

.. _function-math_determinant_float4x4_implicit:

.. das:function:: determinant(x: float4x4 implicit) : float

Returns the determinant of the matrix `m`.

:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_determinant_float3x4_implicit:

.. das:function:: determinant(x: float3x4 implicit) : float

Returns the determinant of the matrix `m`.

:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math__dot__float3x4_implicit__eq__eq_const_int:

.. das:function:: float3x4 implicit ==const.[](m: float3x4 implicit ==const; i: int) : float3&

Returns the element of the 3x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : int

.. _function-math__dot__float3x4_const_implicit__eq__eq_const_int:

.. das:function:: float3x4 const implicit ==const.[](m: float3x4 const implicit ==const; i: int) : float3

Returns the element of the 3x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : int

.. _function-math__dot__float3x4_implicit__eq__eq_const_uint:

.. das:function:: float3x4 implicit ==const.[](m: float3x4 implicit ==const; i: uint) : float3&

Returns the element of the 3x4 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : uint

.. _function-math__dot__float3x4_const_implicit__eq__eq_const_uint:

.. das:function:: float3x4 const implicit ==const.[](m: float3x4 const implicit ==const; i: uint) : float3

Returns element of 3x4 matrix at specified index.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : uint

.. _function-math_quat_from_euler_float3:

.. das:function:: quat_from_euler(angles: float3) : float4

Construct quaternion from euler angles.

:Arguments: * **angles** : float3

.. _function-math_quat_from_euler_float_float_float:

.. das:function:: quat_from_euler(x: float; y: float; z: float) : float4

Construct quaternion from euler angles.

:Arguments: * **x** : float

            * **y** : float

            * **z** : float

.. _function-math_euler_from_quat_float4:

.. das:function:: euler_from_quat(angles: float4) : float3

Construct euler angles from quaternion.

:Arguments: * **angles** : float4

.. _function-math_quat_float3x3_implicit:

.. das:function:: quat(m: float3x3 implicit) : float4

Construct quaternion from matrix.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math_quat_float3x4_implicit:

.. das:function:: quat(m: float3x4 implicit) : float4

Construct quaternion from matrix.

:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-math_quat_float4x4_implicit:

.. das:function:: quat(m: float4x4 implicit) : float4

Construct quaternion from matrix.

:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-math_quat_slerp_float_float4_float4:

.. das:function:: quat_slerp(t: float; a: float4; b: float4) : float4

Spherical linear interpolation between `a` and `b` by `t`.

:Arguments: * **t** : float

            * **a** : float4

            * **b** : float4

.. _function-math_determinant_float3x3_implicit:

.. das:function:: determinant(x: float3x3 implicit) : float

Returns the determinant of the matrix `m`.

:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-math__dot__float3x3_implicit__eq__eq_const_int:

.. das:function:: float3x3 implicit ==const.[](m: float3x3 implicit ==const; i: int) : float3&

Returns the element of the 3x3 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : int

.. _function-math__dot__float3x3_const_implicit__eq__eq_const_int:

.. das:function:: float3x3 const implicit ==const.[](m: float3x3 const implicit ==const; i: int) : float3

Returns the element of the 3x3 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : int

.. _function-math__dot__float3x3_implicit__eq__eq_const_uint:

.. das:function:: float3x3 implicit ==const.[](m: float3x3 implicit ==const; i: uint) : float3&

Returns the element of the 3x3 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : uint

.. _function-math__dot__float3x3_const_implicit__eq__eq_const_uint:

.. das:function:: float3x3 const implicit ==const.[](m: float3x3 const implicit ==const; i: uint) : float3

Returns the element of the 3x3 matrix at the specified index.

:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : uint


