# Dynamic Module Descriptors (`.das_module`)

The daslang SDK binary (`bin/daslang`) uses `.das_module` descriptor scripts to locate modules under `modules/`. Each module directory has a `.das_module` file that registers the module's components at startup.

## How module resolution works

For a `require foo/bar` statement:
1. Check if `foo` == `"daslib"` → resolve from `daslib/` directory
2. Try `g_dyn_modules_resolve` entries (populated by `.das_module` scripts)
3. Try other fallbacks

## `.das_module` file format

Every `.das_module` is a daslang script with an `initialize` export:

```das
options gen2
require fio

[export]
def initialize(project_path : string) {
    // register modules here
}
```

- `project_path` — absolute path to the module directory (e.g. `/path/to/modules/dasFoo`)
- The file MUST be named `.das_module` (dot-prefixed, no other name)
- The file lives at `modules/<name>/.das_module`

## Two registration functions

### `register_dynamic_module` — for C++ modules with shared libraries

Used when the module has C++ code compiled into a `.shared_module` DLL:

```das
[export]
def initialize(project_path : string) {
    if (das_is_dll_build()) {
        register_dynamic_module("{project_path}/dasModuleFoo.shared_module", "Module_Foo")
    }
}
```

- First arg: path to the `.shared_module` file
- Second arg: C++ module class name (must match `REGISTER_MODULE(Module_Foo)` in C++)
- Guard with `das_is_dll_build()` — only needed in the dynamic binary
- One call per C++ module class; a single DLL can contain multiple module classes

### `register_native_path` — for pure-das modules (no C++ component)

Used when the module is implemented entirely in `.das` files:

```das
[export]
def initialize(project_path : string) {
    register_native_path("peg", "peg", "{project_path}/peg/peg.das")
}
```

- First arg (`mod_name`): the top-level module name — the part before `/` in `require foo/bar` → `"foo"`
- Second arg (`from_path`): the part after the first `/` in the require path — for `require foo/bar` → `"bar"`. For `require foo/sub/bar` → `"sub/bar"`
- Third arg (`to_path`): absolute filesystem path to the `.das` file

**Critical**: `from_path` must match the suffix after the first `/`. For `require peg/peg`, that's `"peg"`, NOT `"peg/peg"`.

## Examples

### C++ module with one class

```das
options gen2
require fio

[export]
def initialize(project_path : string) {
    if (das_is_dll_build()) {
        register_dynamic_module("{project_path}/dasModuleUnitTest.shared_module", "Module_UnitTest")
    }
}
```

### C++ module with multiple classes

```das
options gen2
require fio

[export]
def initialize(project_path : string) {
    if (das_is_dll_build()) {
        register_dynamic_module("{project_path}/dasModuleImgui.shared_module", "Module_dasIMGUI")
        register_dynamic_module("{project_path}/dasModuleImgui.shared_module", "Module_dasIMGUI_NODE_EDITOR")
        register_dynamic_module("{project_path}/imguiApp.shared_module", "Module_imgui_app")
    }
}
```

### Pure-das module with multiple files

```das
options gen2
require fio

[export]
def initialize(project_path : string) {
    let peg_paths = ["peg", "meta_ast", "parse_macro", "parser_generator"]
    for (path in peg_paths) {
        register_native_path("peg", "{path}", "{project_path}/peg/{path}.das")
    }
}
```

### Mixed module (C++ + das paths)

```das
options gen2
require fio

[export]
def initialize(project_path : string) {
    let daslib_paths = [
        "llvm_boost", "llvm_debug", "llvm_jit", "llvm_targets",
        "llvm_jit_intrin", "llvm_jit_common", "llvm_dll_utils"
    ]
    let bindings_paths = [
        "llvm_const", "llvm_enum", "llvm_func", "llvm_struct",
    ]
    for (path in daslib_paths) {
        register_native_path("llvm", "daslib/{path}", "{project_path}/daslib/{path}.das")
    }
    for (path in bindings_paths) {
        register_native_path("llvm", "bindings/{path}", "{project_path}/bindings/{path}.das")
    }
}
```

## When to create a `.das_module`

| Module type | Has `.das_module`? | Registration calls |
|---|---|---|
| C++ module (`.shared_module` DLL) | Yes | `register_dynamic_module` for each C++ module class |
| Pure-das module | Yes | `register_native_path` for each `.das` file |
| Mixed (C++ + das files) | Yes | Both `register_dynamic_module` and `register_native_path` |
| daslib module (`daslib/*.das`) | No | Resolved via `daslib/` directory convention |

## Debugging module resolution

- If `require foo/bar` fails with "missing prerequisite", check that `modules/dasFoo/.das_module` exists and contains a matching `register_native_path("foo", "bar", ...)` or `register_dynamic_module(...)` call
- Use `das_is_dll_build()` to check at runtime whether running in the dynamic binary
- The `.shared_module` extension is for C++ module DLLs — NOT `.das_module` (that's the descriptor script)

## daspkg packages

Standalone packages (managed by `daspkg`) use the same `.das_module` mechanism. See `skills/daspkg.md` for the package-specific layout and `.das_module` patterns.
