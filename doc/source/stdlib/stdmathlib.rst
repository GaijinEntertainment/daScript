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

.. js:function:: abs(x)

    returns the absolute value of `x` as same time

.. js:function:: acos(x)

    returns the arccosine of `x`

.. js:function:: asin(x)

    returns the arcsine of `x`

.. js:function:: atan(x)

    returns the arctangent of `x`

.. js:function:: atan2(x,y)

    returns the arctangent of  `x/y`

.. js:function:: cos(x)

    returns the cosine of `x`

.. js:function:: sin(x)

    returns the sine of `x`

.. js:function:: tan(x)

    returns the tangent of `x`

.. js:function:: exp(x)

    returns the exponential value of the float parameter `x`

.. js:function:: log(x)

    returns the natural logarithm of `x`

.. js:function:: exp2(x)

    returns the 2^x value of the float parameter `x`

.. js:function:: log2(x)

    returns the logarithm base-2 of `x`

.. js:function:: pow(x,y)

    returns `x` raised to the power of `y`

.. js:function:: sqrt(x)

    returns the square root of `x`

.. js:function:: rcp(x)

    returns the 1/x of `x`

.. js:data:: PI

    The numeric constant pi (3.141592) is the ratio of the circumference of a circle to its diameter

.. js:function:: ceil(x)

    returns a value representing the smallest integer (type is still float) that is greater than or equal to `x`

.. js:function:: floor(x)

    returns a float value representing the largest integer that is less than or equal to `x`

.. js:function:: abs(x)

    returns a positive value representing with same module as `x`

.. js:function:: saturate(x)

    returns a clamped to [0..1] inclusive range `x`

.. js:function:: atan2_est(x,y)

    returns the faster approximation of arctangent of  `x/y` - float only

.. js:function:: rcp_est(x)

    returns the fast approximation 1/x of `x` - float only


+++++++++++++++++
Noise functions
+++++++++++++++++

.. js:function:: uint32_hash(x:uint)

    returns hashed value of `x`

.. js:function:: uint_noise1D(position: int; seed:uint)

    returns noise value of `position` in `seed` sequence

.. js:function:: uint_noise2D(x,y: int; seed:uint)

    returns noise value of `x,y` position in `seed` sequence

.. js:function:: uint_noise3D(x,y,z: int; seed:uint)

    returns noise value of `x,y,z` position in `seed` sequence


+++++++++++++++++
Vector functions
+++++++++++++++++

float2, float3, float4:

length - magnitude
lengthSq - squared magnitude
invLength - inverse magnitude
invLengthSq - inverse squared magnitude
distance - distance between arguments
distanceSq - squared distance between arguments
invDistance - inverse distance between arguments
invDistanceSq - inverse squared distance between arguments
dot - dot product between arguments
normalize - returns normalized argument

float3 only:
cross - cross product (only float3).
reflect - reflection, same as :: 

    def reflect(v,n:float3)
        return v - 2. * dot(v, n) * n

refract - refraction, same as ::
    
    def refract(v,n:float3;nint:float;outRefracted:float3&)
        let dt = dot(v,n)
        let discr = 1. - nint*nint*(1.-dt*dt)
        if discr > 0.
            outRefracted = nint*(v-n*dt)-n*sqrt(discr)
            return true
        return false

+++++++++++++++++
lerp/madd/clamp
+++++++++++++++++

lerp(t, a, b)   a + (b-a)*t
madd(a, b, c)   a*b + c 
clamp(t, a, b)  min(max(t,a), b)