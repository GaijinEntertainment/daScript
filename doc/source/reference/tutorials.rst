.. _tutorials:

*****************************
  Tutorials
*****************************

This section provides hands-on tutorials organized by topic:

* **Language Tutorials** — learn daslang syntax and standard library features
* **C Integration Tutorials** — embed daslang in a C host using the ``daScriptC.h`` API
* **C++ Integration Tutorials** — embed daslang in a C++ host using the native ``daScript.h`` API
* **Macro Tutorials** — write compile-time code transformations using the daslang macro system
* **Module Tutorials** — dasHV (HTTP), dasLLAMA (local LLM inference), dasOPENAI (LLM / OpenAI-compatible API), dasPUGIXML (XML), dasStbImage, dasAudio, dasMinfft (FFT / DCT), dasVulkan and OpenGL / WebGL2 (graphics), dasPEG (parser generator)

.. _tutorials_language:

Language Tutorials
==================

These tutorials introduce daslang's core features step by step.
Each comes with a companion ``.das`` file in ``tutorials/language/`` that you
can run directly::

  daslang.exe tutorials/language/01_hello_world.das

The tutorials are designed to be followed in order. Each one builds on concepts
introduced in earlier tutorials.

.. toctree::
   :maxdepth: 1

   tutorials/01_hello_world.rst
   tutorials/02_variables.rst
   tutorials/03_operators.rst
   tutorials/04_control_flow.rst
   tutorials/05_functions.rst
   tutorials/06_arrays.rst
   tutorials/07_strings.rst
   tutorials/08_structs.rst
   tutorials/09_enumerations.rst
   tutorials/10_tables.rst
   tutorials/11_tuples_and_variants.rst
   tutorials/12_function_pointers.rst
   tutorials/13_blocks.rst
   tutorials/14_lambdas.rst
   tutorials/15_iterators_and_generators.rst
   tutorials/16_modules.rst
   tutorials/17_move_copy_clone.rst
   tutorials/18_classes.rst
   tutorials/19_generics.rst
   tutorials/20_lifetime.rst
   tutorials/21_error_handling.rst
   tutorials/22_unsafe.rst
   tutorials/23_string_format.rst
   tutorials/24_pattern_matching.rst
   tutorials/25_annotations_and_options.rst
   tutorials/26_contracts.rst
   tutorials/27_testing.rst
   tutorials/28_linq.rst
   tutorials/29_functional.rst
   tutorials/30_json.rst
   tutorials/31_regex.rst
   tutorials/32_operator_overloading.rst
   tutorials/33_algorithm.rst
   tutorials/34_decs.rst
   tutorials/35_jobque.rst
   tutorials/36_pointers.rst
   tutorials/37_utility_patterns.rst
   tutorials/38_random.rst
   tutorials/39_dynamic_type_checking.rst
   tutorials/40_coroutines.rst
   tutorials/41_serialization.rst
   tutorials/42_testing_tools.rst
   tutorials/43_interfaces.rst
   tutorials/44_compile_and_run.rst
   tutorials/45_debug_agents.rst
   tutorials/46_apply_in_context.rst
   tutorials/47_data_walker.rst
   tutorials/48_apply.rst
   tutorials/49_async.rst
   tutorials/50_soa.rst
   tutorials/51_delegate.rst
   tutorials/52_option_and_result.rst
   tutorials/53_clargs.rst
   tutorials/54_glob.rst
   tutorials/55_linq_decs.rst
   tutorials/56_linq_query.rst
   tutorials/57_toml.rst
   tutorials/58_logger.rst

.. _tutorials_building_from_sdk:

Building from the Installed SDK
================================

Once daslang is installed, you can build the integration tutorials — or your
own projects — against the SDK using CMake's ``find_package(DAS)``.

.. toctree::
   :maxdepth: 1

   tutorials/building_from_sdk.rst

.. _tutorials_integration_c:

C Integration Tutorials
=======================

These tutorials show how to embed daslang in a C application using the
``daScriptC.h`` API. Each tutorial comes with a ``.c`` source file in
``tutorials/integration/c/``. Most use a companion ``.das`` script; tutorials
which focus on virtual files or explicit-length source embed the script in C.

The installed SDK ships with a standalone ``CMakeLists.txt`` that lets you
build all C tutorials directly against the SDK — see
:ref:`tutorial_building_from_sdk`.

.. toctree::
   :maxdepth: 1

   tutorials/integration_c_01_hello_world.rst
   tutorials/integration_c_02_calling_functions.rst
   tutorials/integration_c_03_binding_types.rst
   tutorials/integration_c_04_callbacks.rst
   tutorials/integration_c_05_unaligned_advanced.rst
   tutorials/integration_c_06_sandbox.rst
   tutorials/integration_c_07_context_variables.rst
   tutorials/integration_c_08_serialization.rst
   tutorials/integration_c_09_aot.rst
   tutorials/integration_c_10_threading.rst
   tutorials/integration_c_11_type_introspection.rst
   tutorials/integration_c_12_ecs.rst
   tutorials/integration_c_13_shared_module.rst
   tutorials/integration_c_14_passing_arrays.rst
   tutorials/integration_c_15_static_stbimage.rst

.. _tutorials_integration_cpp:

C++ Integration Tutorials
=========================

These tutorials show how to embed daslang in a C++ application using the
native ``daScript.h`` API.  Each tutorial comes with a ``.cpp`` source file
and a companion ``.das`` script in ``tutorials/integration/cpp/``.

The installed SDK ships with a standalone ``CMakeLists.txt`` that lets you
build all C++ tutorials directly against the SDK — see
:ref:`tutorial_building_from_sdk`.

.. toctree::
   :maxdepth: 1

   tutorials/integration_cpp_01_hello_world.rst
   tutorials/integration_cpp_02_calling_functions.rst
   tutorials/integration_cpp_03_binding_functions.rst
   tutorials/integration_cpp_04_binding_types.rst
   tutorials/integration_cpp_05_binding_enums.rst
   tutorials/integration_cpp_06_interop.rst
   tutorials/integration_cpp_07_callbacks.rst
   tutorials/integration_cpp_08_methods.rst
   tutorials/integration_cpp_09_operators_and_properties.rst
   tutorials/integration_cpp_10_custom_modules.rst
   tutorials/integration_cpp_11_context_variables.rst
   tutorials/integration_cpp_12_smart_pointers.rst
   tutorials/integration_cpp_13_aot.rst
   tutorials/integration_cpp_14_serialization.rst
   tutorials/integration_cpp_15_custom_annotations.rst
   tutorials/integration_cpp_16_sandbox.rst
   tutorials/integration_cpp_17_coroutines.rst
   tutorials/integration_cpp_18_dynamic_scripts.rst
   tutorials/integration_cpp_19_class_adapters.rst
   tutorials/integration_cpp_20_standalone_contexts.rst
   tutorials/integration_cpp_21_threading.rst
   tutorials/integration_cpp_22_namespace_integration.rst
   tutorials/integration_cpp_23_handle_registry.rst

.. _tutorials_macros:

Macro Tutorials
===============

These tutorials teach daslang's compile-time macro system: call macros,
reader macros, function macros, and AST manipulation.  Each tutorial has
**two** source files — a module (``.das``) that defines the macros and a
usage file that exercises them — because macros cannot be used in the same
module that defines them.

Run any tutorial from the project root::

  daslang.exe tutorials/macros/01_call_macro.das

.. toctree::
   :maxdepth: 1

   tutorials/macros/01_call_macro.rst
   tutorials/macros/02_when_macro.rst
   tutorials/macros/03_function_macro.rst
   tutorials/macros/04_advanced_function_macro.rst
   tutorials/macros/05_tag_function_macro.rst
   tutorials/macros/06_structure_macro.rst
   tutorials/macros/07_block_macro.rst
   tutorials/macros/08_variant_macro.rst
   tutorials/macros/09_for_loop_macro.rst
   tutorials/macros/10_capture_macro.rst
   tutorials/macros/11_reader_macro.rst
   tutorials/macros/12_typeinfo_macro.rst
   tutorials/macros/13_enumeration_macro.rst
   tutorials/macros/14_pass_macro.rst
   tutorials/macros/15_type_macro.rst
   tutorials/macros/16_template_type_macro.rst
   tutorials/macros/17_qmacro.rst
   tutorials/macros/18_with_boost.rst
   tutorials/macros/19_add_module_option.rst
   tutorials/macros/20_template_struct_instance.rst

.. _tutorials_dashv:

dasHV (HTTP / WebSocket) Tutorials
==================================

These tutorials cover the ``dashv`` module — HTTP client and server
bindings built on `libhv <https://github.com/ithewei/libhv>`_.  The
companion ``.das`` files are in ``tutorials/dasHV/``.

Run any tutorial from the project root::

   daslang.exe tutorials/dasHV/01_http_requests.das

.. toctree::
   :maxdepth: 1

   tutorials/dasHV_01_http_requests.rst
   tutorials/dasHV_02_http_requests_advanced.rst
   tutorials/dasHV_03_http_server.rst
   tutorials/dasHV_04_http_server_advanced.rst
   tutorials/dasHV_05_cookies_and_forms.rst
   tutorials/dasHV_06_websockets.rst
   tutorials/dasHV_07_sse_and_streaming.rst
   tutorials/dasHV_08_https_wss.rst

.. _tutorials_dasllama:

dasLLAMA (CPU LLM Inference) Tutorials
======================================

Tutorial 00 states the problem with a complete, minimal Llama-2 inference
engine over a tiny llama2.c checkpoint. The remaining tutorials cover the
``dasllama`` module — CPU large-language-model inference in pure daslang:
loading GGUF models, tokenization, streaming generation, chat with templates,
sampling, sessions and memory, performance, the architecture registry,
speech-to-text, audio chat, embeddings, thinking models, tool calling,
voice-activity detection, and serving many conversations.
Tutorial 01 onward is written against the public ``dasllama/dasllama`` facade.

For tutorials 01 onward, you'll need a GGUF model file on disk (models are not
shipped with the repo) — a good tiny one is `SmolLM2-135M-Instruct Q8_0
<https://huggingface.co/bartowski/SmolLM2-135M-Instruct-GGUF>`_ (~145 MB).
dasLLAMA's tuning framework intentionally requires the JIT; interpreted and
AOT execution are not supported. Run those tutorials from the project root
with ``-jit``::

   daslang.exe -jit tutorials/dasLLAMA/01_hello_generate.das -- path/to/model.gguf

.. toctree::
   :maxdepth: 1

   tutorials/dasLLAMA_00_problem_statement.rst
   tutorials/dasLLAMA_01_hello_generate.rst
   tutorials/dasLLAMA_02_chat.rst
   tutorials/dasLLAMA_03_sampling.rst
   tutorials/dasLLAMA_04_sessions_and_memory.rst
   tutorials/dasLLAMA_05_performance.rst
   tutorials/dasLLAMA_06_add_an_arch.rst
   tutorials/dasLLAMA_07_speech_to_text.rst
   tutorials/dasLLAMA_08_audio_chat.rst
   tutorials/dasLLAMA_09_embeddings.rst
   tutorials/dasLLAMA_10_thinking.rst
   tutorials/dasLLAMA_11_tool_calling.rst
   tutorials/dasLLAMA_12_voice_activity.rst
   tutorials/dasLLAMA_13_serving.rst
   tutorials/dasLLAMA_14_vision_chat.rst
   tutorials/dasLLAMA_15_prefix_cache.rst

.. _tutorials_dasopenai:

dasOPENAI (OpenAI-compatible API) Tutorials
===========================================

These tutorials cover the ``openai`` module — a pure-daslang client for
OpenAI-compatible APIs (OpenAI, Ollama, OpenRouter, Kokoro, …): chat,
conversations, structured outputs, function calling, embeddings, models,
audio, and streaming. Built on ``dashv`` (HTTP) and ``daslib/json_boost``.
The companion ``.das`` files are in ``tutorials/dasOPENAI/`` and run against a
local mock server, so no live LLM is required.

Run any tutorial from the project root::

   daslang.exe tutorials/dasOPENAI/01_first_chat.das

.. toctree::
   :maxdepth: 1

   tutorials/dasOPENAI_01_first_chat.rst
   tutorials/dasOPENAI_02_conversations_and_params.rst
   tutorials/dasOPENAI_03_structured_outputs.rst
   tutorials/dasOPENAI_04_tools_and_function_calling.rst
   tutorials/dasOPENAI_05_embeddings_and_models.rst
   tutorials/dasOPENAI_06_audio.rst
   tutorials/dasOPENAI_07_streaming_chat.rst
   tutorials/dasOPENAI_08_vision.rst
   tutorials/dasOPENAI_09_image_generation.rst
   tutorials/dasOPENAI_10_moderations.rst
   tutorials/dasOPENAI_11_completions.rst

.. _tutorials_daspugixml:

dasPUGIXML (XML) Tutorials
===========================

These tutorials cover the ``dasPUGIXML`` module — XML parsing, building,
XPath queries, and automatic struct serialization using
`pugixml <https://pugixml.org/>`_.  The companion ``.das`` files are in
``tutorials/dasPUGIXML/``.

Run any tutorial from the project root::

   daslang.exe tutorials/dasPUGIXML/01_parsing_and_navigation.das

.. toctree::
   :maxdepth: 1

   tutorials/dasPUGIXML_01_parsing.rst
   tutorials/dasPUGIXML_02_building.rst
   tutorials/dasPUGIXML_03_xpath.rst
   tutorials/dasPUGIXML_04_serialization.rst
   tutorials/dasPUGIXML_05_linq.rst

.. _tutorials_dasstbimage:

dasStbImage (Image I/O) Tutorials
=================================

These tutorials cover the ``stbimage_boost`` module — image loading,
saving, format conversion, and pixel manipulation using
`stb_image <https://github.com/nothings/stb>`_.  The companion
``.das`` files are in ``tutorials/dasStbImage/``.

Run any tutorial from the project root::

   daslang.exe tutorials/dasStbImage/01_loading_images.das

.. toctree::
   :maxdepth: 1

   tutorials/dasStbImage_01_loading_images.rst
   tutorials/dasStbImage_02_saving_and_encoding.rst
   tutorials/dasStbImage_03_transforms.rst
   tutorials/dasStbImage_04_pixel_access_and_conversion.rst
   tutorials/dasStbImage_05_drawing_and_blending.rst
   tutorials/dasStbImage_06_truetype_fonts.rst
   tutorials/dasStbImage_07_hdr.rst

.. _tutorials_sql:

dasSQLITE (SQL) Tutorials
=========================

These tutorials cover the ``daslib/sql`` + ``sqlite/sqlite_boost`` API
— RAII connection handling, the ``[sql_table]`` structure macro, and
the typed CRUD helpers built on top of it.  The companion ``.das``
files are in ``tutorials/sql/``.

Run any tutorial from the project root::

   daslang.exe tutorials/sql/01-version.das

.. toctree::
   :maxdepth: 1

   tutorials/sql_01_hello.rst
   tutorials/sql_02_insert_data.rst
   tutorials/sql_03_last_row_id.rst
   tutorials/sql_04_select_all.rst
   tutorials/sql_05_parametrized.rst
   tutorials/sql_06_error_handling.rst
   tutorials/sql_07_anatomy.rst
   tutorials/sql_08_where.rst
   tutorials/sql_09_select.rst
   tutorials/sql_10_order_by.rst
   tutorials/sql_11_take_skip.rst
   tutorials/sql_12_distinct.rst
   tutorials/sql_12b_set_ops.rst
   tutorials/sql_13_aggregates.rst
   tutorials/sql_14_group_by.rst
   tutorials/sql_15_join.rst
   tutorials/sql_16_left_join.rst
   tutorials/sql_17_subqueries.rst
   tutorials/sql_18_null_handling.rst
   tutorials/sql_19_update.rst
   tutorials/sql_20_delete.rst
   tutorials/sql_21_upsert.rst
   tutorials/sql_22_transactions.rst
   tutorials/sql_23_foreign_keys.rst
   tutorials/sql_24_indexes.rst
   tutorials/sql_25_defaults_computed.rst
   tutorials/sql_26_custom_types.rst
   tutorials/sql_27_blob.rst
   tutorials/sql_28_json.rst
   tutorials/sql_29_column_names.rst
   tutorials/sql_30_list_tables.rst
   tutorials/sql_31_views.rst
   tutorials/sql_32_sql_functions.rst
   tutorials/sql_33_pragma.rst
   tutorials/sql_34_backup_vacuum.rst
   tutorials/sql_35_streaming.rst
   tutorials/sql_36_attach.rst
   tutorials/sql_37_bulk_operations.rst
   tutorials/sql_38_concurrency.rst
   tutorials/sql_39_schema_from.rst
   tutorials/sql_40_fts5.rst
   tutorials/sql_41_triggers.rst
   tutorials/sql_42_schema_evolution.rst
   tutorials/sql_43_migrations.rst

.. _tutorials_dasaudio:

dasAudio (Audio) Tutorials
==========================

These tutorials cover the ``audio_boost`` module — audio playback,
3D spatial audio, effects, WAV I/O, and MIDI.  The companion
``.das`` files are in ``tutorials/dasAudio/``.

Run any tutorial from the project root::

   daslang.exe tutorials/dasAudio/01_hello_sound.das

.. toctree::
   :maxdepth: 1

   tutorials/dasAudio_01_hello_sound.rst
   tutorials/dasAudio_02_playing_files.rst
   tutorials/dasAudio_03_sound_control.rst
   tutorials/dasAudio_04_spatial_audio.rst
   tutorials/dasAudio_05_reverb.rst
   tutorials/dasAudio_06_streaming.rst
   tutorials/dasAudio_07_wav_io.rst
   tutorials/dasAudio_08_midi.rst
   tutorials/dasAudio_09_playback_status.rst
   tutorials/dasAudio_10_global_controls.rst
   tutorials/dasAudio_11_recording.rst

.. _tutorials_dasminfft:

dasMinfft (FFT / DCT) Tutorials
===============================

These tutorials cover the ``minfft`` module — fast FFT and DCT on
power-of-two lengths, plus the 8x8 block-DCT pipeline at the heart of
JPEG and MPEG.  Companion ``.das`` files are in ``tutorials/dasMinfft/``.

Run any tutorial from the project root::

   daslang.exe tutorials/dasMinfft/01_real_fft.das

.. toctree::
   :maxdepth: 1

   tutorials/dasMinfft_01_real_fft.rst
   tutorials/dasMinfft_02_dct_basics.rst
   tutorials/dasMinfft_03_dct_image_compression.rst

.. _tutorials_dastrudel:

daStrudel (Live-Coding) Tutorials
==================================

These tutorials cover the ``strudel`` module — pattern-based live-coding
music with mini-notation, time algebra, per-voice effects, samples, SF2
soundfonts, MIDI playback, and live-reload.  Companion ``.das`` files
are in ``tutorials/daStrudel/``.

Run any tutorial from the project root::

   daslang.exe tutorials/daStrudel/daStrudel_01_hello_pattern.das

For the strudel-to-strudel.cc feature comparison, see
:ref:`strudel_vs_strudel_cc`.

.. toctree::
   :maxdepth: 1

   tutorials/daStrudel_01_hello_pattern.rst
   tutorials/daStrudel_02_mini_notation_fundamentals.rst
   tutorials/daStrudel_03_mini_notation_advanced.rst
   tutorials/daStrudel_04_time_manipulation.rst
   tutorials/daStrudel_05_euclidean_rhythms.rst
   tutorials/daStrudel_06_stacking_combining.rst
   tutorials/daStrudel_07_per_voice_fx.rst
   tutorials/daStrudel_08_effects_filters.rst
   tutorials/daStrudel_09_signals_modulation.rst
   tutorials/daStrudel_10_adsr_envelopes.rst
   tutorials/daStrudel_11_scales_music_theory.rst
   tutorials/daStrudel_12_synthesis.rst
   tutorials/daStrudel_13_samples.rst
   tutorials/daStrudel_14_sf2_soundfont.rst
   tutorials/daStrudel_15_midi_files.rst
   tutorials/daStrudel_16_live_reloading.rst
   tutorials/daStrudel_17_hrtf_position.rst
   tutorials/daStrudel_18_sfx_lab.rst
   tutorials/daStrudel_19_one_shots.rst

.. _tutorials_vulkan:

dasVulkan Tutorials
===================

Runnable, self-verifying tutorials for authoring GPU shaders **in daslang** and
driving them through dasVulkan. Each shader is written in daslang and lowered to
SPIR-V at compile time by dasSpirv (no GLSL, no glslang) -- the same language as
the host, compute and graphics alike.

The series builds **graphics → compute → 3D scene → instancing → environment →
GPU-driven scene → multi-pass → modern pipeline → mesh shaders → ray tracing**:
01_triangle is the canonical hello-triangle, 02_mandelbrot and 03_sdf swap to the
compute pipeline, 04_cube takes graphics into 3D with depth + UBO + push constant
+ texture, 05_instancing draws 1000 cubes in one call, 06_skybox wraps the scene
in a cubemap, 07_particles hands the vertex stream itself to a compute shader,
08_shadow runs two render passes per frame sharing one depth image, 09_msaa drops
``VkRenderPass`` entirely in favour of Vulkan 1.3 dynamic rendering plus 4x MSAA
with auto-resolve, 10_deferred brings everything together in a three-pass
deferred renderer (sampled G-buffer) with SSAO + shadow + many lights, and
11_hdr adds an HDR offscreen target + Karis-style five-level bloom pyramid + ACES
tonemap composite for the post-process rail. 12_gpu_driven hands the draw
decision to the GPU: a compute shader does Hi-Z occlusion culling, compacts
survivors into an indirect-draw buffer with a GPU-written count, and
``cmd_draw_indexed_indirect_count`` draws them with bindless materials. 13_mesh
and 14_teapot drop the vertex buffer entirely for the GPU-driven **mesh-shader**
pipeline -- cluster culling, then on-GPU Bezier tessellation of the Utah teapot.
Finally 15_raytracing leaves the rasterizer behind for **hardware ray tracing**:
acceleration structures, a ``VK_KHR_ray_tracing_pipeline``, and a traced shadow
ray per hit -- raygen, miss, and closest-hit shaders all authored in daslang.
Each tutorial's `Next` footer links to the one after.

Every tutorial lives in its own self-contained directory under
``modules/dasVulkan/tutorials/``: the shaders, the offscreen render, a
**pixel-oracle test** that is the lavapipe CI regression gate, and a recording
driver that renders the embedded ``.mp4``. The render behind each video is
pixel-checked **every CI run** by the tutorial's oracle test; the ``.mp4`` is the
documentation figure of that same verified render, regenerated manually with the
local recording driver (which needs stbimage + audio + ffmpeg, so it does not run
in CI). The mesh-shader tutorials (13_mesh, 14_teapot) soft-skip on CI's software
renderer, which lacks ``VK_EXT_mesh_shader``, and 15_raytracing soft-skips there
too (no ``VK_KHR_ray_tracing_pipeline``); 13_mesh's figure is a still image
rather than a video.

.. toctree::
   :maxdepth: 1

   tutorials/vulkan/01_triangle.rst
   tutorials/vulkan/02_mandelbrot.rst
   tutorials/vulkan/03_sdf.rst
   tutorials/vulkan/04_cube.rst
   tutorials/vulkan/05_instancing.rst
   tutorials/vulkan/06_skybox.rst
   tutorials/vulkan/07_particles.rst
   tutorials/vulkan/08_shadow.rst
   tutorials/vulkan/09_msaa.rst
   tutorials/vulkan/10_deferred.rst
   tutorials/vulkan/11_hdr.rst
   tutorials/vulkan/12_gpu_driven.rst
   tutorials/vulkan/13_mesh.rst
   tutorials/vulkan/14_teapot.rst
   tutorials/vulkan/15_raytracing.rst

.. _tutorials_opengl:

OpenGL / WebGL2 Tutorials
=========================

A ladder of runnable OpenGL tutorials that mirror the :ref:`dasVulkan
<tutorials_vulkan>` series shader-for-shader, to
the degree the WebGL2 portable floor allows. Every shader is written in
**daslang** -- the same shared shader builtins (``shader_lingua_franca``) the
Vulkan rail uses -- and lowered to GLSL (GLSL ES 3.00 on the web) at compile time
by dasGlsl. The same daslang shader language, lowered to a third backend.

Each tutorial is **one homogeneous program**: ``init`` / ``update`` /
``shutdown`` plus a ``main`` driver. On the desktop ``main()`` runs the loop in a
GLFW window; on the web the wasm run path drives the three lifecycle functions
from the browser's animation frame (the daslang ``Context`` persists across
frames), so the very same ``.das`` runs live in the playground. That live,
click-to-run version in the browser is what the OpenGL rail offers over the
Vulkan tutorials' recorded video.

The series builds **graphics → fragment compute → 3D scene → instancing →
environment → multi-pass → modern post**. WebGL2 has no compute / SSBO / indirect
draw / mesh shaders, so the Vulkan rungs that depend on those (gpu-driven,
mesh-shader) have no WebGL2 mirror -- and drawing that boundary explicitly is part
of the harmonization story: one shader language, one ladder, three backends, with
the portable floor made visible.

Run any tutorial from the project root::

   daslang.exe tutorials/opengl/01_triangle/01_triangle.das

.. toctree::
   :maxdepth: 1

   tutorials/opengl/01_triangle.rst
   tutorials/opengl/02_mandelbrot.rst
   tutorials/opengl/03_sdf.rst
   tutorials/opengl/04_cube.rst
   tutorials/opengl/05_instancing.rst
   tutorials/opengl/06_skybox.rst
   tutorials/opengl/07_particles.rst
   tutorials/opengl/08_shadow.rst
   tutorials/opengl/09_msaa.rst
   tutorials/opengl/10_deferred.rst
   tutorials/opengl/11_hdr.rst
   tutorials/opengl/12_gltf.rst

.. _tutorials_jsonrpc:

JSON-RPC 2.0 Tutorials
=======================

These tutorials cover ``daslib/jsonrpc`` — the transport-agnostic
JSON-RPC 2.0 library: building requests, implementing servers with
``dispatch_line``, and the §6 batch semantics. The companion ``.das``
files are in ``tutorials/jsonrpc/``.

Run any tutorial from the project root::

   daslang.exe tutorials/jsonrpc/01_request_response.das

.. toctree::
   :maxdepth: 1

   tutorials/jsonrpc_01_request_response.rst
   tutorials/jsonrpc_02_dispatch_line.rst
   tutorials/jsonrpc_03_batch.rst

.. _tutorials_daspeg:

dasPEG (Parser Generator) Tutorials
====================================

These tutorials cover the ``peg`` module --- daslang's built-in PEG
(Parsing Expression Grammar) parser generator.  Define grammars
directly in daslang using the ``parse`` macro; the compiler generates
a packrat parser at compile time.

The companion ``.das`` files are in ``tutorials/dasPEG/``.

Run any tutorial from the project root::

   daslang.exe tutorials/dasPEG/01_hello_parser.das

.. toctree::
   :maxdepth: 1

   tutorials/dasPEG_01_hello_parser.rst
   tutorials/dasPEG_02_calculator.rst
   tutorials/dasPEG_03_csv_parser.rst
   tutorials/dasPEG_04_email_validator.rst
   tutorials/dasPEG_05_json_parser.rst
   tutorials/dasPEG_06_debugging.rst
   tutorials/dasPEG_07_basic_interpreter.rst

.. _tutorials_imgui:

dasImgui Tutorials
==================

Step-by-step walkthroughs of dasImgui's v2 widget DSL — application lifecycle,
widgets, containers, layout and docking, live reload, the playwright testing
harness, and tutorial recording. Each tutorial pairs an annotated source file
under ``modules/dasImgui/examples/tutorial/`` with prose narrating the
construction, and most embed a recorded video of the running app.

.. toctree::
   :maxdepth: 1

   tutorials/imgui/application_lifecycle.rst
   tutorials/imgui/boost_basics.rst
   tutorials/imgui/widgets_tour.rst
   tutorials/imgui/display_widgets.rst
   tutorials/imgui/narrative_widgets.rst
   tutorials/imgui/custom_widgets.rst
   tutorials/imgui/layout.rst
   tutorials/imgui/layout_primitives.rst
   tutorials/imgui/docking.rst
   tutorials/imgui/with_style.rst
   tutorials/imgui/with_id.rst
   tutorials/imgui/with_disabled.rst
   tutorials/imgui/state_telemetry.rst
   tutorials/imgui/containers.rst
   tutorials/imgui/child.rst
   tutorials/imgui/group.rst
   tutorials/imgui/tree_node.rst
   tutorials/imgui/collapsing_header.rst
   tutorials/imgui/tab_bar.rst
   tutorials/imgui/popup_modal.rst
   tutorials/imgui/main_menu_bar.rst
   tutorials/imgui/popup_window.rst
   tutorials/imgui/flat_tooltips.rst
   tutorials/imgui/drag_drop.rst
   tutorials/imgui/live_reload.rst
   tutorials/imgui/driving_outside.rst
   tutorials/imgui/visual_aids_tour.rst
   tutorials/imgui/narrative_layout_tour.rst
   tutorials/imgui/edit_external_tour.rst
   tutorials/imgui/recording.rst
   tutorials/imgui/harness_headless_mode.rst
   tutorials/imgui/data_table.rst
   tutorials/imgui/file_dialog.rst
   tutorials/imgui/window_size_constraints.rst
   tutorials/imgui/with_tab_stop.rst
   tutorials/imgui/color_button_hover.rst
   tutorials/imgui/selectable_hover.rst
   tutorials/imgui/drawlist.rst
   tutorials/imgui/drag.rst
   tutorials/imgui/slider.rst
   tutorials/imgui/input_numeric.rst
   tutorials/imgui/input_text.rst
   tutorials/imgui/toggles.rst
   tutorials/imgui/dropdown_select.rst
   tutorials/imgui/color.rst
   tutorials/imgui/buttons.rst
   tutorials/imgui/plot.rst
   tutorials/imgui/tree_image_misc.rst
   tutorials/imgui/popups.rst
   tutorials/imgui/edit_tab_item.rst
   tutorials/imgui/icons.rst
   tutorials/imgui/wrap_tab_bar.rst
   tutorials/imgui/dynamic_fonts.rst
   tutorials/imgui/texture_ref.rst
   tutorials/imgui/item_flags.rst
   tutorials/imgui/log_capture.rst
