
.. _stdlib_math:

============
Math library
============

Floating point math in general is not bit-precise. Compiler can optimize permutations, replace divisions with multiplications, and some of functions are not bit-exact.
If you need precise math use double precision type.
All functions and symbols are in "math" module, use require to get access to it. ::

    require math


+++++++++
Constants
+++++++++

.. _global-math-PI:

.. das:attribute:: PI = 3.1415927f

// stub


.. _global-math-DBL_PI:

.. das:attribute:: DBL_PI = 3.141592653589793lf

// stub


.. _global-math-FLT_EPSILON:

.. das:attribute:: FLT_EPSILON = 1.1920929e-07f

// stub


.. _global-math-DBL_EPSILON:

.. das:attribute:: DBL_EPSILON = 2.220446049250313e-16lf

// stub


++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-math-float4x4:

.. das:attribute:: float4x4

:Fields: * **x** : float4

         * **y** : float4

         * **z** : float4

         * **w** : float4

// stub


.. _handle-math-float3x4:

.. das:attribute:: float3x4

:Fields: * **x** : float3

         * **y** : float3

         * **z** : float3

         * **w** : float3

// stub


.. _handle-math-float3x3:

.. das:attribute:: float3x3

:Fields: * **x** : float3

         * **y** : float3

         * **z** : float3

// stub


++++++++++++++++++++++++++++++++++++++++++
all numerics (uint*, int*, float*, double)
++++++++++++++++++++++++++++++++++++++++++

  *  :ref:`min (x: int; y: int) : int <function-_at_math_c__c_min_Ci_Ci>` 
  *  :ref:`max (x: int; y: int) : int <function-_at_math_c__c_max_Ci_Ci>` 
  *  :ref:`min (x: int2; y: int2) : int2 <function-_at_math_c__c_min_Ci2_Ci2>` 
  *  :ref:`max (x: int2; y: int2) : int2 <function-_at_math_c__c_max_Ci2_Ci2>` 
  *  :ref:`min (x: int3; y: int3) : int3 <function-_at_math_c__c_min_Ci3_Ci3>` 
  *  :ref:`max (x: int3; y: int3) : int3 <function-_at_math_c__c_max_Ci3_Ci3>` 
  *  :ref:`min (x: int4; y: int4) : int4 <function-_at_math_c__c_min_Ci4_Ci4>` 
  *  :ref:`max (x: int4; y: int4) : int4 <function-_at_math_c__c_max_Ci4_Ci4>` 
  *  :ref:`min (x: uint; y: uint) : uint <function-_at_math_c__c_min_Cu_Cu>` 
  *  :ref:`max (x: uint; y: uint) : uint <function-_at_math_c__c_max_Cu_Cu>` 
  *  :ref:`min (x: uint2; y: uint2) : uint2 <function-_at_math_c__c_min_Cu2_Cu2>` 
  *  :ref:`max (x: uint2; y: uint2) : uint2 <function-_at_math_c__c_max_Cu2_Cu2>` 
  *  :ref:`min (x: uint3; y: uint3) : uint3 <function-_at_math_c__c_min_Cu3_Cu3>` 
  *  :ref:`max (x: uint3; y: uint3) : uint3 <function-_at_math_c__c_max_Cu3_Cu3>` 
  *  :ref:`min (x: uint4; y: uint4) : uint4 <function-_at_math_c__c_min_Cu4_Cu4>` 
  *  :ref:`max (x: uint4; y: uint4) : uint4 <function-_at_math_c__c_max_Cu4_Cu4>` 
  *  :ref:`min (x: float; y: float) : float <function-_at_math_c__c_min_Cf_Cf>` 
  *  :ref:`max (x: float; y: float) : float <function-_at_math_c__c_max_Cf_Cf>` 
  *  :ref:`min (x: float2; y: float2) : float2 <function-_at_math_c__c_min_Cf2_Cf2>` 
  *  :ref:`max (x: float2; y: float2) : float2 <function-_at_math_c__c_max_Cf2_Cf2>` 
  *  :ref:`min (x: float3; y: float3) : float3 <function-_at_math_c__c_min_Cf3_Cf3>` 
  *  :ref:`max (x: float3; y: float3) : float3 <function-_at_math_c__c_max_Cf3_Cf3>` 
  *  :ref:`min (x: float4; y: float4) : float4 <function-_at_math_c__c_min_Cf4_Cf4>` 
  *  :ref:`max (x: float4; y: float4) : float4 <function-_at_math_c__c_max_Cf4_Cf4>` 
  *  :ref:`min (x: double; y: double) : double <function-_at_math_c__c_min_Cd_Cd>` 
  *  :ref:`max (x: double; y: double) : double <function-_at_math_c__c_max_Cd_Cd>` 
  *  :ref:`min (x: int64; y: int64) : int64 <function-_at_math_c__c_min_Ci64_Ci64>` 
  *  :ref:`max (x: int64; y: int64) : int64 <function-_at_math_c__c_max_Ci64_Ci64>` 
  *  :ref:`min (x: uint64; y: uint64) : uint64 <function-_at_math_c__c_min_Cu64_Cu64>` 
  *  :ref:`max (x: uint64; y: uint64) : uint64 <function-_at_math_c__c_max_Cu64_Cu64>` 

.. _function-_at_math_c__c_min_Ci_Ci:

.. das:function:: min(x: int; y: int) : int

// stub



:Arguments: * **x** : int

            * **y** : int

.. _function-_at_math_c__c_max_Ci_Ci:

.. das:function:: max(x: int; y: int) : int

// stub



:Arguments: * **x** : int

            * **y** : int

.. _function-_at_math_c__c_min_Ci2_Ci2:

.. das:function:: min(x: int2; y: int2) : int2

// stub



:Arguments: * **x** : int2

            * **y** : int2

.. _function-_at_math_c__c_max_Ci2_Ci2:

.. das:function:: max(x: int2; y: int2) : int2

// stub



:Arguments: * **x** : int2

            * **y** : int2

.. _function-_at_math_c__c_min_Ci3_Ci3:

.. das:function:: min(x: int3; y: int3) : int3

// stub



:Arguments: * **x** : int3

            * **y** : int3

.. _function-_at_math_c__c_max_Ci3_Ci3:

.. das:function:: max(x: int3; y: int3) : int3

// stub



:Arguments: * **x** : int3

            * **y** : int3

.. _function-_at_math_c__c_min_Ci4_Ci4:

.. das:function:: min(x: int4; y: int4) : int4

// stub



:Arguments: * **x** : int4

            * **y** : int4

.. _function-_at_math_c__c_max_Ci4_Ci4:

.. das:function:: max(x: int4; y: int4) : int4

// stub



:Arguments: * **x** : int4

            * **y** : int4

.. _function-_at_math_c__c_min_Cu_Cu:

.. das:function:: min(x: uint; y: uint) : uint

// stub



:Arguments: * **x** : uint

            * **y** : uint

.. _function-_at_math_c__c_max_Cu_Cu:

.. das:function:: max(x: uint; y: uint) : uint

// stub



:Arguments: * **x** : uint

            * **y** : uint

.. _function-_at_math_c__c_min_Cu2_Cu2:

.. das:function:: min(x: uint2; y: uint2) : uint2

// stub



:Arguments: * **x** : uint2

            * **y** : uint2

.. _function-_at_math_c__c_max_Cu2_Cu2:

.. das:function:: max(x: uint2; y: uint2) : uint2

// stub



:Arguments: * **x** : uint2

            * **y** : uint2

.. _function-_at_math_c__c_min_Cu3_Cu3:

.. das:function:: min(x: uint3; y: uint3) : uint3

// stub



:Arguments: * **x** : uint3

            * **y** : uint3

.. _function-_at_math_c__c_max_Cu3_Cu3:

.. das:function:: max(x: uint3; y: uint3) : uint3

// stub



:Arguments: * **x** : uint3

            * **y** : uint3

.. _function-_at_math_c__c_min_Cu4_Cu4:

.. das:function:: min(x: uint4; y: uint4) : uint4

// stub



:Arguments: * **x** : uint4

            * **y** : uint4

.. _function-_at_math_c__c_max_Cu4_Cu4:

.. das:function:: max(x: uint4; y: uint4) : uint4

// stub



:Arguments: * **x** : uint4

            * **y** : uint4

.. _function-_at_math_c__c_min_Cf_Cf:

.. das:function:: min(x: float; y: float) : float

// stub



:Arguments: * **x** : float

            * **y** : float

.. _function-_at_math_c__c_max_Cf_Cf:

.. das:function:: max(x: float; y: float) : float

// stub



:Arguments: * **x** : float

            * **y** : float

.. _function-_at_math_c__c_min_Cf2_Cf2:

.. das:function:: min(x: float2; y: float2) : float2

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_max_Cf2_Cf2:

.. das:function:: max(x: float2; y: float2) : float2

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_min_Cf3_Cf3:

.. das:function:: min(x: float3; y: float3) : float3

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_max_Cf3_Cf3:

.. das:function:: max(x: float3; y: float3) : float3

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_min_Cf4_Cf4:

.. das:function:: min(x: float4; y: float4) : float4

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_max_Cf4_Cf4:

.. das:function:: max(x: float4; y: float4) : float4

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_min_Cd_Cd:

.. das:function:: min(x: double; y: double) : double

// stub



:Arguments: * **x** : double

            * **y** : double

.. _function-_at_math_c__c_max_Cd_Cd:

.. das:function:: max(x: double; y: double) : double

// stub



:Arguments: * **x** : double

            * **y** : double

.. _function-_at_math_c__c_min_Ci64_Ci64:

.. das:function:: min(x: int64; y: int64) : int64

// stub



:Arguments: * **x** : int64

            * **y** : int64

.. _function-_at_math_c__c_max_Ci64_Ci64:

.. das:function:: max(x: int64; y: int64) : int64

// stub



:Arguments: * **x** : int64

            * **y** : int64

.. _function-_at_math_c__c_min_Cu64_Cu64:

.. das:function:: min(x: uint64; y: uint64) : uint64

// stub



:Arguments: * **x** : uint64

            * **y** : uint64

.. _function-_at_math_c__c_max_Cu64_Cu64:

.. das:function:: max(x: uint64; y: uint64) : uint64

// stub



:Arguments: * **x** : uint64

            * **y** : uint64

+++++++++++++++++
float* and double
+++++++++++++++++

  *  :ref:`sin (x: float) : float <function-_at_math_c__c_sin_Cf>` 
  *  :ref:`cos (x: float) : float <function-_at_math_c__c_cos_Cf>` 
  *  :ref:`tan (x: float) : float <function-_at_math_c__c_tan_Cf>` 
  *  :ref:`sin (x: float2) : float2 <function-_at_math_c__c_sin_Cf2>` 
  *  :ref:`cos (x: float2) : float2 <function-_at_math_c__c_cos_Cf2>` 
  *  :ref:`tan (x: float2) : float2 <function-_at_math_c__c_tan_Cf2>` 
  *  :ref:`sin (x: float3) : float3 <function-_at_math_c__c_sin_Cf3>` 
  *  :ref:`cos (x: float3) : float3 <function-_at_math_c__c_cos_Cf3>` 
  *  :ref:`tan (x: float3) : float3 <function-_at_math_c__c_tan_Cf3>` 
  *  :ref:`sin (x: float4) : float4 <function-_at_math_c__c_sin_Cf4>` 
  *  :ref:`cos (x: float4) : float4 <function-_at_math_c__c_cos_Cf4>` 
  *  :ref:`tan (x: float4) : float4 <function-_at_math_c__c_tan_Cf4>` 
  *  :ref:`exp (x: float) : float <function-_at_math_c__c_exp_Cf>` 
  *  :ref:`log (x: float) : float <function-_at_math_c__c_log_Cf>` 
  *  :ref:`exp2 (x: float) : float <function-_at_math_c__c_exp2_Cf>` 
  *  :ref:`log2 (x: float) : float <function-_at_math_c__c_log2_Cf>` 
  *  :ref:`rcp (x: float) : float <function-_at_math_c__c_rcp_Cf>` 
  *  :ref:`pow (x: float; y: float) : float <function-_at_math_c__c_pow_Cf_Cf>` 
  *  :ref:`exp (x: float2) : float2 <function-_at_math_c__c_exp_Cf2>` 
  *  :ref:`log (x: float2) : float2 <function-_at_math_c__c_log_Cf2>` 
  *  :ref:`exp2 (x: float2) : float2 <function-_at_math_c__c_exp2_Cf2>` 
  *  :ref:`log2 (x: float2) : float2 <function-_at_math_c__c_log2_Cf2>` 
  *  :ref:`rcp (x: float2) : float2 <function-_at_math_c__c_rcp_Cf2>` 
  *  :ref:`pow (x: float2; y: float2) : float2 <function-_at_math_c__c_pow_Cf2_Cf2>` 
  *  :ref:`exp (x: float3) : float3 <function-_at_math_c__c_exp_Cf3>` 
  *  :ref:`log (x: float3) : float3 <function-_at_math_c__c_log_Cf3>` 
  *  :ref:`exp2 (x: float3) : float3 <function-_at_math_c__c_exp2_Cf3>` 
  *  :ref:`log2 (x: float3) : float3 <function-_at_math_c__c_log2_Cf3>` 
  *  :ref:`rcp (x: float3) : float3 <function-_at_math_c__c_rcp_Cf3>` 
  *  :ref:`pow (x: float3; y: float3) : float3 <function-_at_math_c__c_pow_Cf3_Cf3>` 
  *  :ref:`exp (x: float4) : float4 <function-_at_math_c__c_exp_Cf4>` 
  *  :ref:`log (x: float4) : float4 <function-_at_math_c__c_log_Cf4>` 
  *  :ref:`exp2 (x: float4) : float4 <function-_at_math_c__c_exp2_Cf4>` 
  *  :ref:`log2 (x: float4) : float4 <function-_at_math_c__c_log2_Cf4>` 
  *  :ref:`rcp (x: float4) : float4 <function-_at_math_c__c_rcp_Cf4>` 
  *  :ref:`pow (x: float4; y: float4) : float4 <function-_at_math_c__c_pow_Cf4_Cf4>` 
  *  :ref:`floor (x: float) : float <function-_at_math_c__c_floor_Cf>` 
  *  :ref:`ceil (x: float) : float <function-_at_math_c__c_ceil_Cf>` 
  *  :ref:`sqrt (x: float) : float <function-_at_math_c__c_sqrt_Cf>` 
  *  :ref:`saturate (x: float) : float <function-_at_math_c__c_saturate_Cf>` 
  *  :ref:`floor (x: float2) : float2 <function-_at_math_c__c_floor_Cf2>` 
  *  :ref:`ceil (x: float2) : float2 <function-_at_math_c__c_ceil_Cf2>` 
  *  :ref:`sqrt (x: float2) : float2 <function-_at_math_c__c_sqrt_Cf2>` 
  *  :ref:`saturate (x: float2) : float2 <function-_at_math_c__c_saturate_Cf2>` 
  *  :ref:`floor (x: float3) : float3 <function-_at_math_c__c_floor_Cf3>` 
  *  :ref:`ceil (x: float3) : float3 <function-_at_math_c__c_ceil_Cf3>` 
  *  :ref:`sqrt (x: float3) : float3 <function-_at_math_c__c_sqrt_Cf3>` 
  *  :ref:`saturate (x: float3) : float3 <function-_at_math_c__c_saturate_Cf3>` 
  *  :ref:`floor (x: float4) : float4 <function-_at_math_c__c_floor_Cf4>` 
  *  :ref:`ceil (x: float4) : float4 <function-_at_math_c__c_ceil_Cf4>` 
  *  :ref:`sqrt (x: float4) : float4 <function-_at_math_c__c_sqrt_Cf4>` 
  *  :ref:`saturate (x: float4) : float4 <function-_at_math_c__c_saturate_Cf4>` 
  *  :ref:`abs (x: int) : int <function-_at_math_c__c_abs_Ci>` 
  *  :ref:`sign (x: int) : int <function-_at_math_c__c_sign_Ci>` 
  *  :ref:`abs (x: int2) : int2 <function-_at_math_c__c_abs_Ci2>` 
  *  :ref:`sign (x: int2) : int2 <function-_at_math_c__c_sign_Ci2>` 
  *  :ref:`abs (x: int3) : int3 <function-_at_math_c__c_abs_Ci3>` 
  *  :ref:`sign (x: int3) : int3 <function-_at_math_c__c_sign_Ci3>` 
  *  :ref:`abs (x: int4) : int4 <function-_at_math_c__c_abs_Ci4>` 
  *  :ref:`sign (x: int4) : int4 <function-_at_math_c__c_sign_Ci4>` 
  *  :ref:`abs (x: uint) : uint <function-_at_math_c__c_abs_Cu>` 
  *  :ref:`sign (x: uint) : uint <function-_at_math_c__c_sign_Cu>` 
  *  :ref:`abs (x: uint2) : uint2 <function-_at_math_c__c_abs_Cu2>` 
  *  :ref:`sign (x: uint2) : uint2 <function-_at_math_c__c_sign_Cu2>` 
  *  :ref:`abs (x: uint3) : uint3 <function-_at_math_c__c_abs_Cu3>` 
  *  :ref:`sign (x: uint3) : uint3 <function-_at_math_c__c_sign_Cu3>` 
  *  :ref:`abs (x: uint4) : uint4 <function-_at_math_c__c_abs_Cu4>` 
  *  :ref:`sign (x: uint4) : uint4 <function-_at_math_c__c_sign_Cu4>` 
  *  :ref:`abs (x: float) : float <function-_at_math_c__c_abs_Cf>` 
  *  :ref:`sign (x: float) : float <function-_at_math_c__c_sign_Cf>` 
  *  :ref:`abs (x: float2) : float2 <function-_at_math_c__c_abs_Cf2>` 
  *  :ref:`sign (x: float2) : float2 <function-_at_math_c__c_sign_Cf2>` 
  *  :ref:`abs (x: float3) : float3 <function-_at_math_c__c_abs_Cf3>` 
  *  :ref:`sign (x: float3) : float3 <function-_at_math_c__c_sign_Cf3>` 
  *  :ref:`abs (x: float4) : float4 <function-_at_math_c__c_abs_Cf4>` 
  *  :ref:`sign (x: float4) : float4 <function-_at_math_c__c_sign_Cf4>` 
  *  :ref:`abs (x: double) : double <function-_at_math_c__c_abs_Cd>` 
  *  :ref:`sign (x: double) : double <function-_at_math_c__c_sign_Cd>` 
  *  :ref:`abs (x: int64) : int64 <function-_at_math_c__c_abs_Ci64>` 
  *  :ref:`sign (x: int64) : int64 <function-_at_math_c__c_sign_Ci64>` 
  *  :ref:`abs (x: uint64) : uint64 <function-_at_math_c__c_abs_Cu64>` 
  *  :ref:`sign (x: uint64) : uint64 <function-_at_math_c__c_sign_Cu64>` 
  *  :ref:`is_nan (x: float) : bool <function-_at_math_c__c_is_nan_Cf>` 
  *  :ref:`is_finite (x: float) : bool <function-_at_math_c__c_is_finite_Cf>` 
  *  :ref:`is_nan (x: double) : bool <function-_at_math_c__c_is_nan_Cd>` 
  *  :ref:`is_finite (x: double) : bool <function-_at_math_c__c_is_finite_Cd>` 
  *  :ref:`sqrt (x: double) : double <function-_at_math_c__c_sqrt_Cd>` 
  *  :ref:`exp (x: double) : double <function-_at_math_c__c_exp_Cd>` 
  *  :ref:`rcp (x: double) : double <function-_at_math_c__c_rcp_Cd>` 
  *  :ref:`log (x: double) : double <function-_at_math_c__c_log_Cd>` 
  *  :ref:`pow (x: double; y: double) : double <function-_at_math_c__c_pow_Cd_Cd>` 
  *  :ref:`exp2 (x: double) : double <function-_at_math_c__c_exp2_Cd>` 
  *  :ref:`log2 (x: double) : double <function-_at_math_c__c_log2_Cd>` 
  *  :ref:`sin (x: double) : double <function-_at_math_c__c_sin_Cd>` 
  *  :ref:`cos (x: double) : double <function-_at_math_c__c_cos_Cd>` 
  *  :ref:`asin (x: double) : double <function-_at_math_c__c_asin_Cd>` 
  *  :ref:`acos (x: double) : double <function-_at_math_c__c_acos_Cd>` 
  *  :ref:`tan (x: double) : double <function-_at_math_c__c_tan_Cd>` 
  *  :ref:`atan (x: double) : double <function-_at_math_c__c_atan_Cd>` 
  *  :ref:`atan2 (y: double; x: double) : double <function-_at_math_c__c_atan2_Cd_Cd>` 
  *  :ref:`sincos (x: float; s: float& implicit; c: float& implicit) <function-_at_math_c__c_sincos_Cf_&If_&If>` 
  *  :ref:`sincos (x: double; s: double& implicit; c: double& implicit) <function-_at_math_c__c_sincos_Cd_&Id_&Id>` 
  *  :ref:`asin (x: float) : float <function-_at_math_c__c_asin_Cf>` 
  *  :ref:`acos (x: float) : float <function-_at_math_c__c_acos_Cf>` 
  *  :ref:`atan (x: float) : float <function-_at_math_c__c_atan_Cf>` 
  *  :ref:`atan2 (y: float; x: float) : float <function-_at_math_c__c_atan2_Cf_Cf>` 
  *  :ref:`asin (x: float2) : float2 <function-_at_math_c__c_asin_Cf2>` 
  *  :ref:`asin (x: float3) : float3 <function-_at_math_c__c_asin_Cf3>` 
  *  :ref:`asin (x: float4) : float4 <function-_at_math_c__c_asin_Cf4>` 
  *  :ref:`acos (x: float2) : float2 <function-_at_math_c__c_acos_Cf2>` 
  *  :ref:`acos (x: float3) : float3 <function-_at_math_c__c_acos_Cf3>` 
  *  :ref:`acos (x: float4) : float4 <function-_at_math_c__c_acos_Cf4>` 
  *  :ref:`atan (x: float2) : float2 <function-_at_math_c__c_atan_Cf2>` 
  *  :ref:`atan (x: float3) : float3 <function-_at_math_c__c_atan_Cf3>` 
  *  :ref:`atan (x: float4) : float4 <function-_at_math_c__c_atan_Cf4>` 
  *  :ref:`atan2 (y: float2; x: float2) : float2 <function-_at_math_c__c_atan2_Cf2_Cf2>` 
  *  :ref:`atan2 (y: float3; x: float3) : float3 <function-_at_math_c__c_atan2_Cf3_Cf3>` 
  *  :ref:`atan2 (y: float4; x: float4) : float4 <function-_at_math_c__c_atan2_Cf4_Cf4>` 

.. _function-_at_math_c__c_sin_Cf:

.. das:function:: sin(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_cos_Cf:

.. das:function:: cos(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_tan_Cf:

.. das:function:: tan(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_sin_Cf2:

.. das:function:: sin(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_cos_Cf2:

.. das:function:: cos(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_tan_Cf2:

.. das:function:: tan(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_sin_Cf3:

.. das:function:: sin(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_cos_Cf3:

.. das:function:: cos(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_tan_Cf3:

.. das:function:: tan(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_sin_Cf4:

.. das:function:: sin(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_cos_Cf4:

.. das:function:: cos(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_tan_Cf4:

.. das:function:: tan(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_exp_Cf:

.. das:function:: exp(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_log_Cf:

.. das:function:: log(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_exp2_Cf:

.. das:function:: exp2(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_log2_Cf:

.. das:function:: log2(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_rcp_Cf:

.. das:function:: rcp(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_pow_Cf_Cf:

.. das:function:: pow(x: float; y: float) : float

// stub



:Arguments: * **x** : float

            * **y** : float

.. _function-_at_math_c__c_exp_Cf2:

.. das:function:: exp(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_log_Cf2:

.. das:function:: log(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_exp2_Cf2:

.. das:function:: exp2(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_log2_Cf2:

.. das:function:: log2(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_rcp_Cf2:

.. das:function:: rcp(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_pow_Cf2_Cf2:

.. das:function:: pow(x: float2; y: float2) : float2

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_exp_Cf3:

.. das:function:: exp(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_log_Cf3:

.. das:function:: log(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_exp2_Cf3:

.. das:function:: exp2(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_log2_Cf3:

.. das:function:: log2(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_rcp_Cf3:

.. das:function:: rcp(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_pow_Cf3_Cf3:

.. das:function:: pow(x: float3; y: float3) : float3

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_exp_Cf4:

.. das:function:: exp(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_log_Cf4:

.. das:function:: log(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_exp2_Cf4:

.. das:function:: exp2(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_log2_Cf4:

.. das:function:: log2(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_rcp_Cf4:

.. das:function:: rcp(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_pow_Cf4_Cf4:

.. das:function:: pow(x: float4; y: float4) : float4

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_floor_Cf:

.. das:function:: floor(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_ceil_Cf:

.. das:function:: ceil(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_sqrt_Cf:

.. das:function:: sqrt(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_saturate_Cf:

.. das:function:: saturate(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_floor_Cf2:

.. das:function:: floor(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_ceil_Cf2:

.. das:function:: ceil(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_sqrt_Cf2:

.. das:function:: sqrt(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_saturate_Cf2:

.. das:function:: saturate(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_floor_Cf3:

.. das:function:: floor(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_ceil_Cf3:

.. das:function:: ceil(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_sqrt_Cf3:

.. das:function:: sqrt(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_saturate_Cf3:

.. das:function:: saturate(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_floor_Cf4:

.. das:function:: floor(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_ceil_Cf4:

.. das:function:: ceil(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_sqrt_Cf4:

.. das:function:: sqrt(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_saturate_Cf4:

.. das:function:: saturate(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_abs_Ci:

.. das:function:: abs(x: int) : int

// stub



:Arguments: * **x** : int

.. _function-_at_math_c__c_sign_Ci:

.. das:function:: sign(x: int) : int

// stub



:Arguments: * **x** : int

.. _function-_at_math_c__c_abs_Ci2:

.. das:function:: abs(x: int2) : int2

// stub



:Arguments: * **x** : int2

.. _function-_at_math_c__c_sign_Ci2:

.. das:function:: sign(x: int2) : int2

// stub



:Arguments: * **x** : int2

.. _function-_at_math_c__c_abs_Ci3:

.. das:function:: abs(x: int3) : int3

// stub



:Arguments: * **x** : int3

.. _function-_at_math_c__c_sign_Ci3:

.. das:function:: sign(x: int3) : int3

// stub



:Arguments: * **x** : int3

.. _function-_at_math_c__c_abs_Ci4:

.. das:function:: abs(x: int4) : int4

// stub



:Arguments: * **x** : int4

.. _function-_at_math_c__c_sign_Ci4:

.. das:function:: sign(x: int4) : int4

// stub



:Arguments: * **x** : int4

.. _function-_at_math_c__c_abs_Cu:

.. das:function:: abs(x: uint) : uint

// stub



:Arguments: * **x** : uint

.. _function-_at_math_c__c_sign_Cu:

.. das:function:: sign(x: uint) : uint

// stub



:Arguments: * **x** : uint

.. _function-_at_math_c__c_abs_Cu2:

.. das:function:: abs(x: uint2) : uint2

// stub



:Arguments: * **x** : uint2

.. _function-_at_math_c__c_sign_Cu2:

.. das:function:: sign(x: uint2) : uint2

// stub



:Arguments: * **x** : uint2

.. _function-_at_math_c__c_abs_Cu3:

.. das:function:: abs(x: uint3) : uint3

// stub



:Arguments: * **x** : uint3

.. _function-_at_math_c__c_sign_Cu3:

.. das:function:: sign(x: uint3) : uint3

// stub



:Arguments: * **x** : uint3

.. _function-_at_math_c__c_abs_Cu4:

.. das:function:: abs(x: uint4) : uint4

// stub



:Arguments: * **x** : uint4

.. _function-_at_math_c__c_sign_Cu4:

.. das:function:: sign(x: uint4) : uint4

// stub



:Arguments: * **x** : uint4

.. _function-_at_math_c__c_abs_Cf:

.. das:function:: abs(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_sign_Cf:

.. das:function:: sign(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_abs_Cf2:

.. das:function:: abs(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_sign_Cf2:

.. das:function:: sign(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_abs_Cf3:

.. das:function:: abs(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_sign_Cf3:

.. das:function:: sign(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_abs_Cf4:

.. das:function:: abs(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_sign_Cf4:

.. das:function:: sign(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_abs_Cd:

.. das:function:: abs(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_sign_Cd:

.. das:function:: sign(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_abs_Ci64:

.. das:function:: abs(x: int64) : int64

// stub



:Arguments: * **x** : int64

.. _function-_at_math_c__c_sign_Ci64:

.. das:function:: sign(x: int64) : int64

// stub



:Arguments: * **x** : int64

.. _function-_at_math_c__c_abs_Cu64:

.. das:function:: abs(x: uint64) : uint64

// stub



:Arguments: * **x** : uint64

.. _function-_at_math_c__c_sign_Cu64:

.. das:function:: sign(x: uint64) : uint64

// stub



:Arguments: * **x** : uint64

.. _function-_at_math_c__c_is_nan_Cf:

.. das:function:: is_nan(x: float) : bool

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_is_finite_Cf:

.. das:function:: is_finite(x: float) : bool

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_is_nan_Cd:

.. das:function:: is_nan(x: double) : bool

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_is_finite_Cd:

.. das:function:: is_finite(x: double) : bool

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_sqrt_Cd:

.. das:function:: sqrt(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_exp_Cd:

.. das:function:: exp(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_rcp_Cd:

.. das:function:: rcp(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_log_Cd:

.. das:function:: log(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_pow_Cd_Cd:

.. das:function:: pow(x: double; y: double) : double

// stub



:Arguments: * **x** : double

            * **y** : double

.. _function-_at_math_c__c_exp2_Cd:

.. das:function:: exp2(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_log2_Cd:

.. das:function:: log2(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_sin_Cd:

.. das:function:: sin(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_cos_Cd:

.. das:function:: cos(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_asin_Cd:

.. das:function:: asin(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_acos_Cd:

.. das:function:: acos(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_tan_Cd:

.. das:function:: tan(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_atan_Cd:

.. das:function:: atan(x: double) : double

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_atan2_Cd_Cd:

.. das:function:: atan2(y: double; x: double) : double

// stub



:Arguments: * **y** : double

            * **x** : double

.. _function-_at_math_c__c_sincos_Cf_&If_&If:

.. das:function:: sincos(x: float; s: float& implicit; c: float& implicit)

// stub



:Arguments: * **x** : float

            * **s** : float& implicit

            * **c** : float& implicit

.. _function-_at_math_c__c_sincos_Cd_&Id_&Id:

.. das:function:: sincos(x: double; s: double& implicit; c: double& implicit)

// stub



:Arguments: * **x** : double

            * **s** : double& implicit

            * **c** : double& implicit

.. _function-_at_math_c__c_asin_Cf:

.. das:function:: asin(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_acos_Cf:

.. das:function:: acos(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_atan_Cf:

.. das:function:: atan(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_atan2_Cf_Cf:

.. das:function:: atan2(y: float; x: float) : float

// stub



:Arguments: * **y** : float

            * **x** : float

.. _function-_at_math_c__c_asin_Cf2:

.. das:function:: asin(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_asin_Cf3:

.. das:function:: asin(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_asin_Cf4:

.. das:function:: asin(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_acos_Cf2:

.. das:function:: acos(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_acos_Cf3:

.. das:function:: acos(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_acos_Cf4:

.. das:function:: acos(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_atan_Cf2:

.. das:function:: atan(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_atan_Cf3:

.. das:function:: atan(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_atan_Cf4:

.. das:function:: atan(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_atan2_Cf2_Cf2:

.. das:function:: atan2(y: float2; x: float2) : float2

// stub



:Arguments: * **y** : float2

            * **x** : float2

.. _function-_at_math_c__c_atan2_Cf3_Cf3:

.. das:function:: atan2(y: float3; x: float3) : float3

// stub



:Arguments: * **y** : float3

            * **x** : float3

.. _function-_at_math_c__c_atan2_Cf4_Cf4:

.. das:function:: atan2(y: float4; x: float4) : float4

// stub



:Arguments: * **y** : float4

            * **x** : float4

+++++++++++
float* only
+++++++++++

  *  :ref:`rcp_est (x: float) : float <function-_at_math_c__c_rcp_est_Cf>` 
  *  :ref:`rcp_est (x: float2) : float2 <function-_at_math_c__c_rcp_est_Cf2>` 
  *  :ref:`rcp_est (x: float3) : float3 <function-_at_math_c__c_rcp_est_Cf3>` 
  *  :ref:`rcp_est (x: float4) : float4 <function-_at_math_c__c_rcp_est_Cf4>` 
  *  :ref:`fract (x: float) : float <function-_at_math_c__c_fract_Cf>` 
  *  :ref:`rsqrt (x: float) : float <function-_at_math_c__c_rsqrt_Cf>` 
  *  :ref:`rsqrt_est (x: float) : float <function-_at_math_c__c_rsqrt_est_Cf>` 
  *  :ref:`fract (x: float2) : float2 <function-_at_math_c__c_fract_Cf2>` 
  *  :ref:`rsqrt (x: float2) : float2 <function-_at_math_c__c_rsqrt_Cf2>` 
  *  :ref:`rsqrt_est (x: float2) : float2 <function-_at_math_c__c_rsqrt_est_Cf2>` 
  *  :ref:`fract (x: float3) : float3 <function-_at_math_c__c_fract_Cf3>` 
  *  :ref:`rsqrt (x: float3) : float3 <function-_at_math_c__c_rsqrt_Cf3>` 
  *  :ref:`rsqrt_est (x: float3) : float3 <function-_at_math_c__c_rsqrt_est_Cf3>` 
  *  :ref:`fract (x: float4) : float4 <function-_at_math_c__c_fract_Cf4>` 
  *  :ref:`rsqrt (x: float4) : float4 <function-_at_math_c__c_rsqrt_Cf4>` 
  *  :ref:`rsqrt_est (x: float4) : float4 <function-_at_math_c__c_rsqrt_est_Cf4>` 
  *  :ref:`atan_est (x: float) : float <function-_at_math_c__c_atan_est_Cf>` 
  *  :ref:`atan2_est (y: float; x: float) : float <function-_at_math_c__c_atan2_est_Cf_Cf>` 
  *  :ref:`atan_est (x: float2) : float2 <function-_at_math_c__c_atan_est_Cf2>` 
  *  :ref:`atan_est (x: float3) : float3 <function-_at_math_c__c_atan_est_Cf3>` 
  *  :ref:`atan_est (x: float4) : float4 <function-_at_math_c__c_atan_est_Cf4>` 
  *  :ref:`atan2_est (y: float2; x: float2) : float2 <function-_at_math_c__c_atan2_est_Cf2_Cf2>` 
  *  :ref:`atan2_est (y: float3; x: float3) : float3 <function-_at_math_c__c_atan2_est_Cf3_Cf3>` 
  *  :ref:`atan2_est (y: float4; x: float4) : float4 <function-_at_math_c__c_atan2_est_Cf4_Cf4>` 
  *  :ref:`floori (x: float) : int <function-_at_math_c__c_floori_Cf>` 
  *  :ref:`ceili (x: float) : int <function-_at_math_c__c_ceili_Cf>` 
  *  :ref:`roundi (x: float) : int <function-_at_math_c__c_roundi_Cf>` 
  *  :ref:`trunci (x: float) : int <function-_at_math_c__c_trunci_Cf>` 
  *  :ref:`floori (x: double) : int <function-_at_math_c__c_floori_Cd>` 
  *  :ref:`ceili (x: double) : int <function-_at_math_c__c_ceili_Cd>` 
  *  :ref:`roundi (x: double) : int <function-_at_math_c__c_roundi_Cd>` 
  *  :ref:`trunci (x: double) : int <function-_at_math_c__c_trunci_Cd>` 
  *  :ref:`floori (x: float2) : int2 <function-_at_math_c__c_floori_Cf2>` 
  *  :ref:`ceili (x: float2) : int2 <function-_at_math_c__c_ceili_Cf2>` 
  *  :ref:`roundi (x: float2) : int2 <function-_at_math_c__c_roundi_Cf2>` 
  *  :ref:`trunci (x: float2) : int2 <function-_at_math_c__c_trunci_Cf2>` 
  *  :ref:`floori (x: float3) : int3 <function-_at_math_c__c_floori_Cf3>` 
  *  :ref:`ceili (x: float3) : int3 <function-_at_math_c__c_ceili_Cf3>` 
  *  :ref:`roundi (x: float3) : int3 <function-_at_math_c__c_roundi_Cf3>` 
  *  :ref:`trunci (x: float3) : int3 <function-_at_math_c__c_trunci_Cf3>` 
  *  :ref:`floori (x: float4) : int4 <function-_at_math_c__c_floori_Cf4>` 
  *  :ref:`ceili (x: float4) : int4 <function-_at_math_c__c_ceili_Cf4>` 
  *  :ref:`roundi (x: float4) : int4 <function-_at_math_c__c_roundi_Cf4>` 
  *  :ref:`trunci (x: float4) : int4 <function-_at_math_c__c_trunci_Cf4>` 
  *  :ref:`float4x4 implicit- (x: float4x4 implicit) : float4x4 <function-_at_math_c__c_-_CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`float3x4 implicit- (x: float3x4 implicit) : float3x4 <function-_at_math_c__c_-_CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`float3x3 implicit- (x: float3x3 implicit) : float3x3 <function-_at_math_c__c_-_CIH_ls_math_c__c_float3x3_gr_>` 

.. _function-_at_math_c__c_rcp_est_Cf:

.. das:function:: rcp_est(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_rcp_est_Cf2:

.. das:function:: rcp_est(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_rcp_est_Cf3:

.. das:function:: rcp_est(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_rcp_est_Cf4:

.. das:function:: rcp_est(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_fract_Cf:

.. das:function:: fract(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_rsqrt_Cf:

.. das:function:: rsqrt(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_rsqrt_est_Cf:

.. das:function:: rsqrt_est(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_fract_Cf2:

.. das:function:: fract(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_rsqrt_Cf2:

.. das:function:: rsqrt(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_rsqrt_est_Cf2:

.. das:function:: rsqrt_est(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_fract_Cf3:

.. das:function:: fract(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_rsqrt_Cf3:

.. das:function:: rsqrt(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_rsqrt_est_Cf3:

.. das:function:: rsqrt_est(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_fract_Cf4:

.. das:function:: fract(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_rsqrt_Cf4:

.. das:function:: rsqrt(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_rsqrt_est_Cf4:

.. das:function:: rsqrt_est(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_atan_est_Cf:

.. das:function:: atan_est(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_atan2_est_Cf_Cf:

.. das:function:: atan2_est(y: float; x: float) : float

// stub



:Arguments: * **y** : float

            * **x** : float

.. _function-_at_math_c__c_atan_est_Cf2:

.. das:function:: atan_est(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_atan_est_Cf3:

.. das:function:: atan_est(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_atan_est_Cf4:

.. das:function:: atan_est(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_atan2_est_Cf2_Cf2:

.. das:function:: atan2_est(y: float2; x: float2) : float2

// stub



:Arguments: * **y** : float2

            * **x** : float2

.. _function-_at_math_c__c_atan2_est_Cf3_Cf3:

.. das:function:: atan2_est(y: float3; x: float3) : float3

// stub



:Arguments: * **y** : float3

            * **x** : float3

.. _function-_at_math_c__c_atan2_est_Cf4_Cf4:

.. das:function:: atan2_est(y: float4; x: float4) : float4

// stub



:Arguments: * **y** : float4

            * **x** : float4

.. _function-_at_math_c__c_floori_Cf:

.. das:function:: floori(x: float) : int

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_ceili_Cf:

.. das:function:: ceili(x: float) : int

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_roundi_Cf:

.. das:function:: roundi(x: float) : int

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_trunci_Cf:

.. das:function:: trunci(x: float) : int

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_floori_Cd:

.. das:function:: floori(x: double) : int

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_ceili_Cd:

.. das:function:: ceili(x: double) : int

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_roundi_Cd:

.. das:function:: roundi(x: double) : int

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_trunci_Cd:

.. das:function:: trunci(x: double) : int

// stub



:Arguments: * **x** : double

.. _function-_at_math_c__c_floori_Cf2:

.. das:function:: floori(x: float2) : int2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_ceili_Cf2:

.. das:function:: ceili(x: float2) : int2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_roundi_Cf2:

.. das:function:: roundi(x: float2) : int2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_trunci_Cf2:

.. das:function:: trunci(x: float2) : int2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_floori_Cf3:

.. das:function:: floori(x: float3) : int3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_ceili_Cf3:

.. das:function:: ceili(x: float3) : int3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_roundi_Cf3:

.. das:function:: roundi(x: float3) : int3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_trunci_Cf3:

.. das:function:: trunci(x: float3) : int3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_floori_Cf4:

.. das:function:: floori(x: float4) : int4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_ceili_Cf4:

.. das:function:: ceili(x: float4) : int4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_roundi_Cf4:

.. das:function:: roundi(x: float4) : int4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_trunci_Cf4:

.. das:function:: trunci(x: float4) : int4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_-_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: float4x4 implicit-(x: float4x4 implicit) : float4x4

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_-_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: float3x4 implicit-(x: float3x4 implicit) : float3x4

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_-_CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: float3x3 implicit-(x: float3x3 implicit) : float3x3

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

+++++++++++
float3 only
+++++++++++

  *  :ref:`cross (x: float3; y: float3) : float3 <function-_at_math_c__c_cross_Cf3_Cf3>` 
  *  :ref:`distance (x: float2; y: float2) : float <function-_at_math_c__c_distance_Cf2_Cf2>` 
  *  :ref:`distance_sq (x: float2; y: float2) : float <function-_at_math_c__c_distance_sq_Cf2_Cf2>` 
  *  :ref:`inv_distance (x: float2; y: float2) : float <function-_at_math_c__c_inv_distance_Cf2_Cf2>` 
  *  :ref:`inv_distance_sq (x: float2; y: float2) : float <function-_at_math_c__c_inv_distance_sq_Cf2_Cf2>` 
  *  :ref:`distance (x: float3; y: float3) : float <function-_at_math_c__c_distance_Cf3_Cf3>` 
  *  :ref:`distance_sq (x: float3; y: float3) : float <function-_at_math_c__c_distance_sq_Cf3_Cf3>` 
  *  :ref:`inv_distance (x: float3; y: float3) : float <function-_at_math_c__c_inv_distance_Cf3_Cf3>` 
  *  :ref:`inv_distance_sq (x: float3; y: float3) : float <function-_at_math_c__c_inv_distance_sq_Cf3_Cf3>` 
  *  :ref:`distance (x: float4; y: float4) : float <function-_at_math_c__c_distance_Cf4_Cf4>` 
  *  :ref:`distance_sq (x: float4; y: float4) : float <function-_at_math_c__c_distance_sq_Cf4_Cf4>` 
  *  :ref:`inv_distance (x: float4; y: float4) : float <function-_at_math_c__c_inv_distance_Cf4_Cf4>` 
  *  :ref:`inv_distance_sq (x: float4; y: float4) : float <function-_at_math_c__c_inv_distance_sq_Cf4_Cf4>` 
  *  :ref:`reflect (v: float3; n: float3) : float3 <function-_at_math_c__c_reflect_Cf3_Cf3>` 
  *  :ref:`reflect (v: float2; n: float2) : float2 <function-_at_math_c__c_reflect_Cf2_Cf2>` 
  *  :ref:`refract (v: float3; n: float3; nint: float) : float3 <function-_at_math_c__c_refract_Cf3_Cf3_Cf>` 
  *  :ref:`refract (v: float2; n: float2; nint: float) : float2 <function-_at_math_c__c_refract_Cf2_Cf2_Cf>` 

.. _function-_at_math_c__c_cross_Cf3_Cf3:

.. das:function:: cross(x: float3; y: float3) : float3

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_distance_Cf2_Cf2:

.. das:function:: distance(x: float2; y: float2) : float

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_distance_sq_Cf2_Cf2:

.. das:function:: distance_sq(x: float2; y: float2) : float

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_inv_distance_Cf2_Cf2:

.. das:function:: inv_distance(x: float2; y: float2) : float

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_inv_distance_sq_Cf2_Cf2:

.. das:function:: inv_distance_sq(x: float2; y: float2) : float

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_distance_Cf3_Cf3:

.. das:function:: distance(x: float3; y: float3) : float

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_distance_sq_Cf3_Cf3:

.. das:function:: distance_sq(x: float3; y: float3) : float

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_inv_distance_Cf3_Cf3:

.. das:function:: inv_distance(x: float3; y: float3) : float

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_inv_distance_sq_Cf3_Cf3:

.. das:function:: inv_distance_sq(x: float3; y: float3) : float

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_distance_Cf4_Cf4:

.. das:function:: distance(x: float4; y: float4) : float

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_distance_sq_Cf4_Cf4:

.. das:function:: distance_sq(x: float4; y: float4) : float

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_inv_distance_Cf4_Cf4:

.. das:function:: inv_distance(x: float4; y: float4) : float

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_inv_distance_sq_Cf4_Cf4:

.. das:function:: inv_distance_sq(x: float4; y: float4) : float

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_reflect_Cf3_Cf3:

.. das:function:: reflect(v: float3; n: float3) : float3

// stub



:Arguments: * **v** : float3

            * **n** : float3

.. _function-_at_math_c__c_reflect_Cf2_Cf2:

.. das:function:: reflect(v: float2; n: float2) : float2

// stub



:Arguments: * **v** : float2

            * **n** : float2

.. _function-_at_math_c__c_refract_Cf3_Cf3_Cf:

.. das:function:: refract(v: float3; n: float3; nint: float) : float3

// stub



:Arguments: * **v** : float3

            * **n** : float3

            * **nint** : float

.. _function-_at_math_c__c_refract_Cf2_Cf2_Cf:

.. das:function:: refract(v: float2; n: float2; nint: float) : float2

// stub



:Arguments: * **v** : float2

            * **n** : float2

            * **nint** : float

++++++++++++++++++++++
float2, float3, float4
++++++++++++++++++++++

  *  :ref:`dot (x: float2; y: float2) : float <function-_at_math_c__c_dot_Cf2_Cf2>` 
  *  :ref:`dot (x: float3; y: float3) : float <function-_at_math_c__c_dot_Cf3_Cf3>` 
  *  :ref:`dot (x: float4; y: float4) : float <function-_at_math_c__c_dot_Cf4_Cf4>` 
  *  :ref:`fast_normalize (x: float2) : float2 <function-_at_math_c__c_fast_normalize_Cf2>` 
  *  :ref:`fast_normalize (x: float3) : float3 <function-_at_math_c__c_fast_normalize_Cf3>` 
  *  :ref:`fast_normalize (x: float4) : float4 <function-_at_math_c__c_fast_normalize_Cf4>` 
  *  :ref:`normalize (x: float2) : float2 <function-_at_math_c__c_normalize_Cf2>` 
  *  :ref:`normalize (x: float3) : float3 <function-_at_math_c__c_normalize_Cf3>` 
  *  :ref:`normalize (x: float4) : float4 <function-_at_math_c__c_normalize_Cf4>` 
  *  :ref:`length (x: float2) : float <function-_at_math_c__c_length_Cf2>` 
  *  :ref:`length (x: float3) : float <function-_at_math_c__c_length_Cf3>` 
  *  :ref:`length (x: float4) : float <function-_at_math_c__c_length_Cf4>` 
  *  :ref:`inv_length (x: float2) : float <function-_at_math_c__c_inv_length_Cf2>` 
  *  :ref:`inv_length (x: float3) : float <function-_at_math_c__c_inv_length_Cf3>` 
  *  :ref:`inv_length (x: float4) : float <function-_at_math_c__c_inv_length_Cf4>` 
  *  :ref:`inv_length_sq (x: float2) : float <function-_at_math_c__c_inv_length_sq_Cf2>` 
  *  :ref:`inv_length_sq (x: float3) : float <function-_at_math_c__c_inv_length_sq_Cf3>` 
  *  :ref:`inv_length_sq (x: float4) : float <function-_at_math_c__c_inv_length_sq_Cf4>` 
  *  :ref:`length_sq (x: float2) : float <function-_at_math_c__c_length_sq_Cf2>` 
  *  :ref:`length_sq (x: float3) : float <function-_at_math_c__c_length_sq_Cf3>` 
  *  :ref:`length_sq (x: float4) : float <function-_at_math_c__c_length_sq_Cf4>` 

.. _function-_at_math_c__c_dot_Cf2_Cf2:

.. das:function:: dot(x: float2; y: float2) : float

// stub



:Arguments: * **x** : float2

            * **y** : float2

.. _function-_at_math_c__c_dot_Cf3_Cf3:

.. das:function:: dot(x: float3; y: float3) : float

// stub



:Arguments: * **x** : float3

            * **y** : float3

.. _function-_at_math_c__c_dot_Cf4_Cf4:

.. das:function:: dot(x: float4; y: float4) : float

// stub



:Arguments: * **x** : float4

            * **y** : float4

.. _function-_at_math_c__c_fast_normalize_Cf2:

.. das:function:: fast_normalize(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_fast_normalize_Cf3:

.. das:function:: fast_normalize(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_fast_normalize_Cf4:

.. das:function:: fast_normalize(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_normalize_Cf2:

.. das:function:: normalize(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_normalize_Cf3:

.. das:function:: normalize(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_normalize_Cf4:

.. das:function:: normalize(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_length_Cf2:

.. das:function:: length(x: float2) : float

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_length_Cf3:

.. das:function:: length(x: float3) : float

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_length_Cf4:

.. das:function:: length(x: float4) : float

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_inv_length_Cf2:

.. das:function:: inv_length(x: float2) : float

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_inv_length_Cf3:

.. das:function:: inv_length(x: float3) : float

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_inv_length_Cf4:

.. das:function:: inv_length(x: float4) : float

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_inv_length_sq_Cf2:

.. das:function:: inv_length_sq(x: float2) : float

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_inv_length_sq_Cf3:

.. das:function:: inv_length_sq(x: float3) : float

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_inv_length_sq_Cf4:

.. das:function:: inv_length_sq(x: float4) : float

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_length_sq_Cf2:

.. das:function:: length_sq(x: float2) : float

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_length_sq_Cf3:

.. das:function:: length_sq(x: float3) : float

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_length_sq_Cf4:

.. das:function:: length_sq(x: float4) : float

// stub



:Arguments: * **x** : float4

+++++++++++++++
Noise functions
+++++++++++++++

  *  :ref:`uint32_hash (seed: uint) : uint <function-_at_math_c__c_uint32_hash_Cu>` 
  *  :ref:`uint_noise_1D (position: int; seed: uint) : uint <function-_at_math_c__c_uint_noise_1D_Ci_Cu>` 
  *  :ref:`uint_noise_2D (position: int2; seed: uint) : uint <function-_at_math_c__c_uint_noise_2D_Ci2_Cu>` 
  *  :ref:`uint_noise_3D (position: int3; seed: uint) : uint <function-_at_math_c__c_uint_noise_3D_Ci3_Cu>` 

.. _function-_at_math_c__c_uint32_hash_Cu:

.. das:function:: uint32_hash(seed: uint) : uint

// stub



:Arguments: * **seed** : uint

.. _function-_at_math_c__c_uint_noise_1D_Ci_Cu:

.. das:function:: uint_noise_1D(position: int; seed: uint) : uint

// stub



:Arguments: * **position** : int

            * **seed** : uint

.. _function-_at_math_c__c_uint_noise_2D_Ci2_Cu:

.. das:function:: uint_noise_2D(position: int2; seed: uint) : uint

// stub



:Arguments: * **position** : int2

            * **seed** : uint

.. _function-_at_math_c__c_uint_noise_3D_Ci3_Cu:

.. das:function:: uint_noise_3D(position: int3; seed: uint) : uint

// stub



:Arguments: * **position** : int3

            * **seed** : uint

++++++++++++++
lerp/mad/clamp
++++++++++++++

  *  :ref:`mad (a: float; b: float; c: float) : float <function-_at_math_c__c_mad_Cf_Cf_Cf>` 
  *  :ref:`lerp (a: float; b: float; t: float) : float <function-_at_math_c__c_lerp_Cf_Cf_Cf>` 
  *  :ref:`mad (a: float2; b: float2; c: float2) : float2 <function-_at_math_c__c_mad_Cf2_Cf2_Cf2>` 
  *  :ref:`lerp (a: float2; b: float2; t: float2) : float2 <function-_at_math_c__c_lerp_Cf2_Cf2_Cf2>` 
  *  :ref:`mad (a: float3; b: float3; c: float3) : float3 <function-_at_math_c__c_mad_Cf3_Cf3_Cf3>` 
  *  :ref:`lerp (a: float3; b: float3; t: float3) : float3 <function-_at_math_c__c_lerp_Cf3_Cf3_Cf3>` 
  *  :ref:`mad (a: float4; b: float4; c: float4) : float4 <function-_at_math_c__c_mad_Cf4_Cf4_Cf4>` 
  *  :ref:`lerp (a: float4; b: float4; t: float4) : float4 <function-_at_math_c__c_lerp_Cf4_Cf4_Cf4>` 
  *  :ref:`mad (a: float2; b: float; c: float2) : float2 <function-_at_math_c__c_mad_Cf2_Cf_Cf2>` 
  *  :ref:`mad (a: float3; b: float; c: float3) : float3 <function-_at_math_c__c_mad_Cf3_Cf_Cf3>` 
  *  :ref:`mad (a: float4; b: float; c: float4) : float4 <function-_at_math_c__c_mad_Cf4_Cf_Cf4>` 
  *  :ref:`mad (a: int; b: int; c: int) : int <function-_at_math_c__c_mad_Ci_Ci_Ci>` 
  *  :ref:`mad (a: int2; b: int2; c: int2) : int2 <function-_at_math_c__c_mad_Ci2_Ci2_Ci2>` 
  *  :ref:`mad (a: int3; b: int3; c: int3) : int3 <function-_at_math_c__c_mad_Ci3_Ci3_Ci3>` 
  *  :ref:`mad (a: int4; b: int4; c: int4) : int4 <function-_at_math_c__c_mad_Ci4_Ci4_Ci4>` 
  *  :ref:`mad (a: int2; b: int; c: int2) : int2 <function-_at_math_c__c_mad_Ci2_Ci_Ci2>` 
  *  :ref:`mad (a: int3; b: int; c: int3) : int3 <function-_at_math_c__c_mad_Ci3_Ci_Ci3>` 
  *  :ref:`mad (a: int4; b: int; c: int4) : int4 <function-_at_math_c__c_mad_Ci4_Ci_Ci4>` 
  *  :ref:`mad (a: uint; b: uint; c: uint) : uint <function-_at_math_c__c_mad_Cu_Cu_Cu>` 
  *  :ref:`mad (a: uint2; b: uint2; c: uint2) : uint2 <function-_at_math_c__c_mad_Cu2_Cu2_Cu2>` 
  *  :ref:`mad (a: uint3; b: uint3; c: uint3) : uint3 <function-_at_math_c__c_mad_Cu3_Cu3_Cu3>` 
  *  :ref:`mad (a: uint4; b: uint4; c: uint4) : uint4 <function-_at_math_c__c_mad_Cu4_Cu4_Cu4>` 
  *  :ref:`mad (a: uint2; b: uint; c: uint2) : uint2 <function-_at_math_c__c_mad_Cu2_Cu_Cu2>` 
  *  :ref:`mad (a: uint3; b: uint; c: uint3) : uint3 <function-_at_math_c__c_mad_Cu3_Cu_Cu3>` 
  *  :ref:`mad (a: uint4; b: uint; c: uint4) : uint4 <function-_at_math_c__c_mad_Cu4_Cu_Cu4>` 
  *  :ref:`mad (a: double; b: double; c: double) : double <function-_at_math_c__c_mad_Cd_Cd_Cd>` 
  *  :ref:`lerp (a: double; b: double; t: double) : double <function-_at_math_c__c_lerp_Cd_Cd_Cd>` 
  *  :ref:`clamp (t: int; a: int; b: int) : int <function-_at_math_c__c_clamp_Ci_Ci_Ci>` 
  *  :ref:`clamp (t: int2; a: int2; b: int2) : int2 <function-_at_math_c__c_clamp_Ci2_Ci2_Ci2>` 
  *  :ref:`clamp (t: int3; a: int3; b: int3) : int3 <function-_at_math_c__c_clamp_Ci3_Ci3_Ci3>` 
  *  :ref:`clamp (t: int4; a: int4; b: int4) : int4 <function-_at_math_c__c_clamp_Ci4_Ci4_Ci4>` 
  *  :ref:`clamp (t: uint; a: uint; b: uint) : uint <function-_at_math_c__c_clamp_Cu_Cu_Cu>` 
  *  :ref:`clamp (t: uint2; a: uint2; b: uint2) : uint2 <function-_at_math_c__c_clamp_Cu2_Cu2_Cu2>` 
  *  :ref:`clamp (t: uint3; a: uint3; b: uint3) : uint3 <function-_at_math_c__c_clamp_Cu3_Cu3_Cu3>` 
  *  :ref:`clamp (t: uint4; a: uint4; b: uint4) : uint4 <function-_at_math_c__c_clamp_Cu4_Cu4_Cu4>` 
  *  :ref:`clamp (t: float; a: float; b: float) : float <function-_at_math_c__c_clamp_Cf_Cf_Cf>` 
  *  :ref:`clamp (t: float2; a: float2; b: float2) : float2 <function-_at_math_c__c_clamp_Cf2_Cf2_Cf2>` 
  *  :ref:`clamp (t: float3; a: float3; b: float3) : float3 <function-_at_math_c__c_clamp_Cf3_Cf3_Cf3>` 
  *  :ref:`clamp (t: float4; a: float4; b: float4) : float4 <function-_at_math_c__c_clamp_Cf4_Cf4_Cf4>` 
  *  :ref:`clamp (t: double; a: double; b: double) : double <function-_at_math_c__c_clamp_Cd_Cd_Cd>` 
  *  :ref:`clamp (t: int64; a: int64; b: int64) : int64 <function-_at_math_c__c_clamp_Ci64_Ci64_Ci64>` 
  *  :ref:`clamp (t: uint64; a: uint64; b: uint64) : uint64 <function-_at_math_c__c_clamp_Cu64_Cu64_Cu64>` 
  *  :ref:`lerp (a: float2; b: float2; t: float) : float2 <function-_at_math_c__c_lerp_Cf2_Cf2_Cf>` 
  *  :ref:`lerp (a: float3; b: float3; t: float) : float3 <function-_at_math_c__c_lerp_Cf3_Cf3_Cf>` 
  *  :ref:`lerp (a: float4; b: float4; t: float) : float4 <function-_at_math_c__c_lerp_Cf4_Cf4_Cf>` 

.. _function-_at_math_c__c_mad_Cf_Cf_Cf:

.. das:function:: mad(a: float; b: float; c: float) : float

// stub



:Arguments: * **a** : float

            * **b** : float

            * **c** : float

.. _function-_at_math_c__c_lerp_Cf_Cf_Cf:

.. das:function:: lerp(a: float; b: float; t: float) : float

// stub



:Arguments: * **a** : float

            * **b** : float

            * **t** : float

.. _function-_at_math_c__c_mad_Cf2_Cf2_Cf2:

.. das:function:: mad(a: float2; b: float2; c: float2) : float2

// stub



:Arguments: * **a** : float2

            * **b** : float2

            * **c** : float2

.. _function-_at_math_c__c_lerp_Cf2_Cf2_Cf2:

.. das:function:: lerp(a: float2; b: float2; t: float2) : float2

// stub



:Arguments: * **a** : float2

            * **b** : float2

            * **t** : float2

.. _function-_at_math_c__c_mad_Cf3_Cf3_Cf3:

.. das:function:: mad(a: float3; b: float3; c: float3) : float3

// stub



:Arguments: * **a** : float3

            * **b** : float3

            * **c** : float3

.. _function-_at_math_c__c_lerp_Cf3_Cf3_Cf3:

.. das:function:: lerp(a: float3; b: float3; t: float3) : float3

// stub



:Arguments: * **a** : float3

            * **b** : float3

            * **t** : float3

.. _function-_at_math_c__c_mad_Cf4_Cf4_Cf4:

.. das:function:: mad(a: float4; b: float4; c: float4) : float4

// stub



:Arguments: * **a** : float4

            * **b** : float4

            * **c** : float4

.. _function-_at_math_c__c_lerp_Cf4_Cf4_Cf4:

.. das:function:: lerp(a: float4; b: float4; t: float4) : float4

// stub



:Arguments: * **a** : float4

            * **b** : float4

            * **t** : float4

.. _function-_at_math_c__c_mad_Cf2_Cf_Cf2:

.. das:function:: mad(a: float2; b: float; c: float2) : float2

// stub



:Arguments: * **a** : float2

            * **b** : float

            * **c** : float2

.. _function-_at_math_c__c_mad_Cf3_Cf_Cf3:

.. das:function:: mad(a: float3; b: float; c: float3) : float3

// stub



:Arguments: * **a** : float3

            * **b** : float

            * **c** : float3

.. _function-_at_math_c__c_mad_Cf4_Cf_Cf4:

.. das:function:: mad(a: float4; b: float; c: float4) : float4

// stub



:Arguments: * **a** : float4

            * **b** : float

            * **c** : float4

.. _function-_at_math_c__c_mad_Ci_Ci_Ci:

.. das:function:: mad(a: int; b: int; c: int) : int

// stub



:Arguments: * **a** : int

            * **b** : int

            * **c** : int

.. _function-_at_math_c__c_mad_Ci2_Ci2_Ci2:

.. das:function:: mad(a: int2; b: int2; c: int2) : int2

// stub



:Arguments: * **a** : int2

            * **b** : int2

            * **c** : int2

.. _function-_at_math_c__c_mad_Ci3_Ci3_Ci3:

.. das:function:: mad(a: int3; b: int3; c: int3) : int3

// stub



:Arguments: * **a** : int3

            * **b** : int3

            * **c** : int3

.. _function-_at_math_c__c_mad_Ci4_Ci4_Ci4:

.. das:function:: mad(a: int4; b: int4; c: int4) : int4

// stub



:Arguments: * **a** : int4

            * **b** : int4

            * **c** : int4

.. _function-_at_math_c__c_mad_Ci2_Ci_Ci2:

.. das:function:: mad(a: int2; b: int; c: int2) : int2

// stub



:Arguments: * **a** : int2

            * **b** : int

            * **c** : int2

.. _function-_at_math_c__c_mad_Ci3_Ci_Ci3:

.. das:function:: mad(a: int3; b: int; c: int3) : int3

// stub



:Arguments: * **a** : int3

            * **b** : int

            * **c** : int3

.. _function-_at_math_c__c_mad_Ci4_Ci_Ci4:

.. das:function:: mad(a: int4; b: int; c: int4) : int4

// stub



:Arguments: * **a** : int4

            * **b** : int

            * **c** : int4

.. _function-_at_math_c__c_mad_Cu_Cu_Cu:

.. das:function:: mad(a: uint; b: uint; c: uint) : uint

// stub



:Arguments: * **a** : uint

            * **b** : uint

            * **c** : uint

.. _function-_at_math_c__c_mad_Cu2_Cu2_Cu2:

.. das:function:: mad(a: uint2; b: uint2; c: uint2) : uint2

// stub



:Arguments: * **a** : uint2

            * **b** : uint2

            * **c** : uint2

.. _function-_at_math_c__c_mad_Cu3_Cu3_Cu3:

.. das:function:: mad(a: uint3; b: uint3; c: uint3) : uint3

// stub



:Arguments: * **a** : uint3

            * **b** : uint3

            * **c** : uint3

.. _function-_at_math_c__c_mad_Cu4_Cu4_Cu4:

.. das:function:: mad(a: uint4; b: uint4; c: uint4) : uint4

// stub



:Arguments: * **a** : uint4

            * **b** : uint4

            * **c** : uint4

.. _function-_at_math_c__c_mad_Cu2_Cu_Cu2:

.. das:function:: mad(a: uint2; b: uint; c: uint2) : uint2

// stub



:Arguments: * **a** : uint2

            * **b** : uint

            * **c** : uint2

.. _function-_at_math_c__c_mad_Cu3_Cu_Cu3:

.. das:function:: mad(a: uint3; b: uint; c: uint3) : uint3

// stub



:Arguments: * **a** : uint3

            * **b** : uint

            * **c** : uint3

.. _function-_at_math_c__c_mad_Cu4_Cu_Cu4:

.. das:function:: mad(a: uint4; b: uint; c: uint4) : uint4

// stub



:Arguments: * **a** : uint4

            * **b** : uint

            * **c** : uint4

.. _function-_at_math_c__c_mad_Cd_Cd_Cd:

.. das:function:: mad(a: double; b: double; c: double) : double

// stub



:Arguments: * **a** : double

            * **b** : double

            * **c** : double

.. _function-_at_math_c__c_lerp_Cd_Cd_Cd:

.. das:function:: lerp(a: double; b: double; t: double) : double

// stub



:Arguments: * **a** : double

            * **b** : double

            * **t** : double

.. _function-_at_math_c__c_clamp_Ci_Ci_Ci:

.. das:function:: clamp(t: int; a: int; b: int) : int

// stub



:Arguments: * **t** : int

            * **a** : int

            * **b** : int

.. _function-_at_math_c__c_clamp_Ci2_Ci2_Ci2:

.. das:function:: clamp(t: int2; a: int2; b: int2) : int2

// stub



:Arguments: * **t** : int2

            * **a** : int2

            * **b** : int2

.. _function-_at_math_c__c_clamp_Ci3_Ci3_Ci3:

.. das:function:: clamp(t: int3; a: int3; b: int3) : int3

// stub



:Arguments: * **t** : int3

            * **a** : int3

            * **b** : int3

.. _function-_at_math_c__c_clamp_Ci4_Ci4_Ci4:

.. das:function:: clamp(t: int4; a: int4; b: int4) : int4

// stub



:Arguments: * **t** : int4

            * **a** : int4

            * **b** : int4

.. _function-_at_math_c__c_clamp_Cu_Cu_Cu:

.. das:function:: clamp(t: uint; a: uint; b: uint) : uint

// stub



:Arguments: * **t** : uint

            * **a** : uint

            * **b** : uint

.. _function-_at_math_c__c_clamp_Cu2_Cu2_Cu2:

.. das:function:: clamp(t: uint2; a: uint2; b: uint2) : uint2

// stub



:Arguments: * **t** : uint2

            * **a** : uint2

            * **b** : uint2

.. _function-_at_math_c__c_clamp_Cu3_Cu3_Cu3:

.. das:function:: clamp(t: uint3; a: uint3; b: uint3) : uint3

// stub



:Arguments: * **t** : uint3

            * **a** : uint3

            * **b** : uint3

.. _function-_at_math_c__c_clamp_Cu4_Cu4_Cu4:

.. das:function:: clamp(t: uint4; a: uint4; b: uint4) : uint4

// stub



:Arguments: * **t** : uint4

            * **a** : uint4

            * **b** : uint4

.. _function-_at_math_c__c_clamp_Cf_Cf_Cf:

.. das:function:: clamp(t: float; a: float; b: float) : float

// stub



:Arguments: * **t** : float

            * **a** : float

            * **b** : float

.. _function-_at_math_c__c_clamp_Cf2_Cf2_Cf2:

.. das:function:: clamp(t: float2; a: float2; b: float2) : float2

// stub



:Arguments: * **t** : float2

            * **a** : float2

            * **b** : float2

.. _function-_at_math_c__c_clamp_Cf3_Cf3_Cf3:

.. das:function:: clamp(t: float3; a: float3; b: float3) : float3

// stub



:Arguments: * **t** : float3

            * **a** : float3

            * **b** : float3

.. _function-_at_math_c__c_clamp_Cf4_Cf4_Cf4:

.. das:function:: clamp(t: float4; a: float4; b: float4) : float4

// stub



:Arguments: * **t** : float4

            * **a** : float4

            * **b** : float4

.. _function-_at_math_c__c_clamp_Cd_Cd_Cd:

.. das:function:: clamp(t: double; a: double; b: double) : double

// stub



:Arguments: * **t** : double

            * **a** : double

            * **b** : double

.. _function-_at_math_c__c_clamp_Ci64_Ci64_Ci64:

.. das:function:: clamp(t: int64; a: int64; b: int64) : int64

// stub



:Arguments: * **t** : int64

            * **a** : int64

            * **b** : int64

.. _function-_at_math_c__c_clamp_Cu64_Cu64_Cu64:

.. das:function:: clamp(t: uint64; a: uint64; b: uint64) : uint64

// stub



:Arguments: * **t** : uint64

            * **a** : uint64

            * **b** : uint64

.. _function-_at_math_c__c_lerp_Cf2_Cf2_Cf:

.. das:function:: lerp(a: float2; b: float2; t: float) : float2

// stub



:Arguments: * **a** : float2

            * **b** : float2

            * **t** : float

.. _function-_at_math_c__c_lerp_Cf3_Cf3_Cf:

.. das:function:: lerp(a: float3; b: float3; t: float) : float3

// stub



:Arguments: * **a** : float3

            * **b** : float3

            * **t** : float

.. _function-_at_math_c__c_lerp_Cf4_Cf4_Cf:

.. das:function:: lerp(a: float4; b: float4; t: float) : float4

// stub



:Arguments: * **a** : float4

            * **b** : float4

            * **t** : float

+++++++++++++++++
Matrix operations
+++++++++++++++++

  *  :ref:`float4x4 implicit* (x: float4x4 implicit; y: float4x4 implicit) : float4x4 <function-_at_math_c__c__st__CIH_ls_math_c__c_float4x4_gr__CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`float4x4 implicit== (x: float4x4 implicit; y: float4x4 implicit) : bool <function-_at_math_c__c__eq__eq__CIH_ls_math_c__c_float4x4_gr__CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`float4x4 implicit\!= (x: float4x4 implicit; y: float4x4 implicit) : bool <function-_at_math_c__c__ex__eq__CIH_ls_math_c__c_float4x4_gr__CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`float3x4 implicit* (x: float3x4 implicit; y: float3x4 implicit) : float3x4 <function-_at_math_c__c__st__CIH_ls_math_c__c_float3x4_gr__CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`float3x4 implicit* (x: float3x4 implicit; y: float3) : float3 <function-_at_math_c__c__st__CIH_ls_math_c__c_float3x4_gr__Cf3>` 
  *  :ref:`float4x4 implicit* (x: float4x4 implicit; y: float4) : float4 <function-_at_math_c__c__st__CIH_ls_math_c__c_float4x4_gr__Cf4>` 
  *  :ref:`float3x4 implicit== (x: float3x4 implicit; y: float3x4 implicit) : bool <function-_at_math_c__c__eq__eq__CIH_ls_math_c__c_float3x4_gr__CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`float3x4 implicit\!= (x: float3x4 implicit; y: float3x4 implicit) : bool <function-_at_math_c__c__ex__eq__CIH_ls_math_c__c_float3x4_gr__CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`float3x3 implicit* (x: float3x3 implicit; y: float3x3 implicit) : float3x3 <function-_at_math_c__c__st__CIH_ls_math_c__c_float3x3_gr__CIH_ls_math_c__c_float3x3_gr_>` 
  *  :ref:`float3x3 implicit* (x: float3x3 implicit; y: float3) : float3 <function-_at_math_c__c__st__CIH_ls_math_c__c_float3x3_gr__Cf3>` 
  *  :ref:`float3x3 implicit== (x: float3x3 implicit; y: float3x3 implicit) : bool <function-_at_math_c__c__eq__eq__CIH_ls_math_c__c_float3x3_gr__CIH_ls_math_c__c_float3x3_gr_>` 
  *  :ref:`float3x3 implicit\!= (x: float3x3 implicit; y: float3x3 implicit) : bool <function-_at_math_c__c__ex__eq__CIH_ls_math_c__c_float3x3_gr__CIH_ls_math_c__c_float3x3_gr_>` 

.. _function-_at_math_c__c__st__CIH_ls_math_c__c_float4x4_gr__CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: float4x4 implicit*(x: float4x4 implicit; y: float4x4 implicit) : float4x4

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c__eq__eq__CIH_ls_math_c__c_float4x4_gr__CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: float4x4 implicit==(x: float4x4 implicit; y: float4x4 implicit) : bool

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c__ex__eq__CIH_ls_math_c__c_float4x4_gr__CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: float4x4 implicit!=(x: float4x4 implicit; y: float4x4 implicit) : bool

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c__st__CIH_ls_math_c__c_float3x4_gr__CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: float3x4 implicit*(x: float3x4 implicit; y: float3x4 implicit) : float3x4

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c__st__CIH_ls_math_c__c_float3x4_gr__Cf3:

.. das:function:: float3x4 implicit*(x: float3x4 implicit; y: float3) : float3

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** : float3

.. _function-_at_math_c__c__st__CIH_ls_math_c__c_float4x4_gr__Cf4:

.. das:function:: float4x4 implicit*(x: float4x4 implicit; y: float4) : float4

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **y** : float4

.. _function-_at_math_c__c__eq__eq__CIH_ls_math_c__c_float3x4_gr__CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: float3x4 implicit==(x: float3x4 implicit; y: float3x4 implicit) : bool

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c__ex__eq__CIH_ls_math_c__c_float3x4_gr__CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: float3x4 implicit!=(x: float3x4 implicit; y: float3x4 implicit) : bool

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c__st__CIH_ls_math_c__c_float3x3_gr__CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: float3x3 implicit*(x: float3x3 implicit; y: float3x3 implicit) : float3x3

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-_at_math_c__c__st__CIH_ls_math_c__c_float3x3_gr__Cf3:

.. das:function:: float3x3 implicit*(x: float3x3 implicit; y: float3) : float3

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** : float3

.. _function-_at_math_c__c__eq__eq__CIH_ls_math_c__c_float3x3_gr__CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: float3x3 implicit==(x: float3x3 implicit; y: float3x3 implicit) : bool

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-_at_math_c__c__ex__eq__CIH_ls_math_c__c_float3x3_gr__CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: float3x3 implicit!=(x: float3x3 implicit; y: float3x3 implicit) : bool

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

            * **y** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

+++++++++++++++++++
Matrix initializers
+++++++++++++++++++

  *  :ref:`float3x3 () : float3x3 <function-_at_math_c__c_float3x3>` 
  *  :ref:`float3x4 () : float3x4 <function-_at_math_c__c_float3x4>` 
  *  :ref:`float4x4 () : float4x4 <function-_at_math_c__c_float4x4>` 
  *  :ref:`float4x4 (arg0: float3x4 implicit) : float4x4 <function-_at_math_c__c_float4x4_CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`identity4x4 () : float4x4 <function-_at_math_c__c_identity4x4>` 
  *  :ref:`float3x4 (arg0: float4x4 implicit) : float3x4 <function-_at_math_c__c_float3x4_CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`identity3x4 () : float3x4 <function-_at_math_c__c_identity3x4>` 
  *  :ref:`float3x3 (arg0: float4x4 implicit) : float3x3 <function-_at_math_c__c_float3x3_CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`float3x3 (arg0: float3x4 implicit) : float3x3 <function-_at_math_c__c_float3x3_CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`identity3x3 () : float3x3 <function-_at_math_c__c_identity3x3>` 

.. _function-_at_math_c__c_float3x3:

.. das:function:: float3x3() : float3x3

// stub


.. _function-_at_math_c__c_float3x4:

.. das:function:: float3x4() : float3x4

// stub


.. _function-_at_math_c__c_float4x4:

.. das:function:: float4x4() : float4x4

// stub


.. _function-_at_math_c__c_float4x4_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: float4x4(arg0: float3x4 implicit) : float4x4

// stub



:Arguments: * **arg0** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_identity4x4:

.. das:function:: identity4x4() : float4x4

// stub


.. _function-_at_math_c__c_float3x4_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: float3x4(arg0: float4x4 implicit) : float3x4

// stub



:Arguments: * **arg0** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_identity3x4:

.. das:function:: identity3x4() : float3x4

// stub


.. _function-_at_math_c__c_float3x3_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: float3x3(arg0: float4x4 implicit) : float3x3

// stub



:Arguments: * **arg0** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_float3x3_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: float3x3(arg0: float3x4 implicit) : float3x3

// stub



:Arguments: * **arg0** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_identity3x3:

.. das:function:: identity3x3() : float3x3

// stub


+++++++++++++++++++
Matrix manipulation
+++++++++++++++++++

  *  :ref:`identity (x: float4x4 implicit) <function-_at_math_c__c_identity_IH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`translation (xyz: float3) : float4x4 <function-_at_math_c__c_translation_Cf3>` 
  *  :ref:`transpose (x: float4x4 implicit) : float4x4 <function-_at_math_c__c_transpose_CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`persp_forward (wk: float; hk: float; zn: float; zf: float) : float4x4 <function-_at_math_c__c_persp_forward_Cf_Cf_Cf_Cf>` 
  *  :ref:`persp_reverse (wk: float; hk: float; zn: float; zf: float) : float4x4 <function-_at_math_c__c_persp_reverse_Cf_Cf_Cf_Cf>` 
  *  :ref:`look_at (eye: float3; at: float3; up: float3) : float4x4 <function-_at_math_c__c_look_at_Cf3_Cf3_Cf3>` 
  *  :ref:`compose (pos: float3; rot: float4; scale: float3) : float4x4 <function-_at_math_c__c_compose_Cf3_Cf4_Cf3>` 
  *  :ref:`decompose (mat: float4x4 implicit; pos: float3& implicit; rot: float4& implicit; scale: float3& implicit) <function-_at_math_c__c_decompose_CIH_ls_math_c__c_float4x4_gr__&If3_&If4_&If3>` 
  *  :ref:`identity (x: float3x4 implicit) <function-_at_math_c__c_identity_IH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`inverse (x: float3x4 implicit) : float3x4 <function-_at_math_c__c_inverse_CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`inverse (m: float4x4 implicit) : float4x4 <function-_at_math_c__c_inverse_CIH_ls_math_c__c_float4x4_gr_>` 
  *  :ref:`orthonormal_inverse (m: float3x3 implicit) : float3x3 <function-_at_math_c__c_orthonormal_inverse_CIH_ls_math_c__c_float3x3_gr_>` 
  *  :ref:`orthonormal_inverse (m: float3x4 implicit) : float3x4 <function-_at_math_c__c_orthonormal_inverse_CIH_ls_math_c__c_float3x4_gr_>` 
  *  :ref:`rotate (x: float3x4 implicit; y: float3) : float3 <function-_at_math_c__c_rotate_CIH_ls_math_c__c_float3x4_gr__Cf3>` 
  *  :ref:`identity (x: float3x3 implicit) <function-_at_math_c__c_identity_IH_ls_math_c__c_float3x3_gr_>` 

.. _function-_at_math_c__c_identity_IH_ls_math_c__c_float4x4_gr_:

.. das:function:: identity(x: float4x4 implicit)

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_translation_Cf3:

.. das:function:: translation(xyz: float3) : float4x4

// stub



:Arguments: * **xyz** : float3

.. _function-_at_math_c__c_transpose_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: transpose(x: float4x4 implicit) : float4x4

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_persp_forward_Cf_Cf_Cf_Cf:

.. das:function:: persp_forward(wk: float; hk: float; zn: float; zf: float) : float4x4

// stub



:Arguments: * **wk** : float

            * **hk** : float

            * **zn** : float

            * **zf** : float

.. _function-_at_math_c__c_persp_reverse_Cf_Cf_Cf_Cf:

.. das:function:: persp_reverse(wk: float; hk: float; zn: float; zf: float) : float4x4

// stub



:Arguments: * **wk** : float

            * **hk** : float

            * **zn** : float

            * **zf** : float

.. _function-_at_math_c__c_look_at_Cf3_Cf3_Cf3:

.. das:function:: look_at(eye: float3; at: float3; up: float3) : float4x4

// stub



:Arguments: * **eye** : float3

            * **at** : float3

            * **up** : float3

.. _function-_at_math_c__c_compose_Cf3_Cf4_Cf3:

.. das:function:: compose(pos: float3; rot: float4; scale: float3) : float4x4

// stub



:Arguments: * **pos** : float3

            * **rot** : float4

            * **scale** : float3

.. _function-_at_math_c__c_decompose_CIH_ls_math_c__c_float4x4_gr__&If3_&If4_&If3:

.. das:function:: decompose(mat: float4x4 implicit; pos: float3& implicit; rot: float4& implicit; scale: float3& implicit)

// stub



:Arguments: * **mat** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

            * **pos** : float3& implicit

            * **rot** : float4& implicit

            * **scale** : float3& implicit

.. _function-_at_math_c__c_identity_IH_ls_math_c__c_float3x4_gr_:

.. das:function:: identity(x: float3x4 implicit)

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_inverse_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: inverse(x: float3x4 implicit) : float3x4

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_inverse_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: inverse(m: float4x4 implicit) : float4x4

// stub



:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_orthonormal_inverse_CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: orthonormal_inverse(m: float3x3 implicit) : float3x3

// stub



:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-_at_math_c__c_orthonormal_inverse_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: orthonormal_inverse(m: float3x4 implicit) : float3x4

// stub



:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_rotate_CIH_ls_math_c__c_float3x4_gr__Cf3:

.. das:function:: rotate(x: float3x4 implicit; y: float3) : float3

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

            * **y** : float3

.. _function-_at_math_c__c_identity_IH_ls_math_c__c_float3x3_gr_:

.. das:function:: identity(x: float3x3 implicit)

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

+++++++++++++++++++++
Quaternion operations
+++++++++++++++++++++

  *  :ref:`quat_from_unit_arc (v0: float3; v1: float3) : float4 <function-_at_math_c__c_quat_from_unit_arc_Cf3_Cf3>` 
  *  :ref:`quat_from_unit_vec_ang (v: float3; ang: float) : float4 <function-_at_math_c__c_quat_from_unit_vec_ang_Cf3_Cf>` 
  *  :ref:`quat_mul (q1: float4; q2: float4) : float4 <function-_at_math_c__c_quat_mul_Cf4_Cf4>` 
  *  :ref:`quat_mul_vec (q: float4; v: float3) : float3 <function-_at_math_c__c_quat_mul_vec_Cf4_Cf3>` 
  *  :ref:`quat_conjugate (q: float4) : float4 <function-_at_math_c__c_quat_conjugate_Cf4>` 

.. _function-_at_math_c__c_quat_from_unit_arc_Cf3_Cf3:

.. das:function:: quat_from_unit_arc(v0: float3; v1: float3) : float4

// stub



:Arguments: * **v0** : float3

            * **v1** : float3

.. _function-_at_math_c__c_quat_from_unit_vec_ang_Cf3_Cf:

.. das:function:: quat_from_unit_vec_ang(v: float3; ang: float) : float4

// stub



:Arguments: * **v** : float3

            * **ang** : float

.. _function-_at_math_c__c_quat_mul_Cf4_Cf4:

.. das:function:: quat_mul(q1: float4; q2: float4) : float4

// stub



:Arguments: * **q1** : float4

            * **q2** : float4

.. _function-_at_math_c__c_quat_mul_vec_Cf4_Cf3:

.. das:function:: quat_mul_vec(q: float4; v: float3) : float3

// stub



:Arguments: * **q** : float4

            * **v** : float3

.. _function-_at_math_c__c_quat_conjugate_Cf4:

.. das:function:: quat_conjugate(q: float4) : float4

// stub



:Arguments: * **q** : float4

+++++++++++++++++++++
Packing and unpacking
+++++++++++++++++++++

  *  :ref:`pack_float_to_byte (x: float4) : uint <function-_at_math_c__c_pack_float_to_byte_Cf4>` 
  *  :ref:`unpack_byte_to_float (x: uint) : float4 <function-_at_math_c__c_unpack_byte_to_float_Cu>` 

.. _function-_at_math_c__c_pack_float_to_byte_Cf4:

.. das:function:: pack_float_to_byte(x: float4) : uint

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c_unpack_byte_to_float_Cu:

.. das:function:: unpack_byte_to_float(x: uint) : float4

// stub



:Arguments: * **x** : uint

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_math_c__c_round_Cf:

.. das:function:: round(x: float) : float

// stub



:Arguments: * **x** : float

.. _function-_at_math_c__c_round_Cf2:

.. das:function:: round(x: float2) : float2

// stub



:Arguments: * **x** : float2

.. _function-_at_math_c__c_round_Cf3:

.. das:function:: round(x: float3) : float3

// stub



:Arguments: * **x** : float3

.. _function-_at_math_c__c_round_Cf4:

.. das:function:: round(x: float4) : float4

// stub



:Arguments: * **x** : float4

.. _function-_at_math_c__c__lb__rb__I_eq_H_ls_math_c__c_float4x4_gr__Ci_C_c_C_l:

.. das:function:: float4x4 implicit ==const[](m: float4x4 implicit ==const; i: int) : float4&

// stub



:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : int

.. _function-_at_math_c__c__lb__rb__CI_eq_H_ls_math_c__c_float4x4_gr__Ci_C_c_C_l:

.. das:function:: float4x4 const implicit ==const[](m: float4x4 const implicit ==const; i: int) : float4 const&

// stub



:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : int

.. _function-_at_math_c__c__lb__rb__I_eq_H_ls_math_c__c_float4x4_gr__Cu_C_c_C_l:

.. das:function:: float4x4 implicit ==const[](m: float4x4 implicit ==const; i: uint) : float4&

// stub



:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : uint

.. _function-_at_math_c__c__lb__rb__CI_eq_H_ls_math_c__c_float4x4_gr__Cu_C_c_C_l:

.. das:function:: float4x4 const implicit ==const[](m: float4x4 const implicit ==const; i: uint) : float4 const&

// stub



:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit!

            * **i** : uint

.. _function-_at_math_c__c_determinant_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: determinant(x: float4x4 implicit) : float

// stub



:Arguments: * **x** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_determinant_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: determinant(x: float3x4 implicit) : float

// stub



:Arguments: * **x** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c__lb__rb__I_eq_H_ls_math_c__c_float3x4_gr__Ci_C_c_C_l:

.. das:function:: float3x4 implicit ==const[](m: float3x4 implicit ==const; i: int) : float3&

// stub



:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : int

.. _function-_at_math_c__c__lb__rb__CI_eq_H_ls_math_c__c_float3x4_gr__Ci_C_c_C_l:

.. das:function:: float3x4 const implicit ==const[](m: float3x4 const implicit ==const; i: int) : float3 const&

// stub



:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : int

.. _function-_at_math_c__c__lb__rb__I_eq_H_ls_math_c__c_float3x4_gr__Cu_C_c_C_l:

.. das:function:: float3x4 implicit ==const[](m: float3x4 implicit ==const; i: uint) : float3&

// stub



:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : uint

.. _function-_at_math_c__c__lb__rb__CI_eq_H_ls_math_c__c_float3x4_gr__Cu_C_c_C_l:

.. das:function:: float3x4 const implicit ==const[](m: float3x4 const implicit ==const; i: uint) : float3 const&

// stub



:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit!

            * **i** : uint

.. _function-_at_math_c__c_quat_from_euler_Cf3:

.. das:function:: quat_from_euler(angles: float3) : float4

// stub



:Arguments: * **angles** : float3

.. _function-_at_math_c__c_quat_from_euler_Cf_Cf_Cf:

.. das:function:: quat_from_euler(x: float; y: float; z: float) : float4

// stub



:Arguments: * **x** : float

            * **y** : float

            * **z** : float

.. _function-_at_math_c__c_euler_from_quat_Cf4:

.. das:function:: euler_from_quat(angles: float4) : float3

// stub



:Arguments: * **angles** : float4

.. _function-_at_math_c__c_quat_CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: quat(m: float3x3 implicit) : float4

// stub



:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-_at_math_c__c_quat_CIH_ls_math_c__c_float3x4_gr_:

.. das:function:: quat(m: float3x4 implicit) : float4

// stub



:Arguments: * **m** :  :ref:`float3x4 <handle-math-float3x4>`  implicit

.. _function-_at_math_c__c_quat_CIH_ls_math_c__c_float4x4_gr_:

.. das:function:: quat(m: float4x4 implicit) : float4

// stub



:Arguments: * **m** :  :ref:`float4x4 <handle-math-float4x4>`  implicit

.. _function-_at_math_c__c_quat_slerp_Cf_Cf4_Cf4:

.. das:function:: quat_slerp(t: float; a: float4; b: float4) : float4

// stub



:Arguments: * **t** : float

            * **a** : float4

            * **b** : float4

.. _function-_at_math_c__c_determinant_CIH_ls_math_c__c_float3x3_gr_:

.. das:function:: determinant(x: float3x3 implicit) : float

// stub



:Arguments: * **x** :  :ref:`float3x3 <handle-math-float3x3>`  implicit

.. _function-_at_math_c__c__lb__rb__I_eq_H_ls_math_c__c_float3x3_gr__Ci_C_c_C_l:

.. das:function:: float3x3 implicit ==const[](m: float3x3 implicit ==const; i: int) : float3&

// stub



:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : int

.. _function-_at_math_c__c__lb__rb__CI_eq_H_ls_math_c__c_float3x3_gr__Ci_C_c_C_l:

.. das:function:: float3x3 const implicit ==const[](m: float3x3 const implicit ==const; i: int) : float3 const&

// stub



:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : int

.. _function-_at_math_c__c__lb__rb__I_eq_H_ls_math_c__c_float3x3_gr__Cu_C_c_C_l:

.. das:function:: float3x3 implicit ==const[](m: float3x3 implicit ==const; i: uint) : float3&

// stub



:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : uint

.. _function-_at_math_c__c__lb__rb__CI_eq_H_ls_math_c__c_float3x3_gr__Cu_C_c_C_l:

.. das:function:: float3x3 const implicit ==const[](m: float3x3 const implicit ==const; i: uint) : float3 const&

// stub



:Arguments: * **m** :  :ref:`float3x3 <handle-math-float3x3>`  implicit!

            * **i** : uint


