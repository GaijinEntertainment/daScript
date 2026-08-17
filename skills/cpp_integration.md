# Embedding daslang in C++

Read this before writing a C++ host that runs daslang scripts, or a C++ module that binds
types and functions into daslang. Working examples of every pattern here live under
`tutorials/integration/cpp/`. Working on the daslang compiler itself instead:
`skills/internal/cpp_codebase_notes.md` (repo-only).

## Host boilerplate

```cpp
#include "daScript/daScript.h"
using namespace das;

void run_script() {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + "/path/to/script.das",
                                   fAccess, tout, dummyLibGroup);
    if (program->failed()) { /* report program->errors */ return; }
    Context ctx(program->getContextStackSize());
    if (!program->simulate(ctx, tout)) { /* report errors */ return; }
    ctx.evalWithCatch(ctx.findFunction("test"), nullptr);
}

int main(int, char * []) {
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_MyModule);
    Module::Initialize();
    run_script();
    Module::Shutdown();
    return 0;
}
```

`Module::Shutdown(bool dumpHandleLeaks = true)` prints live `Handle<T>` leaks; pass
`false` to suppress (CI runs that exit with live handles).

## Creating a module

```cpp
class Module_MyMod : public Module {
public:
    Module_MyMod() : Module("my_module_name") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        // addAnnotation, addExtern, addEnumeration, addConstant ...
    }
};
REGISTER_MODULE(Module_MyMod);
```

Scripts reach it with `require my_module_name`; the host pulls it with
`NEED_MODULE(Module_MyMod)` before `Module::Initialize()`.

`NEED_MODULE` declares an `extern` that binds to the enclosing C++ namespace. From inside
a namespace use the namespace-safe pair instead: `DECLARE_MODULE(ClassName)` /
`DECLARE_ALL_DEFAULT_MODULES` at file scope, `PULL_MODULE(ClassName)` /
`PULL_ALL_DEFAULT_MODULES` inside the namespace (`tutorials/integration/cpp/22_namespace_integration.cpp`).

A C++ module also needs a `.das_module` descriptor to load into the DLL binary —
`skills/dynamic_modules.md`.

## Binding functions — `addExtern` + `DAS_BIND_FUN`

```cpp
addExtern<DAS_BIND_FUN(cpp_function)>(*this, lib, "das_name",
    SideEffects::none, "cpp_function")
        ->args({"param1", "param2"});
```

`SideEffects`: `none` (pure), `modifyExternal` (stdout/files), `modifyArgument` (mutates
ref params), `modifyArgumentAndExternal`, `accessGlobal`, `invoke` (calls back into
daslang), `worstDefault`.

**`modifyArgument` on a temporary loses the call.** When a function mutates state reachable
*through* an argument that is itself a temporary — `set(node.text, value)` where `.text`
returns by value — the optimizer sees the temporary is dead after the call and eliminates
the call. Declare `modifyArgumentAndExternal`.

## Binding types — `MAKE_TYPE_FACTORY` + `ManagedStructureAnnotation`

1. `MAKE_TYPE_FACTORY(DasName, CppType)` at file scope
2. `ManagedStructureAnnotation<T, canNew, canDelete>` with
   `addField<DAS_BIND_MANAGED_FIELD(member)>("name", "name")`
3. `addAnnotation(make_smart<MyAnnotation>(lib))` — register a contained type before the
   type that contains it
4. A function returning a bound type by value needs the `SimNode_ExtFuncCallAndCopyOrMove`
   template argument on `addExtern`

Handled types are **reference types**, which the script side feels: a mutable local (`var`)
of a handled type needs `unsafe`, an immutable `let` returned from a factory does not.
Provide `make_xxx()` factories returning by value so scripts write `let x = make_xxx(...)`.
POD structs (no default member initializers, no virtuals) bind best.

## Binding methods — `DAS_CALL_MEMBER` + `DAS_CALL_METHOD`

daslang has no member functions; a method is a free function whose first argument is
`self`, called with pipe syntax (`obj |> method()`).

```cpp
using method_increment = DAS_CALL_MEMBER(Counter::increment);

addExtern<DAS_CALL_METHOD(method_increment)>(*this, lib, "increment",
    SideEffects::modifyArgument,
    DAS_CALL_MEMBER_CPP(Counter::increment))
        ->args({"self"});
```

Non-const methods take `modifyArgument`, const methods `none`.
`DAS_CALL_MEMBER_CPP` supplies the AOT-compatible name string.

## Callbacks — `TBlock`, `TFunc`, `TLambda`

| Type | Template | Invocation | Lifetime |
|---|---|---|---|
| Block | `TBlock<Ret, Args...>` | `das_invoke<Ret>::invoke(ctx, at, blk, args...)` | stack-bound, valid only during the call |
| Func | `TFunc<Ret, Args...>` | `das_invoke_function<Ret>::invoke(ctx, at, fn, args...)` | context-bound, storable |
| Lambda | `TLambda<Ret, Args...>` | `das_invoke_lambda<Ret>::invoke(ctx, at, lmb, args...)` | heap-allocated, captures |

```cpp
void with_values(int32_t a, int32_t b,
                 const TBlock<void, int32_t, int32_t> & blk,
                 Context * context, LineInfoArg * at) {
    das_invoke<void>::invoke(context, at, blk, a, b);
}

addExtern<DAS_BIND_FUN(with_values)>(*this, lib, "with_values",
    SideEffects::invoke, "with_values")->args({"a", "b", "blk", "context", "at"});
```

Prefer the typed templates: `TBlock<int,int>` maps to `block<(arg:int):int>` and is
signature-checked, while untyped `Lambda` maps to `lambda<>` and will **not** match a
typed `lambda<(x:int):int>`. Anything that invokes a script callback takes
`SideEffects::invoke`.

Calling into daslang from C++ goes through the same helpers —
`das_invoke_function<Ret>::invoke(ctx, at, fnPtr, args...)` marshals arguments for you and
is preferred over raw `cast<>` plus `evalWithCatch`.

## Binding `std::shared_ptr<T>` — `Handle<T>` + `HandleRegistry`

Use this when the C++ type is already owned by `std::shared_ptr` and cannot be retrofitted
with `ptr_ref_count`. Self-contained example: `tutorials/integration/cpp/23_handle_registry.cpp`.

```cpp
#include "daScript/misc/handle_registry.h"   // Handle<T>, HandleRegistry<T>
#include "daScript/ast/ast_handle.h"         // addHandleAnnotation, cast<Handle<T>>

MAKE_TYPE_FACTORY(MyType, MyType)            // file scope — REQUIRED

addHandleAnnotation<MyType>(this, lib, "MyType",
    "destroy_my_type",                       // optional daslang destructor
    "das::Handle<MyType>");                  // C++ name AOT emits into stubs
```

`typeName<T>` must exist at file scope — the leak dump reads it, and without it
`addHandleAnnotation` fails to compile (`use of undefined type 'das::typeName<T>'`). Use
`MAKE_TYPE_FACTORY` for a single TU, or the
`MAKE_EXTERNAL_TYPE_FACTORY` / `IMPLEMENT_EXTERNAL_TYPE_FACTORY` pair across header and cpp.

One `addHandleAnnotation` call registers the annotation plus `==`, `!=`, `is_alive`, the
release helper, and the per-`T` leak-dump and live-count hooks.

```cpp
Handle<MyType> make_my_type(const char * n) {
    return HandleRegistry<MyType>::instance().acquire(std::make_shared<MyType>(n));
}
int my_type_do_thing(Handle<MyType> h, int arg) {
    auto p = HandleRegistry<MyType>::instance().lookup(h);
    if ( !p ) return -1;                     // null, stale, or slot-reused handle
    return p->do_thing(arg);
}
```

`lookup` returns an empty `shared_ptr` for a null, stale, or reused handle — the null check
is a generation-checked use-after-free guard. `instance()` is one per-`T` singleton shared
across the executable and every `.shared_module`, and is mutex-guarded.

Bound methods take `SideEffects::modifyExternal`, **not** `modifyArgument`: `Handle<T>` is
passed by value, so `modifyArgument` is rejected at registration
(`modify argument requires non-const ref argument`), and the mutated state is the object
behind the handle anyway.

Script side, a handle is a plain value: `var h = make_my_type(...)`, no `var inscope`,
`==` / `!=` / `is_alive(h)` all work. There is **no scope-based release** — every acquired
handle must be destroyed explicitly or it leaks and is printed at shutdown
(`--no-dump-leaks` on the CLI suppresses that for intentional cases). AOT needs no extra
work: `cast<Handle<T>>` maps to `uint64_t`.

| | `smart_ptr<T>` | `Handle<T>` |
|---|---|---|
| C++ class | must inherit `ptr_ref_count` | unchanged |
| Primary owner | daslang | C++ |
| Per-copy cost | refcount bump | 64-bit copy |
| Use-after-free safety | refcount | generation check |
| Script syntax | `var inscope p <- ...` | `var h = ...` |

## Operators and properties

Operators bind by name — register a function whose daslang name is the symbol:

```cpp
addExtern<DAS_BIND_FUN(vec3_add), SimNode_ExtFuncCallAndCopyOrMove>(
    *this, lib, "+", SideEffects::none, "vec3_add")->args({"a", "b"});
```

Bindable: `+ - * / % << >> < > <= >= & | ^ ~ ! && ||`, the compound-assign family, `++`,
`--`, `[]`, and the special names `clone` and `finalize`. There is no whitelist — only the
characters are validated, so any punctuation name the parser accepts binds.
`addEquNeq<T>(*this, lib)` binds `==` and `!=` together.

Properties are field-like access backed by C++. Inside a `ManagedStructureAnnotation`, for
a member function: `addProperty<DAS_BIND_MANAGED_PROP(length)>("length", "length")` (use
`addPropertyExtConst<...>` when const and non-const accessors differ — see
`tutorials/integration/cpp/09_operators_and_properties.cpp`). For a free function, name it
with the `` .` `` prefix:

```cpp
int get_content_length(const HttpResponse & resp) { return int(resp.content_length); }

// after addAnnotation for the type:
addExtern<DAS_BIND_FUN(get_content_length)>(*this, lib, ".`content_length",
    SideEffects::none, "get_content_length")->args({"self"});
```

`resp.content_length` then calls it. This is the way to bind a property whose getter is a
free function, or whose member needs a conversion (`size_t` → `int`).

## Binding enums — `DAS_BASE_BIND_ENUM`

```cpp
// MUST come BEFORE `using namespace das`
DAS_BASE_BIND_ENUM(CppEnum, DasName, Value1, Value2, Value3)

using namespace das;

addEnumeration(new EnumerationDasName());    // in the module constructor
```

The macro defines names inside `namespace das`, which collide with global enum names once
`using namespace das` is in effect — hence the ordering rule. `das::LogLevel` already
exists, so never name an enum `LogLevel`. Unscoped C-style enums use
`DAS_BASE_BIND_ENUM_98`; `DAS_BIND_ENUM_CAST(CppEnum)` adds an explicit `cast<>`
specialization when the SFINAE default is not enough. Manual alternative:
`new Enumeration("Name")` plus `pEnum->addIEx("Value", "CppEnum::Value", intValue, LineInfo())`.

## Low-level interop — `addInterop`

`addInterop` binds a function that receives raw simulation arguments plus the call node, so
it can inspect what was actually passed. Its distinguishing power is the **"any type"
argument**: a `vec4f` template parameter accepts any daslang type, and the function reads
`call->types[i]` to find out which. Concrete types work too, like `addExtern`. Used
internally for `sprint`, `hash`, `write`, `binary_save`/`load`, `invoke_in_context`.

```cpp
vec4f my_interop(Context & context, SimNode_CallBase * call, vec4f * args) {
    TypeInfo * ti = call->types[0];          // also: call->debugInfo = call site
    return v_zero();
}

addInterop<my_interop, ReturnType, ArgType1, ArgType2>(
    *this, lib, "das_name", SideEffects::none, "my_interop");
```

**`TypeInfo` holds a union** — `structType`, `enumType` and `annotation_info` share
memory, and reading the wrong one is undefined behavior. Dispatch on `ti->type`:
`tStructure` → `ti->structType`, `tEnumeration`/`tEnumeration8`/`tEnumeration16` →
`ti->enumType`, `tHandle` → `ti->getAnnotation()` (resolves and caches the live
`TypeAnnotation`; `das_to_string(Type::tHandle)` is empty, so handled type names come from
`ti->getAnnotation()->name`).

## `aotRequire()` — headers for AOT output

When a module binds functions declared in specific headers, AOT-generated C++ must include
them, or it fails to compile with "undeclared identifier":

```cpp
virtual ModuleAotType aotRequire(TextWriter & tw) const override {
    tw << "#include \"daScript/misc/performance_time.h\"\n";
    return ModuleAotType::cpp;
}
```

Generate stubs with `bin/daslang -aot input.das output.cpp`, adding `-aot-macros` when the
script defines macros. `error[50101]: AOT link failed on <fn>` means the recorded hash no
longer matches the source — regenerate and rebuild.

## Diagnostics — `TextPrinter`, never `fprintf(stderr, ...)`

```cpp
#include "daScript/misc/string_writer.h"
TextPrinter tp;
tp << "leaked " << count << " handles\n";
```

Every `TextPrinter` writes through one process-wide sink (`setTextPrinterSink`, stdout by
default), so a program whose stdout carries a protocol redirects all diagnostics at once
with `textPrinterToStderr()` (das: `diagnostics_to_stderr()`) or `textPrinterToFile(path)`
(das: `diagnostics_to_file`). `fprintf` writes wherever the OS `stderr` points, and on
consoles (Switch, PlayStation, Xbox) there is no `stderr` at all. The rule covers temporary
debug prints too — do not leave `fprintf` scaffolding in the tree.

## C-string builtins — guard `!str || !*str`

At runtime the empty string is `nullptr`, and the interpreter always passes `null`. But a
constant-folded AOT call may pass a real `""` instead, so a null-only guard changes
semantics between tiers:

```cpp
const char * find(const char * str, const char * needle) {
    if (!str || !*str || !needle || !*needle) return nullptr;
    ...
}
```

Real bug: `strstr("","")` returned a match under AOT and no-match under the interpreter.
Highest risk is the 2-argument overloads (no `Context*`) that participate in constant
folding — `find`, `rfind`, `contains`.
