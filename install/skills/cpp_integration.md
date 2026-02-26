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

`SideEffects` flags: `none` (pure), `modifyExternal` (stdout/files), `modifyArgument` (mutates ref params), `accessGlobal` (reads shared state), `invoke` (calls daslang), `worstDefault` (safe fallback).

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

## Binding operators and properties

**Operators**: register functions with the operator symbol as the daslang name:

```cpp
addExtern<DAS_BIND_FUN(vec3_add), SimNode_ExtFuncCallAndCopyOrMove>(
    *this, lib, "+", SideEffects::none, "vec3_add")->args({"a", "b"});
// Unary: addExtern<...>(*this, lib, "-", ...)->args({"a"});
```

Available operator names: `+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `<`, `>`, `<=`, `>=`, `&`, `|`, `^`.

**Equality**: `addEquNeq<T>(*this, lib)` binds both `==` and `!=` (requires `operator==` and `operator!=` on T).

**Properties**: method calls disguised as field access in `ManagedStructureAnnotation`:

```cpp
addProperty<DAS_BIND_MANAGED_PROP(length)>("length", "length");
```

In daslang, `v.length` calls `Vec3::length()` — looks like a field, calls a method.

## Binding C++ enums — `DAS_BASE_BIND_ENUM`

```cpp
// MUST come BEFORE `using namespace das` to avoid name collisions
DAS_BASE_BIND_ENUM(CppEnum, DasName, Value1, Value2, Value3)

using namespace das;

// In module constructor:
addEnumeration(make_smart<EnumerationDasName>());
```

- `DAS_BASE_BIND_ENUM` creates class `EnumerationDasName` + `typeFactory<CppEnum>`
- `DAS_BIND_ENUM_CAST(CppEnum)` — explicit `cast<>` specialization (often not needed, SFINAE default suffices)
- `DAS_BASE_BIND_ENUM_98` — for unscoped (C-style) enums
- **Critical**: place enum macros BEFORE `using namespace das` — the macros define names inside `namespace das` that collide with global enum names
- **Name collision pitfall**: `das::LogLevel` is defined internally — do NOT name your enum `LogLevel` when `using namespace das`
- Manual construction alternative: `make_smart<Enumeration>("Name")` + `pEnum->addIEx("Value", "CppEnum::Value", intValue, LineInfo())`

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
