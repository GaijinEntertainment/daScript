# daScript
daScript - high-performance statically strong typed scripting language

Win64 build status [![Build status](https://ci.appveyor.com/api/projects/status/87jho7utcy3d5pcc/branch/master?svg=true)](https://ci.appveyor.com/project/imp5imp5/dascript/branch/master)\
Linux build status [![Build status](https://ci.appveyor.com/api/projects/status/n2xv2i829xuvjv3g/branch/master?svg=true)](https://ci.appveyor.com/project/imp5imp5/dascript-linux/branch/master)
macOS build status [![Build status](https://ci.appveyor.com/api/projects/status/dk833vli83lko0m4/branch/master?svg=true)](https://ci.appveyor.com/project/imp5imp5/dascript-mac/branch/master)


11/26/2021
  * hardware breakpoints are now supported on mac arm64
  * linux build is functional again

11/25/2012
  * better parallel build with msvc under vscode and all around
  * we can now generate xcode projects again

11/24/2021 - 0.3 tag
  * hash is now 64 bit function just about anywhere in daScript

11/23/2021
  * reuse allocator - things work a lot faster when enabled vs crt

11/22/2021
  * daScript parallel compilation cleanup - daScript parallel compilation
  * siginificant memory and performance optimizations
  * bugfix - desc query(eid) to be r\w

11/20/2021
  * daScript parser cleanup - its now reentrant

11/19/2021
  * logger - log handling, log levels, all that jazz
  * safebox container with hashes - can add and iterate

11/18/2021
  * minfft and sound modules merged in
  * feature - assume keyword

11/17/2021
  * checking locked containers all around move
  * OpenGL3+ bindings for IMGUI

11/16/2021
  * OpenGL is now up and running on OSX via binder
  * debugger now has decent eval

11/15/2021
  * better BGFX integration on OSX
  * dasAOT and dasContDev discontinued. use daScript for everything now
  * we now build our own GLFW

11/14/2021
  * archive and serialization is up and running

11/13/2021
  * HW breakpoints on PC
  * DECS state debugger
  * vulkan and related infrastructure temporarily removed

11/12/2021
  * local ref is unsafe
  * clang bind started

11/11/2021
  * lambda now captures const ref fine
  * find_query in DECS

11/10/2021
  * daScript now starts debugger automagically
  * DECS can now serialize
  * apply now works with misc types like variant and tuple

11/9/2021
  * DECS tests

11/8/2021
  * general language tests (found missing %= operator )
  * JIT tests
  * ZYDIS disassembly hooked

11/7/2021
  * dasbind - we can now dynamically load libraries and bind functions

11/6/2021
  * classes can now be sealed
  * member functions and variables can now be private

11/5/2021
  * file access roots
  * unit test framework is now merged in

11/4/2021
  * self keyword no longer necessary when calling class methods

11/3/2021
  * DECS now supports full-blown types (not just raw pod)

10/30/2021
  * XBYAK hooked up for inline assembly GENERATOR
  * operator := and operator delete (clone and finalize aliases)
  * more overrideable operators (like [], ?[], ., ?., etc)

10/29/2021
  * BGFX IMGUI

10/28/2021
  * geometry generation shared between OpenGL and BGFX
  * shader bindings now generate their own names
  * defaults and semantics inferred in GLSL

10/27/2021
  * BGFX - cleaner shader bindings
  * BGFX - matrices are now uploaded with proper transpose

10/25/2021
  * BGFX - compute shaders
  * support for const functions
  * default arguments now work in class methods

10/24/2021
  * BGFX - declarations, macros, etc

10/22/2021
  * popen - we can now run external process safely

10/21/2021
  * BGFX - module introduced

10/20/2021
  * das_string vs string - lots of nice operators for comparison etc
  * live context loopback

10/19/2021
  * math libraries unification (look_at etc matrices)

10/15/2021
  * LANGUAGE CHANGE - bitfields are now strongish type. i.e bitfield vs bitfield<...>
  * misc - massive cleanup of variable lookup. solid_context
  * misc - more macro cleanup
  * bug fixed - mat3x3 math is no longer broken
  * decs - find_query, query - harmonized, lookups, bugs fixed
  * live - now supports persistent desc data

10/9/2021
  * api change - aot is now part of CodeOfPolicies, not linkCppAot
  * misc - massive memory clenaup
      more compact debug info
      Func now stored pointer to a function
      GetGlobal and GetLocal are doing MNH for now
      MNH tables of all sorts are now regular hash_map, vs O(1) with really low capacity
        to be replaced by more efficient O(1) hash at some point
      code is now compacted for both interpreted and AOT scenarios
        unused code is thrown away in aot
  * misc - tones of new memory logging
  * feature - [macro_function] and [macro_interface]. to be thrown away from shared modules and such
  * misc - concept cleanup. module will no longer 'run'. init scripts at compilation time will only be called from non-modules
    macro scripts, however, will continue running
  * notes - attempts are shared code showed that keeping full blown (or even stripped) module is not viable

10/4/2021
  * feature - global_lint_macro - those happen reqardless of the visibility
  * FEATURE - LINT - daslib/lint for details. all 'i can catch it during compilation' stuff goes there.
  * misc - desc is now GC-friendly
  * feature - daslib/live started. it should handle live reloading, update, cross context in raw das
      live controls
  * bug fixed - annotationDataSid conflicts with SimNode_NOP, when empty
  * bug fixed - move of local nonRef types in strange instances no longer causes internal compiler error

### What's new
10/2/2021
  * FEATURE - DECS - which is daScript ECS.
      see daslib/decs and daslib/decs_boost for details. decs_test.das in examples.
      upcomming is separate context for ecs memory
  * feature - typedecl can now be a one-liner
  * misc - empty block now produces SimNode_NOP vs SimNode_Block with nothing. its way faster and less memory
  * feature - options log_symbol_use will report all used symbols with links on how they got used
  * feature - block macro is now bound to das
  * feature - many pass macros (lint, infer, dirty_infer, optimization) are now bound to das
  * feature - Expr as, Expr is, Expr ?is for ExpressionPtr
  * misc - better documentation. arguments names for just about everything
  * misc - significantly better binding error reporting. experimental is automatic lib.addModule(requre()) if it can be deduced from all modules.
  * greyprint - better preview

9/26/2021
  * bug fixed - integer vector division and modulo operations now cause proper 'division by zero' exceptions. unused components of the vector no longer cause said exceptions.

9/25/2021
  * misc - float3x3 matrix type introduced. basic operations provided.

9/24/2021
  * bug fixed - value[] arrays were considered value types due to the bug. additional cleanup of 'isSimpleType' function without arguments, which was only used in the dereferencing test.
  * bug fixed - numbers which look like octal in C++ were not parsing correctly. a = 08 was parsing as 0. now its 8.
  * feature - TypeInfo and StructInfo now contain two new flags (flag_heapGC and flag_stringHeapGC). GC uses those flags to disable walking specific data types. This has overall positive effect on performance, mainly for the arrays.
  * misc - bind_vertex_buffer no longer takes vbo, and takes an extra pointer instead.

9/23/2021
  * stb_truetupe integrated. opengl_ttf wrapper under dasGlfw helps with rendering as well (see 06_hello_ttf example)

9/22/2021
  * readme started somewhere around 'have functional GC'
  * lint - top level no-side-effect operator based expressions are now prohibited, i.e a == b is now a compilation error
  * bug fixed - if exception occurs during 'invoke_in_context', it will be forwarded to the original context

