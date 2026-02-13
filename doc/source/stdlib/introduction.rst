.. _stdlib_introduction:

============
Introduction
============

The Daslang standard library is a collection of modules that extend the language
with commonly needed functionality — math, string manipulation, file I/O,
serialization, regular expressions, AST manipulation, and more.

Some modules are implemented in C++ and are available by default (``builtin``, ``math``,
``strings``, ``fio``, ``rtti``, ``ast``, ``network``, ``jobque``, ``uriparser``).
The rest are written in daScript itself and live in the ``daslib/`` directory;
import them with ``require daslib/<module_name>``.

Core runtime
------------

* :doc:`builtin <builtin>` — built-in runtime functions, operators, containers, smart pointers, and system infrastructure
* :doc:`math <math>` — vector and scalar math, trigonometry, noise, matrix and quaternion operations
* :doc:`math_bits <math_bits>` — bit-level float/int/double reinterpretation helpers
* :doc:`math_boost <math_boost>` — angle conversions, projection matrices, plane math, color packing
* :doc:`fio <fio>` — file input/output, directory manipulation, process spawning
* :doc:`random <random>` — LCG-based random number generators and distributions
* :doc:`network <network>` — low-level TCP socket server
* :doc:`uriparser <uriparser>` — URI parsing, normalization, and file-name conversion (based on UriParser)
* :doc:`uriparser_boost <uriparser_boost>` — URI component accessors and query-string helpers

Strings
-------

* :doc:`strings <strings>` — core string manipulation: search, slice, conversion, builder, character groups
* :doc:`strings_boost <strings_boost>` — split/join, replace, Levenshtein distance, formatting helpers
* :doc:`utf8_utils <utf8_utils>` — UTF-8 encoding and decoding utilities
* :doc:`temp_strings <temp_strings>` — temporary string construction that avoids heap allocation
* :doc:`stringify <stringify>` — ``%stringify~`` reader macro for embedding long strings at compile time
* :doc:`base64 <base64>` — Base64 encoding and decoding

Regular expressions
-------------------

* :doc:`regex <regex>` — regular expression matching and searching
* :doc:`regex_boost <regex_boost>` — ``%regex~`` reader macro for compile-time regex construction

Reflection and AST
------------------

* :doc:`rtti <rtti>` — runtime type information: type queries, module/function/variable iteration, compilation and simulation
* :doc:`ast <ast>` — compile-time AST access: expression/function/structure creation, visitor pattern, macro infrastructure
* :doc:`ast_boost <ast_boost>` — AST convenience helpers: queries, annotation manipulation, expression generation, visitor utilities
* :doc:`ast_block_to_loop <ast_block_to_loop>` — AST transform that converts block-based iteration to explicit loops (used by DECS)
* :doc:`ast_used <ast_used>` — collect all types used by a set of functions (for code generation)
* :doc:`quote <quote>` — AST quasiquotation for constructing syntax trees from inline code
* :doc:`type_traits <type_traits>` — compile-time type introspection and manipulation macros
* :doc:`typemacro_boost <typemacro_boost>` — type macro and template structure support infrastructure
* :doc:`dynamic_cast_rtti <dynamic_cast_rtti>` — runtime dynamic casting between class hierarchies

Functional and algorithms
-------------------------

* :doc:`functional <functional>` — higher-order functions: ``filter``, ``map``, ``reduce``, ``any``, ``all``, ``flatten``, ``sorted``
* :doc:`algorithm <algorithm>` — binary search, topological sort, set operations on tables, array utilities
* :doc:`sort_boost <sort_boost>` — custom comparator support for the built-in ``sort``
* :doc:`linq <linq>` — LINQ-style query operations: select, where, order, group, join, aggregate, set operations
* :doc:`linq_boost <linq_boost>` — macro support for LINQ query syntax

Data structures
---------------

* :doc:`flat_hash_table <flat_hash_table>` — open-addressing flat hash table template
* :doc:`cuckoo_hash_table <cuckoo_hash_table>` — cuckoo hash table with O(1) worst-case lookup
* :doc:`bool_array <bool_array>` — packed boolean array (``BoolArray``) backed by ``uint[]`` storage
* :doc:`linked_list <linked_list>` — intrusive doubly-linked list data structure
* :doc:`soa <soa>` — Structure of Arrays transformation for cache-friendly data layout

Serialization and data
----------------------

* :doc:`archive <archive>` — general-purpose binary serialization framework with ``Serializer`` / ``Archive`` pattern
* :doc:`json <json>` — JSON parser and writer (``JsValue`` variant, ``read_json``, ``write_json``)
* :doc:`json_boost <json_boost>` — automatic struct ↔ JSON conversion via reflection

Jobs and concurrency
--------------------

* :doc:`jobque <jobque>` — job queue primitives: channels, job status, lock boxes, atomics
* :doc:`jobque_boost <jobque_boost>` — ``new_job`` / ``new_thread`` helpers, channel iteration
* :doc:`apply_in_context <apply_in_context>` — cross-context function evaluation helpers
* :doc:`async_boost <async_boost>` — async/await coroutine macros using job queues

Macros and metaprogramming
--------------------------

* :doc:`templates <templates>` — ``decltype`` macro and ``[template]`` function annotation
* :doc:`templates_boost <templates_boost>` — template application helpers: variable/type replacement, hygienic names
* :doc:`macro_boost <macro_boost>` — miscellaneous macro manipulation utilities
* :doc:`contracts <contracts>` — function argument contract annotations (``[expect_any_array]``, ``[expect_any_table]``, etc.)
* :doc:`apply <apply>` — ``apply`` reflection pattern for struct field iteration
* :doc:`enum_trait <enum_trait>` — compile-time enumeration trait queries
* :doc:`constexpr <constexpr>` — constant expression detection and substitution macro
* :doc:`bitfield_boost <bitfield_boost>` — operator overloads for bitfield types
* :doc:`bitfield_trait <bitfield_trait>` — reflection utilities for bitfield names and values
* :doc:`consume <consume>` — ``consume`` pattern for move-ownership argument passing
* :doc:`generic_return <generic_return>` — ``[generic_return]`` annotation for return type instantiation
* :doc:`remove_call_args <remove_call_args>` — AST transformation to remove specified call arguments
* :doc:`class_boost <class_boost>` — macros for extending class functionality and method binding

Control flow macros
-------------------

* :doc:`defer <defer>` — ``defer`` and ``defer_delete`` — execute code at scope exit
* :doc:`if_not_null <if_not_null>` — ``if_not_null`` safe-access macro
* :doc:`safe_addr <safe_addr>` — ``safe_addr`` and ``temp_ptr`` — safe temporary pointer macros
* :doc:`static_let <static_let>` — ``static_let`` — variables initialized once and persisted across calls
* :doc:`lpipe <lpipe>` — left-pipe operator macro (``<|``)
* :doc:`is_local <is_local>` — ``is_local_expr`` / ``is_scope_expr`` AST query helpers
* :doc:`assert_once <assert_once>` — ``assert_once`` — assertion that fires only on first failure
* :doc:`unroll <unroll>` — compile-time loop unrolling macro
* :doc:`instance_function <instance_function>` — ``[instance_function]`` annotation for struct method binding
* :doc:`export_constructor <export_constructor>` — auto-export default constructors for annotated structs
* :doc:`array_boost <array_boost>` — ``temp_array``, ``array_view``, and ``empty`` helpers
* :doc:`heartbeat <heartbeat>` — periodic heartbeat callback injection

Pattern matching
----------------

* :doc:`match <match>` — ``match`` macro for structural pattern matching on variants and values

Entity component system
-----------------------

* :doc:`decs <decs>` — DECS (Daslang Entity Component System): archetypes, components, queries, staged updates
* :doc:`decs_boost <decs_boost>` — DECS macro support for query syntax
* :doc:`decs_state <decs_state>` — DECS state machine support for entity lifecycle

OOP and interfaces
------------------

* :doc:`coroutines <coroutines>` — coroutine runner (``cr_run``, ``cr_run_all``) and generator macros (``yield_from``)
* :doc:`interfaces <interfaces>` — ``[interface]`` and ``[implements]`` annotations for interface-based polymorphism
* :doc:`cpp_bind <cpp_bind>` — C++ class adapter binding code generator

Testing and debugging
---------------------

* :doc:`faker <faker>` — random test data generator: strings, numbers, dates
* :doc:`fuzzer <fuzzer>` — function fuzzing framework
* :doc:`coverage <coverage>` — code coverage instrumentation and reporting
* :doc:`profiler <profiler>` — instrumenting CPU profiler for function-level timing
* :doc:`profiler_boost <profiler_boost>` — profiler cross-context helpers and high-level macros
* :doc:`debug_eval <debug_eval>` — runtime expression evaluation for interactive debugging
* :doc:`dap <dap>` — Debug Adapter Protocol (DAP) data structures for debugger integration

Code quality and tooling
------------------------

* :doc:`lint <lint>` — static analysis pass for common code issues
* :doc:`lint_everything <lint_everything>` — global lint pass applying paranoid diagnostics to all modules
* :doc:`validate_code <validate_code>` — AST validation annotations for custom code checks
* :doc:`refactor <refactor>` — automated code refactoring transformations

Developer tools
---------------

* :doc:`das_source_formatter <das_source_formatter>` — daScript source code formatter
* :doc:`das_source_formatter_fio <das_source_formatter_fio>` — file-based source code formatting
* :doc:`rst <rst>` — RST documentation generator used to produce this reference
