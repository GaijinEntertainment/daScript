# C++ Integration Patterns

These patterns are used in C++ host applications that embed daslang.

## Host program boilerplate

Every C++ host that runs daslang scripts follows this pattern:

```cpp
#include "daScript/daScript.h"
using namespace das;

void run_script() {
    TextPrinter tout;
    ModuleGroup dummyLibGroup;
    auto fAccess = make_smart<FsFileAccess>();
    auto program = compileDaScript(getDasRoot() + "/path/to/script.das",
                                   fAccess, tout, dummyLibGroup);
    if (program->failed()) { /* report errors */ return; }
    Context ctx(program->getContextStackSize());
    if (!program->simulate(ctx, tout)) { /* report errors */ return; }
    auto fn = ctx.findFunction("test");
    ctx.evalWithCatch(fn, nullptr);
}

int main(int, char * []) {
    NEED_ALL_DEFAULT_MODULES;
    NEED_MODULE(Module_MyModule);  // custom modules
    Module::Initialize();
    run_script();
    Module::Shutdown();
    return 0;
}
```

## Creating a module

Derive from `Module`, register types/functions/enums in the constructor, then use `REGISTER_MODULE`:

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

The host uses `NEED_MODULE(Module_MyMod)` before `Module::Initialize()`.  Scripts access it via `require my_module_name`.

> **Namespace caveat**: `NEED_MODULE` contains an `extern` declaration that binds to the enclosing C++ namespace.  If your initialization code lives inside a namespace, use the namespace-safe pair instead:
> - `DECLARE_MODULE(ClassName)` / `DECLARE_ALL_DEFAULT_MODULES` — at file scope (outside namespaces)
> - `PULL_MODULE(ClassName)` / `PULL_ALL_DEFAULT_MODULES` — inside the namespace
>
> See `tutorials/integration/cpp/22_namespace_integration.cpp` for a working example.

> **Dynamic binary support**: C++ modules also need a `.das_module` descriptor for the dynamic binary (`daslang`). See `skills/dynamic_modules.md` for the full pattern — you'll need `register_dynamic_module` to load the `.shared_module` DLL.

## Callbacks — `TBlock<>`, `TFunc<>`, `TLambda<>`, `das_invoke*`

Three closure types exist, each with a typed template and an invocation helper:

| Type | Template | Invocation | Lifetime |
|------|----------|------------|----------|
| Block | `TBlock<Ret, Args...>` | `das_invoke<Ret>::invoke(ctx, at, blk, args...)` | Stack-bound — valid only during the call |
| Func | `TFunc<Ret, Args...>` (or untyped `Func`) | `das_invoke_function<Ret>::invoke(ctx, at, fn, args...)` | Context-bound — storable |
| Lambda | `TLambda<Ret, Args...>` (or untyped `Lambda`) | `das_invoke_lambda<Ret>::invoke(ctx, at, lmb, args...)` | Heap-allocated — captures variables |

**Typed vs untyped**: `TBlock<int,int>` maps to `block<(arg:int):int>` in daslang — the compiler checks signatures. Untyped `Lambda` maps to `lambda<>` and will **not** match typed lambdas like `lambda<(x:int):int>`. Prefer typed templates.

**Block callback example**:

```cpp
void with_values(int32_t a, int32_t b,
                 const TBlock<void, int32_t, int32_t> & blk,
                 Context * context, LineInfoArg * at) {
    das_invoke<void>::invoke(context, at, blk, a, b);
}

addExtern<DAS_BIND_FUN(with_values)>(*this, lib, "with_values",
    SideEffects::invoke, "with_values")
        ->args({"a", "b", "blk", "context", "at"});
```

Use `SideEffects::invoke` for any function that invokes script callbacks.

In daslang: blocks follow function calls directly (parameterless: `defer() { ... }`, with params: `build_string() $(var writer) { ... }`), function pointers use `@@func_name`, lambdas use `@(args) { body }`.

## Calling daslang functions from C++ — `das_invoke_function`

The high-level `das_invoke_function<ReturnType>::invoke(ctx, at, fnPtr, arg1, arg2, ...)` handles argument marshalling automatically.  Preferred over raw `cast<>` + `evalWithCatch`.

## Binding C++ functions — `addExtern` + `DAS_BIND_FUN`

```cpp
addExtern<DAS_BIND_FUN(cpp_function)>(*this, lib, "das_name",
    SideEffects::none, "cpp_function")
        ->args({"param1", "param2"});
```

`SideEffects` flags: `none` (pure), `modifyExternal` (stdout/files), `modifyArgument` (mutates ref params), `modifyArgumentAndExternal` (mutates ref params AND has external side effects), `accessGlobal` (reads shared state), `invoke` (calls daslang), `worstDefault` (safe fallback).

**`modifyArgument` vs `modifyArgumentAndExternal`**: Use `modifyArgumentAndExternal` when a function mutates state reachable *through* an argument but the argument itself is a temporary (returned by value from a property). With plain `modifyArgument` the optimizer sees the temporary is unused after the call and may **eliminate the call entirely**. Classic example: `node.text` returns `xml_text` by value — calling `set(node.text, value)` with `modifyArgument` gets optimized away; `modifyArgumentAndExternal` prevents this.

## Binding C++ types — `MAKE_TYPE_FACTORY` + `ManagedStructureAnnotation`

1. **`MAKE_TYPE_FACTORY(DasName, CppType)`** at file scope — creates `typeFactory<CppType>` + `typeName<CppType>`
2. **`ManagedStructureAnnotation<T, canNew, canDelete>`** — describe fields with `addField<DAS_BIND_MANAGED_FIELD(member)>("name", "name")`
3. **`addAnnotation(make_smart<MyAnnotation>(lib))`** in the module — order matters: if type B contains type A, register A first
4. Functions returning bound types by value require **`SimNode_ExtFuncCallAndCopyOrMove`** template parameter in `addExtern`

**Handled types are reference types** — important consequences for scripts:
- Mutable local variables (`var`) of handled types require `unsafe` blocks
- Immutable locals (`let`) returned from factory functions work without `unsafe`
- **Factory function pattern**: provide `make_xxx()` functions returning by value so scripts can create instances ergonomically with `let x = make_xxx(...)` — no `unsafe` needed
- POD structs (no default member initializers, no virtual functions) work best with `ManagedStructureAnnotation`

## Binding C++ methods — `DAS_CALL_MEMBER` + `DAS_CALL_METHOD`

daslang has no member functions — "methods" are free functions where the first argument is `self`. Pipe syntax (`obj |> method()`) provides method-call ergonomics.

```cpp
// Step 1: Create wrapper aliases
using method_increment = DAS_CALL_MEMBER(Counter::increment);
using method_get       = DAS_CALL_MEMBER(Counter::get);

// Step 2: Register with addExtern
addExtern<DAS_CALL_METHOD(method_increment)>(*this, lib, "increment",
    SideEffects::modifyArgument,
    DAS_CALL_MEMBER_CPP(Counter::increment))
        ->args({"self"});

addExtern<DAS_CALL_METHOD(method_get)>(*this, lib, "get",
    SideEffects::none,
    DAS_CALL_MEMBER_CPP(Counter::get))
        ->args({"self"});
```

- **Non-const methods**: `SideEffects::modifyArgument` (they mutate the object)
- **Const methods**: `SideEffects::none`
- `DAS_CALL_MEMBER_CPP(Class::method)` provides the AOT-compatible name string

## Binding `std::shared_ptr<T>` — `Handle<T>` + `HandleRegistry`

Use this pattern when the C++ type is already owned by `std::shared_ptr` and you cannot or do not want to retrofit `ptr_ref_count`. This is how `modules/dasHV` exposes `WebSocketClient`, `WebSocketServer`, `HttpServer`, etc. See `tutorials/integration/cpp/23_handle_registry.cpp` for a self-contained example.

**Headers:**
```cpp
#include "daScript/misc/handle_registry.h"   // Handle<T>, HandleRegistry<T>
#include "daScript/ast/ast_handle.h"         // ManagedHandleAnnotation, addHandleAnnotation, cast<Handle<T>>
```

**File-scope `typeName<T>` — REQUIRED.** The leak-dump path reads `typeName<T>::name()` on the inner type. Without this you get `error C2027: use of undefined type 'das::typeName<T>'` at the `addHandleAnnotation` call site. Use `MAKE_TYPE_FACTORY(MyType, MyType)` for single-TU types, or `MAKE_EXTERNAL_TYPE_FACTORY` / `IMPLEMENT_EXTERNAL_TYPE_FACTORY` when split across header/cpp (dasHV uses the external pair).

**One-call registration:**
```cpp
MAKE_TYPE_FACTORY(MyType, MyType)     // at file scope

addHandleAnnotation<MyType>(this, lib, "MyType",
    "destroy_my_type",                // optional daslang destructor
    "das::Handle<MyType>");           // C++ name AOT emits into stubs
```
Registers the annotation **plus** `==`, `!=`, `is_alive`, and (if `destroyFnName` is non-empty) the release helper. Also installs a per-T leak-dump hook via `handleRegistry_registerDump` — `Module::Shutdown(bool dumpHandleLeaks = true)` calls `handleRegistry_dumpAll()` directly; pass `false` to suppress leak output (CI use).

**Factory (acquire) and method (lookup) patterns:**
```cpp
Handle<MyType> make_my_type(const char * n) {
    auto sp = std::make_shared<MyType>(n);
    return HandleRegistry<MyType>::instance().acquire(sp);
}
int my_type_do_thing(Handle<MyType> h, int arg) {
    auto p = HandleRegistry<MyType>::instance().lookup(h);
    if ( !p ) return -1;                       // stale / reused / null handle
    return p->do_thing(arg);
}
```
`lookup` returns an empty `shared_ptr` for null, stale, or slot-reused handles — the null-check is a **guaranteed** use-after-free guard (generation-checked).

**`HandleRegistry<T>::instance()`** is a per-`T` singleton shared across `daslang.exe` and every `dasModule*.shared_module` DLL — all modules see the same storage. The registry is thread-safe (internal `mutex`).

**`SideEffects` for bound methods — use `modifyExternal`, NOT `modifyArgument`.** `Handle<T>` is passed by value, so `modifyArgument` is rejected at module-registration time (`can't add function ... modify argument requires non-const ref argument`). The handle itself isn't mutated — the Actor/connection/whatever behind it is, which is external state.

**Script side — Handle<T> is a value, not a smart_ptr:**
- No `var inscope` required — plain `var h = make_my_type(...)`.
- `==`, `!=`, `is_alive(h)` all work directly.
- `destroy_my_type(h)` releases **only** the registry's `shared_ptr` — if another owner (your engine) holds a ref, the object survives; the handle becomes dead (`is_alive` → `false`).
- **No scope-based auto-release** — unlike `smart_ptr<T>`, a handle going out of scope does nothing. Every script-acquired handle must be explicitly destroyed, or it leaks and gets printed at shutdown.

**AOT:** no extra work. `cast<Handle<T>>` in `ast_handle.h` maps directly to `uint64_t`; AOT emits raw register passes. The `cppTypeName` argument is what AOT writes into the generated `.cpp`.

**Leak dump at shutdown:** registered automatically. Use `--no-dump-leaks` on the daslang CLI for CI runs that intentionally exit with live handles.

**When to pick which pattern:**

| | `smart_ptr<T>` (Tutorial 12) | `Handle<T>` (Tutorial 23) |
|---|---|---|
| C++ class | must inherit `ptr_ref_count` | unchanged |
| Primary owner | daslang | C++ engine |
| Per-copy cost | refcount bump | 64-bit value copy |
| UAF safety | refcount | generation check |
| Thread-safe registry | n/a | yes |
| Script-side syntax | `var inscope p <- ...` | `var h = ...` |

## Binding operators and properties

**Operators**: register functions with the operator symbol as the daslang name:

```cpp
addExtern<DAS_BIND_FUN(vec3_add), SimNode_ExtFuncCallAndCopyOrMove>(
    *this, lib, "+", SideEffects::none, "vec3_add")->args({"a", "b"});
// Unary: addExtern<...>(*this, lib, "-", ...)->args({"a"});
```

Available operator names: `+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `<`, `>`, `<=`, `>=`, `&`, `|`, `^`.

**Equality**: `addEquNeq<T>(*this, lib)` binds both `==` and `!=` (requires `operator==` and `operator!=` on T).

**Properties**: field-like access that calls C++ code under the hood.

*Member function property* — inside `ManagedStructureAnnotation`:

```cpp
addProperty<DAS_BIND_MANAGED_PROP(length)>("length", "length");
```

`DAS_BIND_MANAGED_PROP` only works with member functions (methods) of the managed type.

*Free function property* — via `addExtern` with `.\`` name prefix (in the module constructor):

```cpp
int get_content_length(const HttpResponse & resp) { return int(resp.content_length); }

// In module constructor, AFTER addAnnotation for the type:
addExtern<DAS_BIND_FUN(get_content_length)>(*this, lib, ".`content_length",
    SideEffects::none, "get_content_length")->args({"self"});
```

Naming a function `.\`prop_name` makes it callable with property syntax: `resp.content_length`. This is the preferred way to bind properties when the getter is a free function (not a method), or when the member returns a type that needs conversion (e.g. `size_t` → `int`).

## Binding C++ enums — `DAS_BASE_BIND_ENUM`

```cpp
// MUST come BEFORE `using namespace das` to avoid name collisions
DAS_BASE_BIND_ENUM(CppEnum, DasName, Value1, Value2, Value3)

using namespace das;

// In module constructor:
addEnumeration(new EnumerationDasName());
```

- `DAS_BASE_BIND_ENUM` creates class `EnumerationDasName` + `typeFactory<CppEnum>`
- `DAS_BIND_ENUM_CAST(CppEnum)` — explicit `cast<>` specialization (often not needed, SFINAE default suffices)
- `DAS_BASE_BIND_ENUM_98` — for unscoped (C-style) enums
- **Critical**: place enum macros BEFORE `using namespace das` — the macros define names inside `namespace das` that collide with global enum names
- **Name collision pitfall**: `das::LogLevel` is defined internally — do NOT name your enum `LogLevel` when `using namespace das`
- Manual construction alternative: `new Enumeration("Name")` + `pEnum->addIEx("Value", "CppEnum::Value", intValue, LineInfo())`

## Low-level interop — `addInterop`

`addInterop` binds a C++ function that receives raw simulation-level arguments (`vec4f *`), the call node (`SimNode_CallBase *`), and the context. Unlike `addExtern`, it supports **"any type" arguments** — when a template parameter is `vec4f`, it means the argument can be any daslang type. The function inspects `call->types[i]` (`TypeInfo *`) at runtime to determine what was actually passed.

**Signature**: the C++ function must match `InteropFunction`:

```cpp
// typedef vec4f (*InteropFunction)(Context &, SimNode_CallBase *, vec4f *);
vec4f my_interop(Context & context, SimNode_CallBase * call, vec4f * args) {
    TypeInfo * ti = call->types[0];   // type info for first argument
    // ... inspect ti->type, ti->structType, etc.
    return v_zero();
}
```

**Registration**:

```cpp
addInterop<my_interop, ReturnType, ArgType1, ArgType2>(
    *this, lib, "das_name", SideEffects::none, "my_interop");
```

Where `vec4f` as an `ArgType` means "any type" — the argument accepts any daslang value. Concrete types (e.g. `int32_t`, `const char *`, `const Block &`) are also valid and work like `addExtern`.

**Key capabilities** (vs `addExtern`):
- Access to `call->types[]` — per-argument `TypeInfo` with full type metadata
- Access to `call->debugInfo` — source location of the call site
- `vec4f` argument type = "any" — accept arguments of any daslang type
- Used internally for `sprint`, `hash`, `write`, `binary_save/load`, `invoke_in_context`

**TypeInfo union warning**: `TypeInfo` has a union — `structType`, `enumType`, and `annotation_or_name` share the same memory. Which member is valid depends on `ti->type`:
- `tStructure` → `ti->structType` (StructInfo *)
- `tEnumeration` / `tEnumeration8` / `tEnumeration16` → `ti->enumType` (EnumInfo *)
- `tHandle` → use `ti->getAnnotation()` (resolves tagged pointer safely)

Accessing the wrong union member is **undefined behavior**. `das_to_string(Type::tHandle)` returns an empty string — use `ti->getAnnotation()->name` for handled type names.

**Example** — `new_and_init` allocates and initializes any struct:

```cpp
vec4f new_and_init(Context & context, SimNode_CallBase * call, vec4f * args) {
    TypeInfo * typeInfo = call->types[0];
    if (typeInfo->type != Type::tStructure)
        context.throw_error_at(call->debugInfo, "expected struct");
    auto size = getTypeSize(typeInfo);
    auto data = context.allocate(size, &call->debugInfo);
    if (typeInfo->structType && typeInfo->structType->init_mnh) {
        auto fn = context.fnByMangledName(typeInfo->structType->init_mnh);
        context.callWithCopyOnReturn(fn, nullptr, data, 0);
    } else {
        memset(data, 0, size);
    }
    return cast<char *>::from(data);
}

addInterop<new_and_init, void *, vec4f>(*this, lib, "new_and_init",
    SideEffects::none, "new_and_init");
```

## C++ Codebase Notes

- Main type inference: `src/ast/ast_infer_type.cpp` (implementation) + `include/daScript/ast/ast_infer_type.h` (class declarations for `CaptureLambda` and `InferTypes`)
- Builtin runtime functions: `src/builtin/module_builtin_runtime.cpp`
- Smart pointer builtins: `move`, `move_new`, `smart_ptr_clone`, `smart_ptr_use_count`
- Compilation errors: `include/daScript/ast/compilation_errors.h` (error codes 10001–40214)
- Lexer: `src/parser/ds2_lexer.lpp`
- Parser: `src/parser/ds2_parser.ypp`

## Module::aotRequire() — AOT Header Declarations

When a C++ module binds functions whose declarations live in specific headers, it must override `aotRequire()` so AOT-generated C++ can find those declarations:

```cpp
virtual ModuleAotType aotRequire(TextWriter & tw) const override {
    tw << "#include \"daScript/simulate/bin_serializer.h\"\n";
    tw << "#include \"daScript/misc/performance_time.h\"\n";
    return ModuleAotType::cpp;
}
```

**If you add `extern "C"` or `extern` functions via `addExtern` and the declaration lives in a header, you MUST add that header to `aotRequire()`**. Otherwise, AOT-generated C++ will fail to compile with "undeclared identifier" errors.

Real example: `Module_BuiltIn::addTime()` binds `ref_time_ticks`, `get_time_usec`, `get_time_nsec` (declared in `performance_time.h`). The `aotRequire()` must emit `#include "daScript/misc/performance_time.h"` or AOT compilation of any script using these functions will fail.

See `skills/aot_testing.md` for the full AOT pipeline and testing infrastructure.

### Key AST function flags

- `func.flags.isClassMethod` — function is a struct/class method (set after inference)
- `func.moreFlags.isStaticClassMethod` — static method (declared with `def static`); name is `StructName\`methodName`, self is explicit first argument
- Non-static methods (`isClassMethod=true`, `isStaticClassMethod=false`) — self is added implicitly during inference; name stays unqualified (e.g. `finalize`, `[]`)
- `func.moreFlags.propertyFunction` — property accessor (name starts with `.\``)
- `func.classParent` — pointer to the struct/class that owns the method

## Diagnostic output: `TextPrinter`, never `fprintf(stderr, ...)`

Use `TextPrinter` from `include/daScript/misc/string_writer.h` for any
diagnostic, log, or leak-dump output — including temporary debug prints.

```cpp
#include "daScript/misc/string_writer.h"
TextPrinter tp;
tp << "leaked " << count << " handles\n";
```

`fprintf(stderr, ...)` has three concrete problems:

- **No sink override** — `TextPrinter` can be subclassed/redirected;
  `fprintf` writes wherever the OS `stderr` happens to point.
- **`stderr` is not available on consoles** (Switch, PlayStation, Xbox)
  — `TextPrinter` abstracts the output path so platform ports can route
  the text.
- **Platform divergence** in general — `TextPrinter` hides the
  differences.

Applies to permanent diagnostics AND temporary debug prints — don't
leave `fprintf` scaffolding in the tree even if you plan to remove it.
Canonical patterns: `src/misc/job_que.cpp` (`JobStatus::DumpJobQueLeaks`)
and `include/daScript/ast/ast_handle.h` (`dumpHandleLeaks<T>`).

## C-string builtins: guard `!str || !*str`

In daslang, the empty string `""` is represented as `nullptr` at
runtime. The interpreter always passes `null` for empty strings, but
when a function is **AOT-compiled and constant-folded**, the C++ AOT
output may pass an actual `""` (pointer to `'\0'`) instead of `null`.

Any C++ builtin that takes `const char *` must therefore guard:

```cpp
// Wrong — only catches the interpreter case
const char * find(const char * str, const char * needle) {
    if (!str || !needle) return nullptr;
    ...
}

// Right — also catches AOT-folded empty strings
const char * find(const char * str, const char * needle) {
    if (!str || !*str || !needle || !*needle) return nullptr;
    ...
}
```

Real bug: `strstr("","")` returned `0` (match) instead of `-1` (not
found), changing semantics between interpreter and AOT. Diagnosed by
diffing `options log_nodes` output between `daslang.exe` and
`test_aot.exe` — the constant-folded values differed.

Highest-risk surface: 2-arg overloads (no `Context*` parameter) used in
constant-foldable expressions — `find`, `rfind`, `contains`, etc.

## Searching C++ — prefer `mcp__cplusplus__*` over Bash/Grep

For navigating C++ in this repo, default to the C++ MCP tools, not
`Bash grep` / `rg` / the Grep tool. The MCP server builds a real C++
index (~9k files) that resolves symbols, call graphs, class hierarchies,
and overrides — plain text search misses overloads, virtual dispatch,
macro-defined names, and cross-file relationships.

- **First call of a session:** `mcp__cplusplus__set_project_directory`
  with `d:\Work\daScript` (required before any other C++ MCP call).
- **Refresh on demand:** `mcp__cplusplus__refresh_project` after
  significant edits or when switching branches.
- **Tool picks:**
  - `search_symbols` / `search_functions` / `search_classes` — find by name
  - `find_callers` / `find_callees` / `get_call_path` — call graphs
  - `get_class_info` / `get_class_hierarchy` / `get_derived_classes` — OOP structure
  - `get_function_signature` — exact signature including overloads
  - `find_in_file` — scoped, symbol-aware file search
- **Fall back to `Grep` only for:** string literals, comments, non-C++
  files (CMake, shell, docs), or when the index is stale and refresh
  is too slow.
