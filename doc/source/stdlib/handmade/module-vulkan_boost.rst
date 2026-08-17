The hand-written ergonomic core: device/queue **selection** helpers, combined
**builders** that fold several Vulkan objects into one owner
(``build_offscreen_target``, ``create_host_buffer``), **block brackets**
(``run_cmd_sync``, ``record_render_pass``, ``map_memory_to_array``) that pair a
begin/end or map/unmap around a block, and small conveniences (``full_area``,
``clear_color``). This is the layer the examples are written against.
