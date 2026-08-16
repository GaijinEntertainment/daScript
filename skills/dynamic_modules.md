# Dynamic module descriptors (`.das_module`)

Read this before adding a module under `modules/` or shipping a package that others
`require`.

The default `daslang` binary is a DLL build: it resolves modules at startup by running
`modules/<name>/.das_module`, a small daslang script that maps require paths to files and
loads `.shared_module` DLLs. Without a descriptor entry, `require foo/bar` fails with
`error[20605] missing prerequisite 'foo/bar'` — even though the file compiles fine on its
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

## `register_native_path` — pure-das modules

```das
register_native_path(mod_name, from_path, to_path)
```

- `mod_name` — the part before the first `/` in the require path: `require foo/bar` → `"foo"`
- `from_path` — everything after it: `require foo/bar` → `"bar"`; `require foo/sub/bar` → `"sub/bar"`
- `to_path` — absolute path of the `.das` file

One call per `.das` file. For `require peg/peg`, `from_path` is `"peg"`, **not** `"peg/peg"`.

```das
[export]
def initialize(project_path : string) {
    for (path in ["peg", "meta_ast", "parse_macro"]) {
        register_native_path("peg", "{path}", "{project_path}/peg/{path}.das")
    }
}
```

## `register_dynamic_module` — C++ modules

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
- Guard with `das_is_dll_build()` — only the DLL binary loads DLLs
- One call per C++ module class; one descriptor may register several DLLs, and may mix in
  `register_native_path` calls for the module's `.das` files

## Package layout

A daspkg package is a module with the descriptor at the repo root:

```
my-package/
  .das_package        # metadata — skills/daspkg.md
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

## Adding a `.das` file to an existing module needs the same edit

Require-root registration is manifest-driven: a file not named in the module's
`.das_module` is unreachable. Add the name in the commit that adds the file.

## Debugging

- `error[20605] missing prerequisite 'foo/bar'` — the descriptor has no matching
  `register_native_path("foo", "bar", ...)`, or the DLL registration is missing
- `das_is_dll_build()` reports at runtime which binary is running
- `.shared_module` is the C++ DLL extension; `.das_module` is the descriptor script —
  they are different things

Static-binary module resolution and the CMake macros behind it: `skills/internal/dynamic_modules_plumbing.md` (repo-only).
