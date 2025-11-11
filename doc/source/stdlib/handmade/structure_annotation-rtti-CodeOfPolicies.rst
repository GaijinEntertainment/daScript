Object which holds compilation and simulation settings and restrictions.
whether ahead-of-time compilation is enabled
whether standalone context AOT compilation is enabled
specifies to AOT if we are compiling a module, or a final program
enables AOT of macro code (like 'qmacro_block' etc)
if we are in code completion mode
export all functions and global variables
if not, we recompile main module each time
keep context alive after main function
whether to use very safe context (delete of data is delayed, to avoid table[foo]=table[bar] lifetime bugs)
threshold for reporting candidates for function calls. if less than this number, we always report them
maximum number of inference passes
stack size
whether to intern strings
whether to use persistent heap (or linear heap)
whether multiple contexts are allowed (pinvokes between contexts)
heap size hint
string heap size hint
whether to use solid context (global variables are cemented at locations, can't be called from other contexts via pinvoke)
whether macro context uses persistent heap
whether macro context does garbage collection
maximum size of static variables
maximum heap allocated
maximum string heap allocated
whether to enable RTTI
whether to allow unsafe table lookups (via [] operator)
whether to relax pointer constness rules
allows use of version 2 syntax
whether to use gen2 make syntax
allows relaxing of the assignment rules
disables all unsafe operations
local references are considered unsafe
disallows global variables in this context (except for generated)
disallows global variables at all in this context
disallows global heap in this context
only fast AOT, no C++ name generation
whether to consider side effects during AOT ordering
errors on unused function arguments
errors on unused block arguments
allows block variable shadowing
allows local variable shadowing
allows shared lambdas
ignore shared modules during compilation
default module mode is public
disallows use of deprecated features
disallows aliasing (if aliasing is allowed, temporary lifetimes are extended)
enables strict smart pointer checks
disallows use of 'init' in structures
enables strict unsafe delete checks
disallows members functions in structures
disallows local class members
report invisible functions
report private functions
enables strict property checks
disables all optimizations
fails compilation if AOT is not available
fails compilation if AOT export is not available
log compile time
log total compile time
disables fast call optimization
enables debugger support
enables debug inference flag
sets debug module (module which will be loaded when IDE connects)
enables profiler support
sets profile module (module which will be loaded when profiler connects)
enables JIT support
sets JIT module (module which will be loaded when JIT is enabled)
enables threadlock context
