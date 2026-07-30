.. _tutorial_drawlist:

#######################
Drawlist rail
#######################

ImGui exposes three viewport-level draw lists — window, foreground, background —
plus a family of low-level ``Add*`` primitives that paint directly to them
(``AddLine``, ``AddRect``, ``AddCircle``, etc.). The boost layer wraps them
behind three scope wrappers and a primitive set covering the geometric basics
(line / rect / circle / triangle / text) plus the v2 extras
(rect_filled_multi_color / ellipse / ngon / polyline / bezier). The
path-building (``path_*``), channel-splitting (``channels_*``), and
clip-rect rails are control-flow helpers, not registered ``[drawlist_prim]``
entries — they shape what the primitives render but do not themselves
create snapshot rows:

.. code-block:: das

   with_window_drawlist() $(var dl) {
       dl |> add_rect_filled(p0, p1, rgba(40u, 80u, 200u, 200u))
       dl |> add_line(a, b, rgba(255u, 255u, 0u, 255u), 2.0f)
   }

Every primitive registers a per-frame snapshot entry under a synthesized
``<window>/<mod>:<line>:<col>`` path key, so playwright / mouse-cards can
target a specific call site by path without the caller declaring a state
global. Drawlist primitives are decoration-only — no hover / active / focus
state is captured.

Source: ``examples/tutorial/drawlist.das``.

************
Walkthrough
************

.. video:: drawlist.mp4

The recording is a display-only tour - no gesture to drive - so the self-check
is on the *output*: it narrates each scope wrapper while asserting the registry
holds the painted primitives (``record_check_kind_count`` over the ``add_*``
kind family, since drawlist call sites register under synthesized path keys
rather than caller idents) plus the window / foreground-label widgets on screen.
A rail that stopped painting would abort the recording.

.. literalinclude:: ../../../examples/tutorial/drawlist.das
   :language: das
   :linenos:

Scope wrappers
==============

Three thin wrappers cover the three drawlist scopes:

* ``with_window_drawlist() $(var dl) { ... }`` — current window's drawlist.
  Painting is clipped to the window's content region and stacks under any
  subsequent widgets rendered in this window.
* ``with_foreground_drawlist() $(var dl) { ... }`` — top-most foreground
  drawlist. Paints over every window on the viewport. Use for HUD overlays,
  drag indicators, tooltips that must escape window clipping.
* ``with_background_drawlist() $(var dl) { ... }`` — bottom-most background
  drawlist. Paints under every window on the viewport. Use for full-viewport
  backdrops (gradients, vignettes, debug grids).

Each wrapper acquires the drawlist pointer, yields it to the block as ``dl``,
and exits. There's no push / pop pairing under the hood — drawlists are
viewport-level state that any ImGui code can reach via ``GetWindowDrawList``
/ ``GetForegroundDrawList`` / ``GetBackgroundDrawList``; the wrappers exist
to give the rail a uniform body shape (``with_X() $(var dl) { dl |> add_*(...) }``)
and to keep raw ``GetWindowDrawList`` calls off the boost surface.

Primitives
==========

Eight ``[drawlist_prim]``-tagged painters cover the geometric basics:

* ``add_line(dl, a, b, col, thickness = 1.0f)``
* ``add_rect(dl, a, b, col, rounding = 0.0f, flags = ImDrawFlags.None, thickness = 1.0f)``
* ``add_rect_filled(dl, a, b, col, rounding = 0.0f, flags = ImDrawFlags.None)``
* ``add_circle(dl, center, radius, col, num_segments = 0, thickness = 1.0f)``
* ``add_circle_filled(dl, center, radius, col, num_segments = 0)``
* ``add_triangle(dl, a, b, c, col, thickness = 1.0f)``
* ``add_triangle_filled(dl, a, b, c, col)``
* ``add_text(dl, pos, col, text)``

``col`` is a packed ABGR ``uint`` — use ``rgba(r, g, b, a)`` to build one,
or ``GetColorU32(ImGuiCol.*)`` for style-colour references. Positions are
screen-space pixels; widget-relative drawing typically anchors on
``GetCursorScreenPos`` / ``GetItemRectMin`` / ``GetItemRectMax``.

Shape extras (v2 additions)
===========================

Eight additional ``[drawlist_prim]`` painters round out the shape set:

* ``add_rect_filled_multi_color(dl, a, b, col_ul, col_ur, col_br, col_bl)`` —
  per-corner gradient (4 colours, bilinearly interpolated).
* ``add_ellipse(dl, center, radii, col, rot = 0.0f, num_segments = 0, thickness = 1.0f)``
  / ``add_ellipse_filled(dl, center, radii, col, rot = 0.0f, num_segments = 0)``
  — ``radii`` = ``(rx, ry)`` half-axes; ``rot`` rotates the ellipse about
  ``center`` (radians).
* ``add_ngon(dl, center, radius, col, num_segments, thickness = 1.0f)``
  / ``add_ngon_filled(dl, center, radius, col, num_segments)`` —
  regular N-gons; ``num_segments`` required (no adaptive default).
* ``add_polyline(dl, points : array<float2>, col, flags = ImDrawFlags.None, thickness = 1.0f)``
  — open or closed polyline through ``points`` (use
  ``ImDrawFlags.Closed`` to close).
* ``add_bezier_cubic(dl, p1, p2, p3, p4, col, thickness = 1.0f, num_segments = 0)``
  / ``add_bezier_quadratic(dl, p1, p2, p3, col, thickness = 1.0f, num_segments = 0)``
  — bezier curves through anchor + control points.

Path building
=============

For complex one-off shapes (rounded rects assembled from arcs, custom
polygons, etc.) the path family lets the caller build a vertex stack
incrementally then terminate with one of ``path_stroke`` /
``path_fill_convex`` / ``path_fill_concave``:

* ``path_clear(dl)`` — discard pending path vertices.
* ``path_line_to(dl, pos)`` — append a straight-line segment.
* ``path_arc_to(dl, center, radius, a_min, a_max, num_segments = 0)`` —
  append a circular arc.
* ``path_bezier_quadratic_curve_to(dl, p2, p3, num_segments = 0)`` —
  append a quadratic bezier curve from the path's current endpoint.
* ``path_fill_convex(dl, col)`` / ``path_fill_concave(dl, col)`` —
  close + fill (convex is faster; concave handles non-convex polygons).
* ``path_stroke(dl, col, flags = ImDrawFlags.None, thickness = 1.0f)`` —
  stroke the pending path (open by default). Pass
  ``ImDrawFlags.Closed`` to join the last vertex back to the first.

See :download:`examples/features/drawlist_path.das
<../../../examples/features/drawlist_path.das>` for a stroked rounded
rect, filled convex pentagon, and filled concave arrow built path-style.

Channel splitting
=================

ImGui drawlists are append-only — once a vertex lands, later draws sit
on top. ``channels_split`` defers ordering: split the drawlist into N
channels, submit each layer in convenient order, then ``channels_merge``
flattens them in channel-index order (channel 0 first, channel N-1 last).

* ``channels_split(dl, count)`` — push ``count`` channels.
* ``channels_set_current(dl, idx)`` — switch the active channel.
* ``channels_merge(dl)`` — flatten back to the main draw stream.

Typical use: draw a card's foreground content first to measure its
bounding box, then go back to channel 0 to paint the background card
behind it. See :download:`examples/features/drawlist_channels.das
<../../../examples/features/drawlist_channels.das>`.

Clip rect scope
===============

``with_clip_rect(min, max, intersect_with_current) { ... }`` (lives in
``imgui_scope_builtin``) restricts every widget and drawlist primitive
submitted inside the block to the given screen-space rect. Set
``intersect_with_current=true`` (typical) to intersect with the existing
clip; ``false`` replaces it. See :download:`examples/features/clip_rect.das
<../../../examples/features/clip_rect.das>`.

Path-key telemetry
==================

The ``[drawlist_prim]`` annotation synthesizes a stable
``<mod>:<line>:<col>`` path key at every call site (no per-call state
global is registered). Each primitive's body publishes a lightweight
``WidgetEntry`` — ``kind`` (``"add_line"`` / ``"add_rect"`` / ...) plus
``bbox`` — into the per-frame registry under that key. Playwright /
mouse-cards can then target a specific call site by path:

.. code-block:: das

   var snap = wait_for_widget(d, "MY_WIN/<mod>:42:8", 15.0f)
   t |> equal(find_widget(snap, "MY_WIN/<mod>:42:8")?["kind"] ?? "", "add_rect", ...)

Because the synthesized key shifts on edits, tests typically enumerate
drawlist entries by ``kind`` (``"add_rect"`` etc.) rather than hardcoding
the line / column — see ``tests/integration/test_drawlist_path_key.das`` for
the walker pattern.

Standalone vs live
==================

Same convention as the other tutorials. ``daslang.exe`` runs the demo
headlessly to frame N; ``daslang-live`` keeps the window open and reloads
on source edits.

.. seealso::

   Full source: :download:`examples/tutorial/drawlist.das <../../../examples/tutorial/drawlist.das>`

   Integration tests: ``tests/integration/test_drawlist_primitives.das``,
   ``tests/integration/test_with_window_drawlist.das``,
   ``tests/integration/test_drawlist_path_key.das``.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
