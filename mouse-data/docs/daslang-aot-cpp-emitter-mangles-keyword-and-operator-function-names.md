---
slug: daslang-aot-cpp-emitter-mangles-keyword-and-operator-function-names
title: How does the daslang AOT C++ emitter mangle function names — for operator overloads and C++ keywords?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

The AOT C++ emitter lives in `daslib/aot_cpp.das` (NOT the emptied `src/ast/ast_aot_cpp.cpp`).

**The function-name mangler is `aotFuncName`** at `daslib/aot_cpp.das:928-935`:

```daslang
def public aotFuncName(func : Function?) {
    if (func.hash != uint64(0)) {
        return "{aotSuffixNameEx(func.name,"_Func")}_{func.hash:x}";
    } else {
        return "{aotSuffixNameEx(func.name,"_Func")}";
    }
}
```

All daslang→C++ call sites and def sites go through this helper (verified: `daslib/aot_cpp.das:1084` def site, `:2950/:2966/:3459/:3785` call sites). No bypass paths.

**`aotSuffixNameEx`** (line 223-267) is where the rewriting happens:

```daslang
def aotSuffixNameEx(funcName : das_string; suffix : string) {
    var prefix = is_cpp_keyword(string(funcName));   // <-- added 2026-05-14
    let name = build_string() $(var writer) {
        for (ch in string(funcName)) {
            if (is_alnum(ch) || ch == '_') {
                writer |> write_char(ch);
            } else {
                prefix = true;
                var t_ch : string;
                match (ch) {
                    if ('=') { t_ch = "Equ"; }
                    if ('+') { t_ch = "Add"; }
                    if ('-') { t_ch = "Sub"; }
                    if ('*') { t_ch = "Mul"; }
                    if ('/') { t_ch = "Div"; }
                    if ('%') { t_ch = "Mod"; }
                    if ('&') { t_ch = "And"; }
                    if ('|') { t_ch = "Or"; }
                    if ('^') { t_ch = "Xor"; }
                    if ('?') { t_ch = "Qmark"; }
                    if ('~') { t_ch = "Tilda"; }
                    if ('!') { t_ch = "Excl"; }
                    if ('>') { t_ch = "Greater"; }
                    if ('<') { t_ch = "Less"; }
                    if ('[') { t_ch = "Sqbl"; }
                    if (']') { t_ch = "Sqbr"; }
                    if ('.') { t_ch = "Dot"; }
                    if ('`') { t_ch = "Tick"; }
                    if (',') { t_ch = "Comma"; }
                    if (_) { t_ch = "_0x{hex(ch)}_" }    // unmapped chars → hex escape
                }
                write(writer, "{t_ch}")
            }
        }
    }
    return prefix ? (suffix + name) : name;    // suffix prepended (yes, prepended) iff prefix flag set
}
```

The `prefix` boolean controls whether the `suffix` arg ("_Func" for functions, "" for structs via `aotStructName`) is *prepended* to the mangled body. The flag fires in two conditions:

1. **Any non-alnum char in the name** (operator overloads, backtick-encoded names): the substitution loop sets `prefix = true` as a side effect.
2. **The name is a C++ keyword** (since 2026-05-14): `is_cpp_keyword(name)` seeds the flag.

**Example emissions:**

| daslang | C++ name |
|---|---|
| `def foo()` | `foo_<hash>` (no prefix — alnum, not keyword) |
| `def operator+()` | `_FuncAdd_<hash>` |
| `def \`testing\`equal\``() | `_FunctestingTickequalTick_<hash>` |
| `def float()` | `_Funcfloat_<hash>` (was rejected by lint pre-2026-05-14) |
| `def do()` | `_Funcdo_<hash>` |

**`aotStructName`** (line 269-271) uses the same helper with `suffix=""` — struct names need no prefix because the C++-keyword-name lint guard at `ast_lint.cpp:307` is still in force (relaxing struct names would need a different emit strategy, since struct names embed 1:1 into `struct Foo { ... }` syntax).

**`is_cpp_keyword(name : string) : bool`** is the daslang binding of C++ `isCppKeyword` (`src/builtin/module_builtin_ast.cpp:1572-1573`).

## Questions
- How does the daslang AOT emitter handle C++ keyword names like `def float`?
- How are operator overloads (`def +`, `def ==`) mangled in AOT C++ output?
- Where in daslang is the AOT C++ function-name mangler?
- What's `aotSuffixNameEx` and when does it prepend the `_Func` prefix?
- What does `_FuncAdd_<hash>` mean in generated AOT C++?
- Is there a way to find an AOT-emitted C++ symbol from a daslang function name?
- Can I call `is_cpp_keyword` from a daslang macro?
