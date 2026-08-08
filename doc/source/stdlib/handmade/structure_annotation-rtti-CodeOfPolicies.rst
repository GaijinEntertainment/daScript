Object which holds compilation and simulation settings and restrictions.
Whether ahead-of-time compilation is enabled.
AOT library mode.
Whether standalone context AOT compilation is enabled.
Specifies to AOT if we are compiling a module, or a final program.
Enables AOT of macro code (like 'qmacro_block' etc).
Never apply per-box [tune] stamps to this compile (cross-box artifacts: AOT generation, AST serialization).
Whether paranoid validation is enabled (extra checks, no optimizations).
Whether to validate the AST after compilation (uniqueness checks, etc.). Off by default (slow).
Whether cross-platform AOT is enabled (if not, we generate code for the current platform).
File name for AOT output (if not set, we generate a temporary file).
If we are in code completion mode.
Whether this is a documentation/reflection compile; build-machine-specific transforms stay disabled.
Whether we are in lint-check mode (standalone linters set this so modules can adapt behavior).
Skip the Module::Initialize() assert in compileDaScript (for environments initialized later, e.g. dynamic-module discovery).
Export all functions and global variables.
If not set, we recompile main module each time.
Keep context alive after main function.
Whether to use very safe context (delete of data is delayed, to avoid table[foo]=table[bar] lifetime bugs).
Threshold for reporting candidates for function calls. If less than this number, we always report them.
Maximum number of inference passes.
Maximum call expression nesting depth during inference.
Stack size.
Whether to intern strings.
Whether to use persistent heap (or linear heap).
Whether multiple contexts are allowed (pinvokes between contexts).
Heap size hint.
String heap size hint.
Whether to use solid context (global variables are cemented at locations, can't be called from other contexts via pinvoke).
Whether macro context uses persistent heap.
Whether macro context does garbage collection.
Maximum size of static variables.
Maximum heap allocated.
Maximum string heap allocated.
Track where each heap allocation came from (line info + comment). When enabled, every allocation routes through the large-object map so ``heap->report()`` can list leaks with source location.
Whether to enable RTTI.
Whether to allow unsafe table lookups (via [] operator).
Whether to relax pointer constness rules.
Allows use of version 2 syntax.
Whether to use gen2 make syntax.
Allows relaxing of the assignment rules.
Disables all unsafe operations.
Local references are considered unsafe.
User-written `with (module ...)` requires unsafe.
Disallows global variables in this context (except for generated).
Disallows global variables at all in this context.
Disallows global heap in this context.
Only fast AOT, no C++ name generation.
Whether to consider side effects during AOT ordering.
Errors on unused function arguments.
Errors on unused block arguments.
Allows block variable shadowing.
Allows local variable shadowing.
Allows shared lambdas.
Ignore shared modules during compilation.
Default module mode is public.
Disallows use of deprecated features.
Disallows aliasing (if aliasing is allowed, temporary lifetimes are extended).
Enables strict smart pointer checks.
Disallows use of 'init' in structures.
Enables strict unsafe delete checks.
Disallows member functions in structures.
Disallows local class members.
Report invisible functions.
Report private functions.
Containers own their elements' lifetime: resize and fresh table slots get ``default<T>`` of the element type, and erase/clear/shrink finalize dropped elements whose finalizer is fully generated and frees owned heap. Host-side counterpart of ``options default_init_containers``.
Enables strict property checks.
Disables all optimizations.
Allows float optimizations with major bit differences (x*0, x-x, reciprocal division, NaN-compare flips, reassociation); also stamps JIT fast-math flags. Doubles stay bit-exact unless enabled.
Disables the dead-store-elimination optimizer pass. Host-side counterpart of ``options disable_dse`` (the option overrides the policy).
Disables the common-subexpression-elimination optimizer pass. Host-side counterpart of ``options disable_cse`` (the option overrides the policy).
Disables ``[inline]`` splicing - annotated calls stay regular calls. The declaration-level contract checks (body shape, recursion through the inline graph, taking the address) still run; call-site splice checks do not apply, since nothing splices. Host-side counterpart of ``options disable_inline`` (the option overrides the policy).
Disables automatic inlining: block-literal call sites stay regular calls and invoke-of-literal sites stay invokes. ``[inline]`` splicing is unaffected. Host-side counterpart of ``options disable_auto_inline`` (the option overrides the policy).
Heuristic best-effort inlining of plain calls and operator sites (default ON, optimized builds only): small same-module non-generic callees splice, silently declining anything unspliceable. Cross-module inlining stays the explicit ``[inline]`` contract. Host-side counterpart of ``options auto_inline_functions`` (the option overrides the policy). ``disable_auto_inline`` overrides.
Node budget for ``auto_inline_functions``: a loop-free callee body up to this many AST nodes is worth splicing (default 32); private callees referenced exactly once are exempt. Host-side counterpart of ``options auto_inline_cost``.
Disables compile-time function evaluation (RunFolding of pure calls over constant arguments). Host-side counterpart of ``options disable_run`` (the option overrides the policy).
Disables the temp-string reclaim pass: fresh-string call results consumed in place stop riding the 1-slot dispose queue. Reclaim only activates on the persistent, non-interned string heap. Host-side counterpart of ``options disable_temp_string_reclaim`` (the option overrides the policy).
Disables infer-time constant folding.
Fails compilation if AOT is not available.
Fails compilation if AOT export is not available.
Log compile time.
Log total compile time.
Log detailed per-module compile-time breakdown. Each required module emits its own block with parse / infer (with pass count) / optimize / macro (in infer) / macro mods times plus function count. Also enables a separate ``simulate (...) took X, modName (fileName)`` line for every ``Program::simulate`` call and the top-level aggregate summary at the end of compilation. Wired to the daslang CLI flag ``-log-compile-time``.
Log optimizer rewrites: per-pass fired/nothing lines plus inline/devirtualization sites and declines. Host-side counterpart of ``options log_optimization`` (the option overrides the policy).
Log the AST after every optimizer pass (verbose). Host-side counterpart of ``options log_optimization_passes`` (the option overrides the policy).
Disables fast call optimization.
Fuses interpreter nodes into wider superinstructions at simulate time (on by default). Host-side counterpart of ``options fusion`` (the option overrides the policy).
Reuse stack memory after variables go out of scope.
Force in-scope for POD-like types.
Log in-scope for POD-like types.
Escape analysis: free non-escaping ``new`` pointer locals at scope exit (GC-equivalent raw collect, no finalizer).
Escape analysis: allocate non-escaping ``new`` pointer locals on the stack frame instead of the heap (size-capped).
Escape analysis: flow-sensitive partial free - free non-escaping ``new`` pointer locals on the control-flow paths where they do not escape (builds a CFG).
Log escape-analysis static frees.
Enables debugger support.
Enables debug inference flag.
Enables profiler support.
Enables threadlock context.
JIT enabled - if enabled, JIT will be used to compile code at runtime.
JIT all functions - if enabled, JIT will compile all functions in the module.
JIT debug info - if enabled, JIT will generate debug info for JIT compiled code.
JIT dll mode - if enabled, JIT will generate DLL's into JIT output folder and load them from there.
JIT exe mode - if enabled, JIT will generate standalone executable.
JIT offline AOT object mode - emit a native .o (this module only) with a load constructor registering its functions into the AOT library, for static linking into a host binary.
JIT will always emit function prologues, which allows call-stack in debuggers.
JIT output folder (where JIT compiled code will be stored).
JIT optimization level for compiled code (0-3).
JIT size optimization level for compiled code (0-3).
Path to shared library, which is used in JIT.
Path to linker, which is used in JIT.
