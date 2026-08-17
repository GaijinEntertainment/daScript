.. _utils_jobque_timeline:

.. index::
   single: Utils; jobque-timeline
   single: Utils; Trace viewer

==========================================
 jobque-timeline --- Jobque Trace Viewer
==========================================

jobque-timeline opens ``jobque_trace_save`` / ``daslib/jobque_profile``
JSON trace files (perfetto-compatible) in dockable ImGui windows: one
row per lane, events colored by category, unit markers, computed stats
(idle %, parallelism, busy-by-category), multi-file compare with shared
zoom, and a launch rail that reruns the trace-producing app and
auto-refreshes on save.  It needs dasImgui, which ships with the SDK.

Quick start
===========

::

   daslang -project_root utils/jobque-timeline utils/jobque-timeline/main.das -- a.trace.json b.trace.json

Or live (hot reload + remote commands on port 9090)::

   daslang-live -project_root utils/jobque-timeline utils/jobque-timeline/main.das

Wheel zooms at the cursor, drag pans, double-click fits the full span,
shift+drag selects a time range with a stats line.  The full interaction
and ``timeline_*`` live-command reference is the tool's README.

.. seealso::

   :ref:`utils_daslang_live` -- the live-reload host the launch rail integrates with
