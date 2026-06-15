.. _reference_spirv:

##################################
  Writing GPU shaders in Daslang
##################################

The ``dasSpirv`` module (``modules/dasSpirv``) is a pure-daslang compile-time backend that lowers
annotated daslang functions directly to **SPIR-V** binary — no GLSL, no glslang, no LLVM. A
``[compute_shader]`` / ``[vertex_shader]`` / ``[fragment_shader]`` function is read at compile time,
its AST is walked by an ``AstVisitor`` emitter, and the resulting SPIR-V words are captured into a
module global your host code feeds straight to ``vkCreateShaderModule``. Just as ``[jit]`` lowers
daslang to native code via LLVM, ``[compute_shader]`` lowers daslang to GPU code via SPIR-V.

Drivers do all GPU optimization, so dasSpirv emits *naive, valid* SPIR-V (glslang does the same) and
leaves the optimizing to the driver. Validity is enforced by ``spirv-val`` over every blob the test
suite produces; every emitted opcode has a test (an opcode census plus the LCOV dispatch gate).

This page is the language/API reference. For runnable, screenshot-backed walkthroughs (compute,
raymarch, textured mesh, shadow mapping, …) see the **dasVulkan tutorials**, which run the emitted
blobs on a real driver.


Overview
========

* **Module:** ``require spirv/spirv_shader`` brings in the three stage annotations and re-exports the
  shader builtins (``spirv_builtins``). The reflection types live in ``spirv_reflect``.
* **What you write:** an ordinary daslang function annotated with a stage, plus module-global
  ``var``\ s annotated with their resource role (``@ssbo`` / ``@uniform`` / ``@push_constant`` /
  ``@in`` / ``@out`` / a sampler type). The function body is never executed in daslang — only its AST
  is read and lowered.
* **What you get:** a module global ``array<uint>`` holding the SPIR-V words (named ``<func>`spirv``
  by default, or via the ``name`` argument), plus a companion ``<name>_reflect : array<uint>`` holding
  the encoded :ref:`reflection <reference_spirv_reflection>`.


Quick start: a compute shader
=============================

.. code-block:: das

    require spirv/spirv_shader

    // an SSBO at set 0, binding 0
    var @ssbo @binding = 0 data : array<uint>

    [compute_shader(local_size_x=64, name="square_spv")]
    def square {
        let i = gl_GlobalInvocationID.x
        data[i] = i * i
    }

After compilation, ``square_spv : array<uint>`` holds the SPIR-V. The host hands it to Vulkan and
decodes ``square_spv_reflect`` to build the descriptor-set layout — no hand-declared bindings:

.. code-block:: das

    require spirv/spirv_reflect

    let refl = decode_reflection(square_spv_reflect)
    // refl.bindings == [ {set=0, binding=0, kind=storage_buffer, count=1, stages=compute} ]
    // refl.local_size == int3(64, 1, 1)
    // feed square_spv words to vkCreateShaderModule (codeSize = 4*length, pCode = addr(words[0]))


Shader stages
=============

Each stage is a function macro. ``[compute_shader]`` takes ``local_size_x`` / ``local_size_y`` /
``local_size_z`` (default 1) for the workgroup size; all three take an optional ``name`` for the
captured global (default ``<func>`spirv``). The entry point is always emitted as ``"main"``.

.. list-table::
   :header-rows: 1
   :widths: 26 24 50

   * - Annotation
     - Execution model
     - Notes
   * - ``[compute_shader]``
     - ``GLCompute``
     - ``local_size_*`` → ``OpExecutionMode LocalSize``
   * - ``[vertex_shader]``
     - ``Vertex``
     - reads ``gl_VertexIndex`` / ``gl_InstanceIndex``, writes ``gl_Position``
   * - ``[fragment_shader]``
     - ``Fragment``
     - reads ``gl_FragCoord``; ``OriginUpperLeft``


Resources and stage I/O
=======================

Module-global ``var``\ s are classified by their annotation. ``@set`` / ``@binding`` / ``@location``
take an integer value (``@binding = 0``); ``@in`` / ``@out`` / ``@ssbo`` / ``@uniform`` /
``@push_constant`` are flags. Set and binding default to 0 when omitted.

.. list-table::
   :header-rows: 1
   :widths: 34 40 26

   * - Declaration
     - Lowers to
     - Reflected kind
   * - ``var @ssbo @set=S @binding=B``
     - StorageBuffer, std430, ``Block``
     - ``storage_buffer``
   * - ``var @uniform @set=S @binding=B``
     - Uniform, std140, ``Block``
     - ``uniform_buffer``
   * - ``var @push_constant``
     - PushConstant, ``Block`` (no descriptor)
     - push-constant range
   * - ``var @in @location=L``
     - Input, ``Location L``
     - — (graphics I/O)
   * - ``var @out @location=L``
     - Output, ``Location L``
     - — (graphics I/O)

``@uniform`` / ``@push_constant`` globals must be a ``struct`` (32-bit scalar / vector / matrix
members); ``@ssbo`` must be an ``array<T>``. Member access (``ubo.field``) lowers to ``OpAccessChain``
+ ``OpLoad``.


Built-in variables
==================

Declared in ``spirv_builtins`` and recognized by name; available only in the stages shown.

.. list-table::
   :header-rows: 1
   :widths: 36 16 32 16

   * - Builtin
     - Type
     - SPIR-V BuiltIn
     - Stage
   * - ``gl_GlobalInvocationID``
     - ``uint3``
     - ``GlobalInvocationId``
     - compute
   * - ``gl_LocalInvocationID``
     - ``uint3``
     - ``LocalInvocationId``
     - compute
   * - ``gl_LocalInvocationIndex``
     - ``uint``
     - ``LocalInvocationIndex``
     - compute
   * - ``gl_WorkGroupID``
     - ``uint3``
     - ``WorkgroupId``
     - compute
   * - ``gl_NumWorkGroups``
     - ``uint3``
     - ``NumWorkgroups``
     - compute
   * - ``gl_VertexIndex``
     - ``int``
     - ``VertexIndex``
     - vertex
   * - ``gl_InstanceIndex``
     - ``int``
     - ``InstanceIndex``
     - vertex
   * - ``gl_Position`` (write)
     - ``float4``
     - ``Position``
     - vertex
   * - ``gl_FragCoord``
     - ``float4``
     - ``FragCoord``
     - fragment


Textures and storage images
===========================

Sampler / image types are opaque marker structs; the sampling functions are recognized by name and
lower to the matching image op. Declare a sampler as a global with ``@set`` / ``@binding``.

.. list-table::
   :header-rows: 1
   :widths: 34 40 26

   * - Type / call
     - Lowers to
     - Reflected kind
   * - ``sampler2D`` / ``sampler3D`` / ``samplerCube`` / ``sampler2DArray``
     - combined image + sampler
     - ``combined_image_sampler``
   * - ``texture(s, uv)``
     - ``OpImageSampleImplicitLod``
     - — (fragment)
   * - ``textureLod(s, uv, lod)``
     - ``OpImageSampleExplicitLod``
     - — (any stage)
   * - ``texelFetch(s, coord, lod)``
     - ``OpImage`` + ``OpImageFetch``
     - — (any stage)
   * - ``texture2D`` + ``sampler``
     - separate image / separate sampler
     - ``sampled_image`` / ``sampler``
   * - ``sampleTexture(tex, smp, uv)``
     - ``OpSampledImage`` + sample
     - — (fragment)
   * - ``image2D`` (Rgba8)
     - read-write storage image
     - ``storage_image``
   * - ``imageLoad(img, coord)``
     - ``OpImageRead``
     - —
   * - ``imageStore(img, coord, value)``
     - ``OpImageWrite``
     - —


Type and layout mapping
=======================

.. list-table::
   :header-rows: 1
   :widths: 34 34 32

   * - Daslang
     - SPIR-V type
     - Layout
   * - ``int`` / ``uint`` / ``float``
     - ``OpTypeInt 32`` / ``OpTypeFloat 32``
     - 4 bytes
   * - ``bool``
     - ``OpTypeBool``
     - no physical interface-block layout
   * - ``int2..4`` / ``float2..4`` …
     - ``OpTypeVector``
     - component-wise
   * - ``float4x4`` / ``float3x3`` / ``float3x4``
     - ``OpTypeMatrix``
     - ColMajor, ``MatrixStride 16`` (std140)
   * - ``T[N]`` (fixed array)
     - ``OpTypeArray``
     - ``ArrayStride`` from element size/align
   * - ``array<T>`` (in ``@ssbo``)
     - ``OpTypeRuntimeArray``
     - std430
   * - ``struct`` (in ``@uniform``)
     - ``OpTypeStruct`` + ``Block``
     - std140 member ``Offset``\ s

Matrix · vector and matrix · matrix use SPIR-V's default column-major convention, so a daslang
``M * v`` is ``OpMatrixTimesVector`` with the matrix uploaded as-is.


Supported language surface
==========================

The emitter lowers the shader-legal subset of daslang. Anything outside it is **rejected with a clean
compile error** (never silently mis-emitted): no ``new`` / ``delete`` / variants / tuples / ``try`` /
string builders / ``goto`` — these are not shader constructs.

* **Control flow:** ``if`` / ``else`` (``OpSelectionMerge``), ``while`` and range-``for``
  (``OpLoopMerge``), ``break`` / ``continue``, early ``return``, and the ternary ``?:`` (``OpSelect``,
  branchless).
* **Operators:** full scalar and vector arithmetic (``+ - * / %``, unary ``-``), comparisons
  (``== != < > <= >=``), logical ``&&`` / ``||``, and matrix/vector products.
* **Math:** ``dot`` (``OpDot``) plus the GLSL.std.450 set — ``sin`` / ``cos`` / ``tan`` / ``pow`` /
  ``exp`` / ``log`` / ``sqrt`` / ``rsqrt`` / ``floor`` / ``ceil`` / ``fract`` / ``abs`` / ``min`` /
  ``max`` / ``lerp`` / ``length`` / ``distance`` / ``normalize`` / ``cross`` / ``reflect`` /
  ``refract`` and more (the daslang name maps to its ext-inst opcode).

.. note::

   This is a moving surface — additional rails (extra storage-image formats, depth-compare sampling,
   derivatives, ``discard``, shared memory + barriers + atomics, …) land per the dasSpirv roadmap.
   The authoritative, never-drifting list of emitted opcodes is the test suite's opcode census.


.. _reference_spirv_reflection:

Reflection
==========

dasSpirv emits, alongside each blob, an encoded :ref:`SpirvReflection <stdlib_spirv_reflect>`
describing the shader's descriptor bindings and push-constant ranges. The host decodes it once
(``decode_reflection``) and builds descriptor-set / pipeline layouts from it, so the binding facts
live in exactly one place — the shader source — and never drift from a hand-written layout. The wire
form is a versioned ``array<uint>`` that rides the same module-global-capture rail as the blob.

.. toctree::
   :maxdepth: 2

   ../stdlib/generated/spirv_reflect.rst


Tutorials
=========

Runnable, screenshot-backed tutorials — compute, fullscreen raymarching, textured meshes, shadow
mapping, compute particles, and more — live in the **dasVulkan** repository's ``tutorials/``
directory, where the emitted SPIR-V runs on a real Vulkan driver (and on lavapipe in CI).
