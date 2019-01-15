# vecmath
SIMD HAL header-only library

This is one-header-only (it is 4 headers, but you need to include one) Hardware Abstraction Layer over SIMD intrinsics.

Currently for SSE and NEON only, PS3/Xbox360 version is under NDA and not needed nowdays.

It has been tested on PC, Linux, MacOS, iOS, tvOS, Shield, NSwitch, PlayStation4, Xbox One.
And with gcc of several versions, VS 2015, VS 2017, clang 3.9+. Visual Studio of earlier versions used to work, but hasn't been tested in a while.

Current version relies on alignas c++11 modifier, although it is easy to re-write it old-way (__attribute__ / __declspec(aligned)) if needed.

It works with SSE2 set only, although there are some optimizations for SSSE3/SSE4.1, if they are mandatory (by architecture).

Basic types are - plane, point/vector (vec3f/vec4f, which is same 4-float words), bbox (2 words), mat33 (3 vec3f), mat43, mat44, sphere.

In addition to common HAL over simple arithmetics intrinsics, there are some complex functions such as exp,log,sincos, quaternion interpolation, and frustum visibility.
