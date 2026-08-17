Windowing and presentation: create a ``SurfaceKHR`` from a native window,
build and **recreate** a ``Swapchain`` (with its images, views, and
framebuffers), set up per-frame ``FrameSync`` primitives, and run the
acquire → record → submit → present loop via ``draw_frame``. Pair with
``create_graphics_pipeline_simple(..., dynamic_viewport = true)`` for
resize-friendly pipelines.
