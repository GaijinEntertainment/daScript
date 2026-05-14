---
slug: why-is-def-float-rejected-but-def-string-and-def-float2-allowed
title: Why is `def float(...)` rejected but `def string(...)` and `def float2(...)` allowed?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

The asymmetry is **not** in the parser. The grammar at `src/parser/ds2_parser.ypp:1316-1408` says:

```
function_name
    :   NAME                  // user identifiers
    |   DAS_OPERATOR ...       // operator overloads
    |   das_type_name          // any type-name TOKEN
    ;
```

`das_type_name` accepts every type-keyword token: `DAS_TBOOL`, `DAS_TSTRING`, `DAS_TINT`, `DAS_TINT2`, `DAS_TFLOAT`, `DAS_TFLOAT2`, `DAS_TDOUBLE`, etc. So at the parser level `def float`, `def int`, `def string`, `def float2`, `def double` all parse identically.

**The rejection is a post-parse lint guard at `src/ast/ast_lint.cpp:867-884`** (pre-2026-05-14):

```cpp
bool isValidFunctionName(const string & str) const {
    return !isCppKeyword(str.c_str());
}
virtual void preVisit ( Function * fn ) override {
    if (!isValidFunctionName(fn->name))
        program->error("invalid function name " + fn->name, ..., CompilationError::invalid_function_name);
    ...
}
```

`isCppKeyword` (`src/builtin/module_builtin_ast.cpp:68-84`) is a hardcoded **C++**-keyword set. That set contains `float`, `int`, `double`, `bool`, `char`, `void`, `do`, `class`, `new`, `delete`, ... It does NOT contain `string` (that's `std::string` in C++ — qualified, not a keyword), nor `float2`/`int2`/`uint3`/etc. (daslang-specific typedefs, not C++ keywords). That's exactly the observed asymmetry.

The lint guard existed because the AOT C++ emitter would otherwise generate a literal `float(...)` C++ function which wouldn't compile.

**Status (2026-05-14, PR landed):** the function-name guard was dropped. `daslib/aot_cpp.das:223-267` (`aotSuffixNameEx`) now detects C++ keywords and applies the same `_Func` prefix it already used for operator-character names — so `def float` AOTs to `_Funcfloat_<hash>` (a valid C++ identifier). The other 5 `isCppKeyword` lint sites stay (module/enum/enum-value/struct/struct-field names) — those emit 1:1 into C++ and need the guard.

The 6 callsites that *still* reject C++ keywords (post-PR):
- `ast_lint.cpp:253` module name → error 30210
- `ast_lint.cpp:263` enumeration name → 30146
- `ast_lint.cpp:266` enumeration value → 30148
- `ast_lint.cpp:307` structure name → 30240
- `ast_lint.cpp:349` struct field / variable / arg name → 30152 / 30282 / 30106

`is_cpp_keyword(name : string) : bool` is exposed to daslang via `module_builtin_ast.cpp:1572-1573` if you ever need to call it from a macro or AOT emitter.

## Questions
- Why does `def float(...)` give error 30163 "invalid function name" but `def string(...)` doesn't?
- Where does daslang reject function names that are C++ keywords?
- Is the parser the gate for `def <type>` overloads, or is it the lint pass?
- Can I define a daslang function called `int` or `double`?
- What error code is "invalid function name" and where is it emitted?
