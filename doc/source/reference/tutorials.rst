.. _tutorials:

*****************************
  Tutorials
*****************************

This section provides hands-on tutorials organized into four groups:

* **Language Tutorials** — learn daslang syntax and standard library features
* **C Integration Tutorials** — embed daslang in a C host using the ``daScriptC.h`` API
* **C++ Integration Tutorials** — embed daslang in a C++ host using the native ``daScript.h`` API
* **Macro Tutorials** — write compile-time code transformations using the daslang macro system

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
``daScriptC.h`` API.  Each tutorial comes with a ``.c`` source file and a
companion ``.das`` script in ``tutorials/integration/c/``.

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