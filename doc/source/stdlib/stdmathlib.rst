.. _stdlib_stdmathlib:

================
The Math library
================

Floating point math in general is not bit-precise. Compiler can optimize permutations, replace divisions with multiplications, and some of functions are not bit-exact.
If you need precise math use double precision type.
All functions and symbols are in "math" module, use require to get access to it. ::

    require math

+++++++++++++++
Common Symbols
+++++++++++++++

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
all numerics (uint*, int*, float*, double)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. das:function:: min(x, y)

    returns the minimum of `x` and `y`

.. das:function:: max(x, y)

    returns the minimum of `x` and `y`

^^^^^^^^^^^^^^^^^^^^^^^^
float* and double
^^^^^^^^^^^^^^^^^^^^^^^^

.. das:function:: abs(x)

    returns the absolute value of `x` as same time

.. das:function:: acos(x)

    returns the arccosine of `x`

.. das:function:: asin(x)

    returns the arcsine of `x`

.. das:function:: atan(x)

    returns the arctangent of `x`

.. das:function:: atan2(x,y)

    returns the arctangent of  `x/y`

.. das:function:: cos(x)

    returns the cosine of `x`

.. das:function:: sin(x)

    returns the sine of `x`

.. das:function:: tan(x)

    returns the tangent of `x`

.. das:function:: exp(x)

    returns the exponential value of the float parameter `x`

.. das:function:: log(x)

    returns the natural logarithm of `x`

.. das:function:: exp2(x)

    returns the 2^x value of the float parameter `x`

.. das:function:: log2(x)

    returns the logarithm base-2 of `x`

.. das:function:: pow(x,y)

    returns `x` raised to the power of `y`

.. das:function:: sqrt(x)

    returns the square root of `x`

.. das:function:: rcp(x)

    returns the 1/x of `x`

.. das:data:: PI

    The numeric constant pi (3.141592) is the ratio of the circumference of a circle to its diameter

.. das:function:: ceil(x)

    returns a float value representing the smallest integer (type is still float) that is greater than or equal to `x`

.. das:function:: floor(x)

    returns a float value representing the largest integer that is less than or equal to `x`

.. das:function:: abs(x)

    returns a positive value representing with same module as `x`

.. das:function:: saturate(x)

    returns a clamped to [0..1] inclusive range `x`

^^^^^^^^^^^^^^^^^^^^^^^^
float* only
^^^^^^^^^^^^^^^^^^^^^^^^
.. das:function:: atan2_est(x,y)

    returns the faster approximation of arctangent of  `x/y` - float only

.. das:function:: rcp_est(x)

    returns the fast approximation 1/x of `x` - float only

.. das:function:: ceili(x)

    returns a value representing the smallest integer (integer type!) that is greater than or equal to `x`

.. das:function:: floori(x)

    returns a integer value representing the largest integer that is less than or equal to `x`

.. das:function:: roundi(x)

    returns a integer value representing the integer that is closest to `x`

.. das:function:: trunci(x)

    returns a integer value representing the float without fraction part of `x`


+++++++++++++++++
Noise functions
+++++++++++++++++

.. das:function:: uint32_hash(x:uint)

    returns hashed value of `x`

.. das:function:: uint_noise_1D(position: int; seed:uint)

    returns noise value of `position` in `seed` sequence

.. das:function:: uint_noise_2D(x,y: int; seed:uint)

    returns noise value of `x,y` position in `seed` sequence

.. das:function:: uint_noise_3D(x,y,z: int; seed:uint)

    returns noise value of `x,y,z` position in `seed` sequence


+++++++++++++++++
Vector functions
+++++++++++++++++

^^^^^^^^^^^^^^^^^^^^^^^^
float2, float3, float4:
^^^^^^^^^^^^^^^^^^^^^^^^

.. das:function:: length(x)

    returns a non-negative value representing magnitude of `x`

.. das:function:: length_sq(x)

    returns a non-negative value representing squared magnitude of `x`

.. das:function:: inv_length(x)

    returns a non-negative value representing 1/magnitude of `x`

.. das:function:: inv_length_sq(x)

    returns a non-negative value representing 1/squared magnitude of `x`

.. das:function:: distance(x, y)

    returns a non-negative value representing distance between `x` and `y`

.. das:function:: distance_sq(x)

    returns a non-negative value representing squared distance between `x` and `y`

.. das:function:: inv_distance(x, y)

    returns a non-negative value representing 1/distance between `x` and `y`

.. das:function:: inv_distance_sq(x)

    returns a non-negative value representing 1/squared distance between `x` and `y`

.. das:function:: dot(x, y)

    returns scalar representating dot product between `x` and `y`

.. das:function:: normalize(x)

    returns normalized `x`, or nan if length(x) is 0
	
.. das:function:: safe_normalize(x)

    returns normalized `x`, or 0 if length(x) is 0

^^^^^^^^^^^^
float3 only:
^^^^^^^^^^^^

.. das:function:: cross(x, y)

    returns vector representating cross product between `x` and `y`
.. das:function:: reflect(v, n)

    returns vector representating reflection of `x` from `n` same as ::

        def reflect(v, n: float3)
            return v - 2. * dot(v, n) * n

.. das:function:: refract(v, n)

    returns vector representating reflection of `x` from `n` same as ::

        def refract(v, n: float3; nint: float; outRefracted: float3&)
            let dt = dot(v, n)
            let discr = 1. - nint * nint * (1. - dt * dt)
            if discr > 0.
                outRefracted = nint * (v - n * dt) - n * sqrt(discr)
                return true
            return false

+++++++++++++++++
lerp/madd/clamp
+++++++++++++++++

.. das:function:: lerp(a, b, t)

    returns vector or scalar representating `a` + (`b` - `a`) * `t`

.. das:function:: madd(a, b, c)

    returns vector or scalar representating `a` * `b` + `c`

.. das:function:: clamp(t, a, b)

    returns vector or scalar representating min(max(`t`, `a`), `b`)
