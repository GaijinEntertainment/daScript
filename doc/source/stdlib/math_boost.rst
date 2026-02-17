
.. _stdlib_math_boost:

======================
Boost package for math
======================

.. das:module:: math_boost

The MATH_BOOST module adds geometric types (``AABB``, ``AABR``, ``Ray``),
angle conversion (``degrees``, ``radians``), intersection tests, color space
conversion (``linear_to_SRGB``, ``RGBA_TO_UCOLOR``), and view/projection
matrix construction (``look_at_lh``, ``perspective_rh``).

All functions and symbols are in "math_boost" module, use require to get access to it. ::

    require daslib/math_boost

Example: ::

    require daslib/math_boost

        [export]
        def main() {
            print("degrees(PI) = {degrees(PI)}\n")
            print("radians(180) = {radians(180.0)}\n")
            var box = AABB(min = float3(0), max = float3(10))
            print("box = ({box.min}) - ({box.max})\n")
        }
        // output:
        // degrees(PI) = 180
        // radians(180) = 3.1415927
        // box = (0,0,0) - (10,10,10)



++++++++++
Structures
++++++++++

.. _struct-math_boost-AABR:

.. das:attribute:: AABR

axis aligned bounding rectangle

:Fields: * **min** : float2 - min coordinates

         * **max** : float2 - max coordinates



.. _struct-math_boost-AABB:

.. das:attribute:: AABB

axis aligned bounding box

:Fields: * **min** : float3 - min coordinates

         * **max** : float3 - max coordinates



.. _struct-math_boost-Ray:

.. das:attribute:: Ray

ray (direction and origin)

:Fields: * **dir** : float3 - direction

         * **origin** : float3 - origin




+++++++++++++++++
Angle conversions
+++++++++++++++++

  *  :ref:`degrees (f: float) : float <function-math_boost_degrees_float>`
  *  :ref:`radians (f: float) : float <function-math_boost_radians_float>`

.. _function-math_boost_degrees_float:

.. das:function:: degrees(f: float) : float

convert radians to degrees


:Arguments: * **f** : float

.. _function-math_boost_radians_float:

.. das:function:: radians(f: float) : float

convert degrees to radians


:Arguments: * **f** : float


+++++++++++++
Intersections
+++++++++++++

  *  :ref:`is_intersecting (a: AABB; b: AABB) : bool <function-math_boost_is_intersecting_AABB_AABB>`
  *  :ref:`is_intersecting (a: AABR; b: AABR) : bool <function-math_boost_is_intersecting_AABR_AABR>`
  *  :ref:`is_intersecting (ray: Ray; aabb: AABB; Tmin: float = 0f; Tmax: float = FLT_MAX) : bool <function-math_boost_is_intersecting_Ray_AABB_float_float>`


is_intersecting
^^^^^^^^^^^^^^^

.. _function-math_boost_is_intersecting_AABB_AABB:

.. das:function:: is_intersecting(a: AABB; b: AABB) : bool

returns true if inputs intersect


:Arguments: * **a** :  :ref:`AABB <struct-math_boost-AABB>`

            * **b** :  :ref:`AABB <struct-math_boost-AABB>`

.. _function-math_boost_is_intersecting_AABR_AABR:

.. das:function:: is_intersecting(a: AABR; b: AABR) : bool

.. _function-math_boost_is_intersecting_Ray_AABB_float_float:

.. das:function:: is_intersecting(ray: Ray; aabb: AABB; Tmin: float = 0f; Tmax: float = FLT_MAX) : bool


++++++++
Matrices
++++++++

  *  :ref:`look_at_lh (Eye: float3; At: float3; Up: float3) : float4x4 <function-math_boost_look_at_lh_float3_float3_float3>`
  *  :ref:`look_at_rh (Eye: float3; At: float3; Up: float3) : float4x4 <function-math_boost_look_at_rh_float3_float3_float3>`
  *  :ref:`ortho_rh (left: float; right: float; bottom: float; top: float; zNear: float; zFar: float) : float4x4 <function-math_boost_ortho_rh_float_float_float_float_float_float>`
  *  :ref:`perspective_lh (fovy: float; aspect: float; zn: float; zf: float) : float4x4 <function-math_boost_perspective_lh_float_float_float_float>`
  *  :ref:`perspective_rh (fovy: float; aspect: float; zn: float; zf: float) : float4x4 <function-math_boost_perspective_rh_float_float_float_float>`
  *  :ref:`perspective_rh_opengl (fovy: float; aspect: float; zn: float; zf: float) : float4x4 <function-math_boost_perspective_rh_opengl_float_float_float_float>`
  *  :ref:`planar_shadow (Light: float4; Plane: float4) : float4x4 <function-math_boost_planar_shadow_float4_float4>`

.. _function-math_boost_look_at_lh_float3_float3_float3:

.. das:function:: look_at_lh(Eye: float3; At: float3; Up: float3) : float4x4

left-handed (z forward) look at matrix with origin at `Eye` and target at `At`, and up vector `Up`.


:Arguments: * **Eye** : float3

            * **At** : float3

            * **Up** : float3

.. _function-math_boost_look_at_rh_float3_float3_float3:

.. das:function:: look_at_rh(Eye: float3; At: float3; Up: float3) : float4x4

right-handed (z towards viewer) look at matrix with origin at `Eye` and target at `At`, and up vector `Up`.


:Arguments: * **Eye** : float3

            * **At** : float3

            * **Up** : float3

.. _function-math_boost_ortho_rh_float_float_float_float_float_float:

.. das:function:: ortho_rh(left: float; right: float; bottom: float; top: float; zNear: float; zFar: float) : float4x4

right handed (z towards viewer) orthographic (parallel) projection matrix


:Arguments: * **left** : float

            * **right** : float

            * **bottom** : float

            * **top** : float

            * **zNear** : float

            * **zFar** : float

.. _function-math_boost_perspective_lh_float_float_float_float:

.. das:function:: perspective_lh(fovy: float; aspect: float; zn: float; zf: float) : float4x4

left-handed (z forward) perspective matrix


:Arguments: * **fovy** : float

            * **aspect** : float

            * **zn** : float

            * **zf** : float

.. _function-math_boost_perspective_rh_float_float_float_float:

.. das:function:: perspective_rh(fovy: float; aspect: float; zn: float; zf: float) : float4x4

right-handed (z toward viewer) perspective matrix


:Arguments: * **fovy** : float

            * **aspect** : float

            * **zn** : float

            * **zf** : float

.. _function-math_boost_perspective_rh_opengl_float_float_float_float:

.. das:function:: perspective_rh_opengl(fovy: float; aspect: float; zn: float; zf: float) : float4x4

right-handed (z toward viewer) opengl (z in [-1..1]) perspective matrix


:Arguments: * **fovy** : float

            * **aspect** : float

            * **zn** : float

            * **zf** : float

.. _function-math_boost_planar_shadow_float4_float4:

.. das:function:: planar_shadow(Light: float4; Plane: float4) : float4x4

planar shadow projection matrix, i.e. all light shadows to be projected on a plane


:Arguments: * **Light** : float4

            * **Plane** : float4


+++++
Plane
+++++

  *  :ref:`plane_dot (Plane: float4; Vec: float4) : float <function-math_boost_plane_dot_float4_float4>`
  *  :ref:`plane_from_point_normal (p: float3; n: float3) : float4 <function-math_boost_plane_from_point_normal_float3_float3>`
  *  :ref:`plane_normalize (Plane: float4) : float4 <function-math_boost_plane_normalize_float4>`

.. _function-math_boost_plane_dot_float4_float4:

.. das:function:: plane_dot(Plane: float4; Vec: float4) : float

dot product of `Plane` and 'Vec'


:Arguments: * **Plane** : float4

            * **Vec** : float4

.. _function-math_boost_plane_from_point_normal_float3_float3:

.. das:function:: plane_from_point_normal(p: float3; n: float3) : float4

construct plane from point `p` and normal `n`


:Arguments: * **p** : float3

            * **n** : float3

.. _function-math_boost_plane_normalize_float4:

.. das:function:: plane_normalize(Plane: float4) : float4

normalize ``Plane``, length xyz will be 1.0 (or 0.0 for no plane)


:Arguments: * **Plane** : float4


+++++++++++++++++
Color conversions
+++++++++++++++++

  *  :ref:`linear_to_SRGB (c: float3) : float3 <function-math_boost_linear_to_SRGB_float3>`
  *  :ref:`linear_to_SRGB (x: float) : float <function-math_boost_linear_to_SRGB_float>`
  *  :ref:`linear_to_SRGB (c: float4) : float4 <function-math_boost_linear_to_SRGB_float4>`


linear_to_SRGB
^^^^^^^^^^^^^^

.. _function-math_boost_linear_to_SRGB_float3:

.. das:function:: linear_to_SRGB(c: float3) : float3

convert value from linear space to sRGB curve space


:Arguments: * **c** : float3

.. _function-math_boost_linear_to_SRGB_float:

.. das:function:: linear_to_SRGB(x: float) : float

.. _function-math_boost_linear_to_SRGB_float4:

.. das:function:: linear_to_SRGB(c: float4) : float4


+++++++++++++++++++++++++++
Color packing and unpacking
+++++++++++++++++++++++++++

  *  :ref:`RGBA_TO_UCOLOR (xyzw: float4) : uint <function-math_boost_RGBA_TO_UCOLOR_float4>`
  *  :ref:`RGBA_TO_UCOLOR (x: float; y: float; z: float; w: float) : uint <function-math_boost_RGBA_TO_UCOLOR_float_float_float_float>`
  *  :ref:`UCOLOR_TO_RGB (x: uint) : float3 <function-math_boost_UCOLOR_TO_RGB_uint>`
  *  :ref:`UCOLOR_TO_RGBA (x: uint) : float4 <function-math_boost_UCOLOR_TO_RGBA_uint>`


RGBA_TO_UCOLOR
^^^^^^^^^^^^^^

.. _function-math_boost_RGBA_TO_UCOLOR_float4:

.. das:function:: RGBA_TO_UCOLOR(xyzw: float4) : uint

conversion from RGBA to ucolor. xyzw components are in [0,1] range


:Arguments: * **xyzw** : float4

.. _function-math_boost_RGBA_TO_UCOLOR_float_float_float_float:

.. das:function:: RGBA_TO_UCOLOR(x: float; y: float; z: float; w: float) : uint

----

.. _function-math_boost_UCOLOR_TO_RGB_uint:

.. das:function:: UCOLOR_TO_RGB(x: uint) : float3

conversion from ucolor to RGB. x components are in [0,255] range. result is float3(x,y,z)


:Arguments: * **x** : uint

.. _function-math_boost_UCOLOR_TO_RGBA_uint:

.. das:function:: UCOLOR_TO_RGBA(x: uint) : float4

conversion from ucolor to RGBA. x components are in [0,255] range


:Arguments: * **x** : uint


