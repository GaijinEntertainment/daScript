# Dynamic module descriptors (`.das_module`)

Read this before adding a module under `modules/` or shipping a package that others
`require`.

The default `daslang` binary is a DLL build: it resolves modules at startup by running
`modules/<name>/.das_module`, a small daslang script that maps require paths to files and
loads `.shared_module` DLLs. Without a descriptor entry, `require foo/bar` fails with
`error[20605] missing prerequisite 'foo/bar'` - even though the file compiles fine on its
own.

## The file

Named exactly `.das_module`, living at the module's root, exporting `initialize`:

```das
options gen2
require daslib/fio

[export]
def initialize(project_path : string) {
    // registration calls go here
}
```

`project_path` is the absolute path of the directory holding the descriptor, so every
path is written as `"{project_path}/..."`.

## `register_native_path` - pure-das modules

```das
register_native_path(mod_name, from_path, to_path)
```

- `mod_name` - the part before the first `/` in the require path: `require foo/bar` -> `"foo"`
- `from_path` - everything after it: `require foo/bar` -> `"bar"`; `require foo/sub/bar` -> `"sub/bar"`
- `to_path` - absolute path of the `.das` file

One call per `.das` file. For `require peg/peg`, `from_path` is `"peg"`, **not** `"peg/peg"`.

```das
[export]
def initialize(project_path : string) {
    for (path in ["peg", "meta_ast", "parse_macro"]) {
        register_native_path("peg", "{path}", "{project_path}/peg/{path}.das")
    }
}
```

## `register_dynamic_module` - C++ modules

```das
[export]
def initialize(project_path : string) {
    if (das_is_dll_build()) {
        register_dynamic_module("{project_path}/dasModuleFoo.shared_module", "Module_Foo")
    }
}
```

- First argument: path to the `.shared_module` DLL
- Second: the C++ class name from `REGISTER_MODULE(Module_Foo)`
- Guard with `das_is_dll_build()` - only the DLL binary loads DLLs
- One call per C++ module class; one descriptor may register several DLLs, and may mix in
  `register_native_path` calls for the module's `.das` files

## Package layout

A daspkg package is a module with the descriptor at the repo root:

```
my-package/
  .das_package        # metadata - skills/daspkg.md
  .das_module         # register_native_path("namespace", "module", ...)
  namespace/
    module.das        # require namespace/module
    helper.das        # require namespace/helper
  test_module.das     # tests and examples live at the root
```

Tests and examples at the root `require namespace/module` with no extra configuration:
the package root is the working directory, so the compiler finds `namespace/module.das`
relative to it. No `.das_project` is needed. The descriptor is what makes the same
require work after `daspkg install` drops the package into a consumer's `modules/`.

## The manifest beside the descriptor

The scan runs every descriptor as a daslang program on every start, and that costs more than
the rest of a small program's startup. So after a descriptor runs, the scan writes what it
registered to `.das_module.manifest` next to it, and later starts replay those rows without
compiling the descriptor. The file is keyed by the descriptor's content, the content of every
module its compile read (`require ./helper` beside it, the daslib it uses) and the four inputs a
descriptor can read - its folder, the das root, the binary kind and the `--jit-target`
cross-compile name: edit the descriptor or a module it requires, move the module or the tree,
switch binaries or targets and the next start recompiles it and rewrites the manifest. It is generated - gitignore it (this repo does) - and
a read-only tree just compiles on every start.

A descriptor whose registrations depend on something the file's content cannot see - a probe
of the machine, a variant picked by hardware - opts out by calling `no_manifest()` inside
`initialize`; it then runs on every start. Everything a descriptor registers is replayed, so
the opt-out is only for a descriptor whose answer changes between starts.

A replayed C++ module loads at the first `require` naming it, not in the scan: the require
walk loads the row and runs the `initDependencies` fixed point; a module needing another
deferred one pulls the whole deferred set in. A program pays for the C++ modules it requires.
Two consequences:

- `require ?mod x` and `typeinfo builtin_module_exists(mod)` still ask whether the build has
  `mod`: a guard loads a waiting module, so a cold start and a warm one answer alike.
- A tool that enumerates the process's modules (the MCP server, the LSP subtools) runs with
  `-ignore-manifest`: no manifest read or written, every descriptor compiles, every C++ module
  loads on start. `has_module(name)` (`daslib/rtti`) answers loaded-or-deferred, so a sweep
  gate asking what the tree has keeps its answer.

`DAS_TRACE_MODULE_LOAD=1` prints one line per descriptor (replayed or compiled, why, the
deferred count) and one per deferred module as a require loads it.

## Adding a `.das` file to an existing module needs the same edit

Require-root registration is manifest-driven: a file not named in the module's
`.das_module` is unreachable. Add the name in the commit that adds the file.

## Debugging

- `error[20605] missing prerequisite 'foo/bar'` - the descriptor has no matching
  `register_native_path("foo", "bar", ...)`, or the DLL registration is missing
- `das_is_dll_build()` reports at runtime which binary is running
- `.shared_module` is the C++ DLL extension; `.das_module` is the descriptor script -
  they are different things

Module resolution in both binaries and the CMake macros behind it: `skills/internal/dynamic_modules_plumbing.md` (repo-only).
