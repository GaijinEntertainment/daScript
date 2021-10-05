# daScript
daScript - high-performance statically strong typed scripting language

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

