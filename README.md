# vecmath
SIMD HAL header-only library by Gaijin Entertainment.
Currently SSE and NEON only versions. (PS4 and X360 are not shared due to NDA, and they are outdated anyway).

The main purpose of this library is to work with SIMD vectorized code more or less optimal way with simple function like (no operators) syntax.

Has planes, quaternions, 3 different matrices, vectors, bbox types, and simple wrappers over SSE/NEON intrinsics.

In addition: pow, exp, log, sincos, frustum culling, and other rather complicated functions.

It is tested on MSVC 2015, MSVC 2017, clang 3.9 and higher, gcc several versions, Nintendo Switch, tvOS, iOS, Android, PS4, XBox One, PC, Linux, Mac.

It used to work on MSVC2008/2010, however, recent version hadn't been tested on these compilers.
