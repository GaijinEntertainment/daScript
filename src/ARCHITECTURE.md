# src Architecture

`src/` is the daslang compiler and runtime. Each subfolder carries its own architecture
documents; this one holds what is true across them.

## Where a written static lives {#static-storage-homes}

A host compiles and runs on several threads. Each thread binds an environment
(`daScriptEnvironment`) - its own, or the one a worker shares with the thread that started it -
and nothing orders one thread's first call against another's. So an object with static storage
duration that code writes at run time has one of these homes:

- **A thread** - `thread_local`, for state one thread's call sequence owns: the AOT main-module
  name an emission sets and then reads, the parallel JIT emit queue between its `begin` and
  its `run`, the arguments a `with_argv` block puts over the process command line.
- **An environment** - a field of `daScriptEnvironment`, or of a `Program`, `Module` or
  `Context` reached through it, for state one environment owns.
- **A value built once** - a `const` function-local static built by its own initializer: a
  setting read from an environment variable.
- **A guarded process object** - every read and write takes one named lock or goes through an
  atomic: the das root, the diagnostics sink, the process command line.

A static mutex or atomic is the guard itself and needs no other home. Run time starts after an
object's own initializer and after the constructors a linked object runs as it loads
(`das_aot_register`): those finish before a second thread can call in, so an object only they
write needs no home.

Ruled exceptions:

- `g_jobQue` (`builtin/module_builtin_jobque.cpp`) is written under `g_jobQueMutex`, and every
  read of it in that file takes no lock. A `with_job_que` block holds its own copy of the
  pointer, taken under the lock, so no other block's exit resets the queue while a job is
  dispatched from inside one. The reads sit on the dispatch path of every job, team dispatch
  included.
- `g_pending_dynamic_modules`, `g_registered_dynamic_modules` and `g_registered_native_paths`
  (`builtin/module_builtin_fio.cpp`) and `g_nameless_manifests` (`ast/dyn_modules.cpp`) belong
  to the process, and the one module scan a process runs writes them.
