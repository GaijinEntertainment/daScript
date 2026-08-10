The MATH_BOOST module adds geometric types (``AABB``, ``AABR``, ``Ray``),
intersection tests (``is_intersecting``), plane helpers (``plane_dot``,
``plane_normalize``, ``plane_from_point_normal``, ``planar_shadow``), color
space conversion (``linear_to_SRGB``, ``RGBA_TO_UCOLOR``, ``UCOLOR_TO_RGBA``),
and view/projection matrix construction (``look_at_lh``, ``look_at_rh``,
``perspective_rh``, ``ortho_rh``). It requires ``math`` publicly, so requiring
``math_boost`` also brings in the whole scalar/vector math surface
(``degrees``, ``radians``, ``sin``, ``length``, ...).

All functions and symbols are in "math_boost" module, use require to get access to it.

.. code-block:: das

    require daslib/math_boost

Example:

.. code-block:: das

    require daslib/math_boost

    [export]
    def main() {
        let box = AABB(min = float3(0), max = float3(10))
        let other = AABB(min = float3(5), max = float3(15))
        print("boxes intersect = {is_intersecting(box, other)}\n")
        let ray = Ray(origin = float3(-1, 5, 5), dir = float3(1, 0, 0))
        print("ray hits box = {is_intersecting(ray, box, 0.0, 100.0)}\n")
        print("linear_to_SRGB(0.5) = {linear_to_SRGB(0.5)}\n")
        print("RGBA_TO_UCOLOR(red) = {RGBA_TO_UCOLOR(1.0, 0.0, 0.0, 1.0)}\n")
    }
    // output:
    // boxes intersect = true
    // ray hits box = true
    // linear_to_SRGB(0.5) = 0.73535705
    // RGBA_TO_UCOLOR(red) = 0xff0000ff
