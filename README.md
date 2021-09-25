# daScript
daScript - high-performance statically strong typed scripting language

### What's new

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

