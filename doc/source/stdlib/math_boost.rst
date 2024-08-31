
.. _stdlib_math_boost:

======================
Boost package for math
======================

The math boost module implements collection of helper macros and functions to accompany :ref:`math <stdlib_math>`.

All functions and symbols are in "math_boost" module, use require to get access to it. ::

    require daslib/math_boost


++++++++++
Structures
++++++++++

.. _struct-math_boost-AABR:

.. das:attribute:: AABR

axis aligned bounding rectangle

:Fields: * **min** : float2

         * **max** : float2

axis aligned bounding rectangle

.. _struct-math_boost-AABB:

.. das:attribute:: AABB

axis aligned bounding box

:Fields: * **min** : float3

         * **max** : float3

axis aligned bounding box

.. _struct-math_boost-Ray:

.. das:attribute:: Ray

ray (direction and origin)

:Fields: * **dir** : float3

         * **origin** : float3

ray (direction and origin)

+++++++++++++++++
Angle conversions
+++++++++++++++++

  *  :ref:`degrees (f: float) : float <function-_at_math_boost_c__c_degrees_Cf>` 
  *  :ref:`radians (f: float) : float <function-_at_math_boost_c__c_radians_Cf>` 

.. _function-_at_math_boost_c__c_degrees_Cf:

.. das:function:: degrees(f: float) : float

convert radians to degrees


:Arguments: * **f** : float

.. _function-_at_math_boost_c__c_radians_Cf:

.. das:function:: radians(f: float) : float

convert degrees to radians


:Arguments: * **f** : float

+++++++++++++
Intersections
+++++++++++++

  *  :ref:`is_intersecting (a: AABR; b: AABR) : bool <function-_at_math_boost_c__c_is_intersecting_CS_ls_math_boost_c__c_AABR_gr__CS_ls_math_boost_c__c_AABR_gr_>` 
  *  :ref:`is_intersecting (a: AABB; b: AABB) : bool <function-_at_math_boost_c__c_is_intersecting_CS_ls_math_boost_c__c_AABB_gr__CS_ls_math_boost_c__c_AABB_gr_>` 
  *  :ref:`is_intersecting (ray: Ray; aabb: AABB; Tmin: float = 0f; Tmax: float = FLT_MAX) : bool <function-_at_math_boost_c__c_is_intersecting_CS_ls_math_boost_c__c_Ray_gr__CS_ls_math_boost_c__c_AABB_gr__Cf_Cf>` 

.. _function-_at_math_boost_c__c_is_intersecting_CS_ls_math_boost_c__c_AABR_gr__CS_ls_math_boost_c__c_AABR_gr_:

.. das:function:: is_intersecting(a: AABR; b: AABR) : bool

returns true if inputs intersect


:Arguments: * **a** :  :ref:`AABR <struct-math_boost-AABR>` 

            * **b** :  :ref:`AABR <struct-math_boost-AABR>` 

.. _function-_at_math_boost_c__c_is_intersecting_CS_ls_math_boost_c__c_AABB_gr__CS_ls_math_boost_c__c_AABB_gr_:

.. das:function:: is_intersecting(a: AABB; b: AABB) : bool

|detail/function-math_boost-is_intersecting-0x1c857741a57acbb2|

:Arguments: * **a** :  :ref:`AABB <struct-math_boost-AABB>` 

            * **b** :  :ref:`AABB <struct-math_boost-AABB>` 

.. _function-_at_math_boost_c__c_is_intersecting_CS_ls_math_boost_c__c_Ray_gr__CS_ls_math_boost_c__c_AABB_gr__Cf_Cf:

.. das:function:: is_intersecting(ray: Ray; aabb: AABB; Tmin: float = 0f; Tmax: float = FLT_MAX) : bool

|detail/function-math_boost-is_intersecting-0x9e58f0ad7bc52d40|

:Arguments: * **ray** :  :ref:`Ray <struct-math_boost-Ray>` 

            * **aabb** :  :ref:`AABB <struct-math_boost-AABB>` 

            * **Tmin** : float

            * **Tmax** : float

++++++++
Matrices
++++++++

  *  :ref:`look_at_lh (Eye: float3; At: float3; Up: float3) : float4x4 <function-_at_math_boost_c__c_look_at_lh_Cf3_Cf3_Cf3>` 
  *  :ref:`look_at_rh (Eye: float3; At: float3; Up: float3) : float4x4 <function-_at_math_boost_c__c_look_at_rh_Cf3_Cf3_Cf3>` 
  *  :ref:`perspective_lh (fovy: float; aspect: float; zn: float; zf: float) : float4x4 <function-_at_math_boost_c__c_perspective_lh_Cf_Cf_Cf_Cf>` 
  *  :ref:`perspective_rh (fovy: float; aspect: float; zn: float; zf: float) : float4x4 <function-_at_math_boost_c__c_perspective_rh_Cf_Cf_Cf_Cf>` 
  *  :ref:`perspective_rh_opengl (fovy: float; aspect: float; zn: float; zf: float) : float4x4 <function-_at_math_boost_c__c_perspective_rh_opengl_Cf_Cf_Cf_Cf>` 
  *  :ref:`ortho_rh (left: float; right: float; bottom: float; top: float; zNear: float; zFar: float) : float4x4 <function-_at_math_boost_c__c_ortho_rh_Cf_Cf_Cf_Cf_Cf_Cf>` 
  *  :ref:`planar_shadow (Light: float4; Plane: float4) : float4x4 <function-_at_math_boost_c__c_planar_shadow_Cf4_Cf4>` 

.. _function-_at_math_boost_c__c_look_at_lh_Cf3_Cf3_Cf3:

.. das:function:: look_at_lh(Eye: float3; At: float3; Up: float3) : float4x4

left-handed (z forward) look at matrix with origin at `Eye` and target at `At`, and up vector `Up`.


:Arguments: * **Eye** : float3

            * **At** : float3

            * **Up** : float3

.. _function-_at_math_boost_c__c_look_at_rh_Cf3_Cf3_Cf3:

.. das:function:: look_at_rh(Eye: float3; At: float3; Up: float3) : float4x4

right-handed (z towards viewer) look at matrix with origin at `Eye` and target at `At`, and up vector `Up`.


:Arguments: * **Eye** : float3

            * **At** : float3

            * **Up** : float3

.. _function-_at_math_boost_c__c_perspective_lh_Cf_Cf_Cf_Cf:

.. das:function:: perspective_lh(fovy: float; aspect: float; zn: float; zf: float) : float4x4

left-handed (z forward) perspective matrix


:Arguments: * **fovy** : float

            * **aspect** : float

            * **zn** : float

            * **zf** : float

.. _function-_at_math_boost_c__c_perspective_rh_Cf_Cf_Cf_Cf:

.. das:function:: perspective_rh(fovy: float; aspect: float; zn: float; zf: float) : float4x4

right-handed (z toward viewer) perspective matrix


:Arguments: * **fovy** : float

            * **aspect** : float

            * **zn** : float

            * **zf** : float

.. _function-_at_math_boost_c__c_perspective_rh_opengl_Cf_Cf_Cf_Cf:

.. das:function:: perspective_rh_opengl(fovy: float; aspect: float; zn: float; zf: float) : float4x4

right-handed (z toward viewer) opengl (z in [-1..1]) perspective matrix


:Arguments: * **fovy** : float

            * **aspect** : float

            * **zn** : float

            * **zf** : float

.. _function-_at_math_boost_c__c_ortho_rh_Cf_Cf_Cf_Cf_Cf_Cf:

.. das:function:: ortho_rh(left: float; right: float; bottom: float; top: float; zNear: float; zFar: float) : float4x4

right handed (z towards viwer) orthographic (parallel) projection matrix


:Arguments: * **left** : float

            * **right** : float

            * **bottom** : float

            * **top** : float

            * **zNear** : float

            * **zFar** : float

.. _function-_at_math_boost_c__c_planar_shadow_Cf4_Cf4:

.. das:function:: planar_shadow(Light: float4; Plane: float4) : float4x4

planar shadow projection matrix, i.e. all light shadows to be projected on a plane


:Arguments: * **Light** : float4

            * **Plane** : float4

+++++
Plane
+++++

  *  :ref:`plane_dot (Plane: float4; Vec: float4) : float <function-_at_math_boost_c__c_plane_dot_Cf4_Cf4>` 
  *  :ref:`plane_normalize (Plane: float4) : float4 <function-_at_math_boost_c__c_plane_normalize_Cf4>` 
  *  :ref:`plane_from_point_normal (p: float3; n: float3) : float4 <function-_at_math_boost_c__c_plane_from_point_normal_Cf3_Cf3>` 

.. _function-_at_math_boost_c__c_plane_dot_Cf4_Cf4:

.. das:function:: plane_dot(Plane: float4; Vec: float4) : float

dot product of `Plane` and 'Vec'


:Arguments: * **Plane** : float4

            * **Vec** : float4

.. _function-_at_math_boost_c__c_plane_normalize_Cf4:

.. das:function:: plane_normalize(Plane: float4) : float4

normalize `Plane', length xyz will be 1.0 (or 0.0 for no plane)


:Arguments: * **Plane** : float4

.. _function-_at_math_boost_c__c_plane_from_point_normal_Cf3_Cf3:

.. das:function:: plane_from_point_normal(p: float3; n: float3) : float4

construct plane from point `p` and normal `n`


:Arguments: * **p** : float3

            * **n** : float3

++++++++++++++++++++++++++
Color packig and unpacking
++++++++++++++++++++++++++

  *  :ref:`RGBA_TO_UCOLOR (x: float; y: float; z: float; w: float) : uint <function-_at_math_boost_c__c_RGBA_TO_UCOLOR_Cf_Cf_Cf_Cf>` 
  *  :ref:`RGBA_TO_UCOLOR (xyzw: float4) : uint <function-_at_math_boost_c__c_RGBA_TO_UCOLOR_Cf4>` 
  *  :ref:`UCOLOR_TO_RGBA (x: uint) : float4 <function-_at_math_boost_c__c_UCOLOR_TO_RGBA_Cu>` 
  *  :ref:`UCOLOR_TO_RGB (x: uint) : float3 <function-_at_math_boost_c__c_UCOLOR_TO_RGB_Cu>` 

.. _function-_at_math_boost_c__c_RGBA_TO_UCOLOR_Cf_Cf_Cf_Cf:

.. das:function:: RGBA_TO_UCOLOR(x: float; y: float; z: float; w: float) : uint

conversion from RGBA to ucolor. x,y,z,w are in [0,1] range


:Arguments: * **x** : float

            * **y** : float

            * **z** : float

            * **w** : float

.. _function-_at_math_boost_c__c_RGBA_TO_UCOLOR_Cf4:

.. das:function:: RGBA_TO_UCOLOR(xyzw: float4) : uint

|detail/function-math_boost-RGBA_TO_UCOLOR-0xfea34bf3d8bd7d71|

:Arguments: * **xyzw** : float4

.. _function-_at_math_boost_c__c_UCOLOR_TO_RGBA_Cu:

.. das:function:: UCOLOR_TO_RGBA(x: uint) : float4

conversion from ucolor to RGBA. x components are in [0,255] range


:Arguments: * **x** : uint

.. _function-_at_math_boost_c__c_UCOLOR_TO_RGB_Cu:

.. das:function:: UCOLOR_TO_RGB(x: uint) : float3

conversion from ucolor to RGB. x components are in [0,255] range. result is float3(x,y,z)


:Arguments: * **x** : uint

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_math_boost_c__c_linear_to_SRGB_Cf:

.. das:function:: linear_to_SRGB(x: float) : float

convert value from linear space to sRGB curve space


:Arguments: * **x** : float

.. _function-_at_math_boost_c__c_linear_to_SRGB_Cf3:

.. das:function:: linear_to_SRGB(c: float3) : float3

|detail/function-math_boost-linear_to_SRGB-0x74ddd213dc94b6af|

:Arguments: * **c** : float3

.. _function-_at_math_boost_c__c_linear_to_SRGB_Cf4:

.. das:function:: linear_to_SRGB(c: float4) : float4

|detail/function-math_boost-linear_to_SRGB-0x74e2d213dc9d35af|

:Arguments: * **c** : float4


