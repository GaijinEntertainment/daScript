.. _vulkan_overview:

####################
dasVulkan overview
####################

dasVulkan is two layers over the same ``vk.xml`` registry. The **raw** layer
(module ``vulkan``) is a mechanical 1:1 binding of the Vulkan C API; the
**boost** layer (module ``vulkan_boost`` and its siblings) is the idiomatic
daslang surface documented in this section. The boost layer is itself generated
from ``vk.xml`` wherever the transform is mechanical, and hand-written where it
encodes judgement.

The raw layer
=============

``require vulkan`` gives you the entire Vulkan surface — every ``Vk*`` struct as
a daslang structure, every enum, every ``VkFlags`` as a daslang ``bitfield``,
and ~820 command wrappers dispatching through volk. These names match the C API
exactly, so the
`Vulkan specification <https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html>`_
is their reference. You rarely call the raw layer directly — the boost layer
wraps the common paths — but it is always available as an escape hatch.

The boost layer
===============

Five modules, re-exported together by ``require vulkan/vulkan_boost``:

``vulkan_handles``
    One RAII wrapper struct per Vulkan handle. The wrapper owns its handle and
    destroys it in ``finalize`` when an ``var inscope`` owner leaves scope.

``vulkan_commands``
    The ``create_*`` / ``allocate_*`` creators that return those owning
    wrappers.

``vulkan_structs`` *(generated, not enumerated here)*
    An idiomatic *view* struct for every ``Vk*`` struct that carries arrays,
    handles, or strings: ``array<T>`` fields replace count+pointer pairs,
    ``string`` replaces ``const char*``, boost handle types replace raw ones,
    and ``sType`` / ``pNext`` / ``pAllocator`` are erased. A hidden scratch
    region marshals the view back to the raw struct on demand. There are
    ~1000 of these; they shadow the Vulkan structs 1:1, so the
    `spec <https://registry.khronos.org/vulkan/specs/latest/html/vkspec.html>`_
    documents their fields.

``vulkan_ctors`` *(generated, not enumerated here)*
    ``sType``-filling constructors for every struct that carries an ``sType``.

``vulkan_cmds`` *(generated, not enumerated here)*
    Plain boost wrappers for the ``vkCmd*`` / ``vkGet*`` / ``vkQueue*`` command
    set — boost handle/struct arguments, defaulted parameters, two-call
    enumeration auto-collapsed to ``array<T>``. ~400 of these, again 1:1 with
    the spec.

``vulkan_boost`` / ``vulkan_window``
    The hand-written core: device selection, combined builders, block
    brackets, and the windowing/swapchain/present helpers.

Ownership
=========

Handle wrappers are value types whose ``finalize`` destroys the handle. The
rules:

- Declare every owner with ``var inscope`` so it finalizes (in reverse order)
  at scope exit. A plain ``var x <- create_*()`` will **leak** — handles are
  raw pointers with no GC safety net.
- Parent handles are stored as raw handles inside the wrapper, never as nested
  wrappers, so a wrapper copy never duplicates ownership of its parent.
- For an intentional non-owning alias, use ``weak_copy`` (it clears the
  ``_needs_delete`` flag on the copy).

A worked example
================

The offscreen triangle, in full boost form — no hand-set ``sType``, no user
``unsafe`` / ``addr``, no manual destroy:

.. code-block:: das

    var inscope instance <- create_instance("dasVulkan triangle", make_api_version(1u, 3u, 0u))
    volkLoadInstance(boost_value_to_vk(instance))
    let phys = select_physical_device(instance)
    let gfx  = select_graphics_queue_family(phys)
    var inscope device <- create_device(phys, gfx)
    volkLoadDevice(boost_value_to_vk(device))
    let queue = get_device_queue(device, gfx, 0u)

    var inscope target      <- build_offscreen_target(device, phys, WIDTH, HEIGHT, FORMAT)
    var inscope render_pass <- create_render_pass_single_color(device, FORMAT)

    // CreateInfo view structs use the named-argument constructor. Handle fields
    // take a weak_copy (a non-owning alias — the create_* keeps ownership);
    // non-copyable array fields are move-initialized with <-. Note the C-style
    // field names (renderPass, pAttachments) — see the p-prefix note above.
    var fbci = FramebufferCreateInfo(renderPass = weak_copy(render_pass),
                                     pAttachments <- [weak_copy(target.view)],
                                     width = uint(WIDTH), height = uint(HEIGHT), layers = 1u)
    var inscope framebuffer <- create_framebuffer(device, fbci)

    var inscope vert     <- create_shader_module(device, vert_code)
    var inscope frag     <- create_shader_module(device, frag_code)
    var inscope layout   <- create_pipeline_layout(device)
    var inscope pipeline <- create_graphics_pipeline_simple(device, render_pass, layout, vert, frag, WIDTH, HEIGHT)
    var inscope readback <- create_host_buffer(device, phys, buf_size)
    var inscope pool     <- create_command_pool(device, command_pool_info(gfx))

    run_cmd_sync(device, pool, queue) $(cmd) {
        record_render_pass(cmd, render_pass, framebuffer, full_area(WIDTH, HEIGHT),
                           clear_color(0.1f, 0.1f, 0.15f, 1.0f)) {
            cmd_bind_pipeline(cmd, pipeline)
            cmd_draw(cmd, 3u)
        }
        copy_image_to_buffer(cmd, target.image, readback, WIDTH, HEIGHT)
    }

    var pixels : array<uint8>
    map_memory_to_array(device, readback.memory, buf_size) $(m) { pixels := m }
    // ... write PPM ...
    // finalizers destroy each inscope owner in reverse order — no cleanup block.

The complete, compiling versions live under ``modules/dasVulkan/examples/``
(``offscreen_triangle_boost.das``, ``compute_boost.das``, ``enumerate.das``,
``window_triangle.das``; ``offscreen_triangle.das`` and ``compute.das`` are the
raw references).
