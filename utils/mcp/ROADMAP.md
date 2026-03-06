# MCP Server Roadmap

Future tools for the daslang MCP server, organized by priority and difficulty.

## Current Tools (v0.1)

| Tool | Description |
|---|---|
| `compile_check` | Compile a file, return errors or success + function list |
| `list_functions` | List all functions after macro expansion |
| `list_types` | List structs, classes, enums, type aliases |
| `list_requires` | List direct and transitive require dependencies |
| `list_modules` | List all available modules (builtin + daslib) |
| `list_module_api` | List functions, types, enums, globals exported by a module |
| `find_symbol` | Cross-module symbol search by substring |
| `ast_dump` | Dump AST of expression or function (S-expression or source mode) |
| `run_script` | Run inline code or a .das file, capture stdout |
| `run_test` | Run dastest on a test file |
| `format_file` | Format a .das file in place |

---

## Phase 1: Navigation (High Priority)

### go_to_definition

**What:** Given a symbol name (and optionally a file + line), return the file path and line number where it's defined.

**Why:** The most-requested IDE feature. Lets the AI jump directly to definitions instead of grepping.

**Implementation approach:**
- For user-defined symbols: compile the file, walk `for_each_function` / `for_each_structure` / `for_each_enumeration` / `for_each_global` on the program's module. Each has a `LineInfo` with `.fileInfo.name` and `.line`.
- For symbols from other modules: the `LineInfo` points to the original source file (or empty for C++ builtins).
- For struct fields: iterate `structure.fields`, each has its own `LineInfo`.
- For class methods: `for_each_function` already includes them (check `isClassMethod` flag).
- For local variables: requires expression-level AST walking — more complex. Could use `ExprVisitor` to find `ExprVar` or `ExprLet` nodes matching the name at a given line.

**Parameters:**
- `file` (required) — the .das file containing the reference
- `symbol` (required) — name of the symbol to find
- `line` (optional) — line number hint to disambiguate overloads
- `column` (optional) — column hint for field access chains

**Output:** File path, line number, symbol kind (function/struct/enum/global/field/variable), and a snippet of the definition.

**Difficulty:** Medium. Module-level symbols are straightforward via existing AST iteration. Local variables need expression walking.

### find_references

**What:** Given a symbol definition, find all files and lines where it's used.

**Why:** Essential for understanding impact before refactoring. "Who calls this function?" "Where is this struct used?"

**Implementation approach:**
- Compile the file to get the full program with all dependencies resolved.
- Walk the AST of every function body using `ExprVisitor`:
  - `ExprCall` nodes → function references
  - `ExprVar` nodes → variable/global references
  - `ExprField` nodes → struct field references
  - Type references in `ExprNew`, `ExprIsType`, variable declarations
- Match by comparing the resolved definition (not just name string) to avoid false positives from overloads.
- For cross-file references: need to compile multiple files or use a workspace-level index.

**Challenges:**
- daslang doesn't have a workspace/project concept — each file is compiled independently. Finding all references across a project means compiling every `.das` file that might use the symbol.
- Could provide a `scope` parameter: `"file"` (single file), `"module"` (the module and its direct users), or `"project"` (scan all `.das` files in a directory tree).
- File-scope references are fast (single compilation). Project-scope is slow but thorough.

**Parameters:**
- `symbol` (required) — name to search for
- `file` (optional) — the file defining the symbol (for disambiguation)
- `scope` (optional) — `"file"`, `"module"`, or `"project"` (default: `"file"`)
- `root` (optional) — root directory for project-scope search

**Output:** List of `(file, line, column, context_snippet)` tuples.

**Difficulty:** Hard. Single-file is medium; cross-file requires scanning and compiling multiple files.

### type_of

**What:** Given a file, line, and column (or an expression string), return the resolved type.

**Why:** daslang has type inference — the AI often needs to know what type an expression resolves to without guessing.

**Implementation approach:**
- For expression strings: wrap in a function, compile, extract the expression's `TypeDecl` via `ExprVisitor`.
- For file+location: compile the file, find the expression at that line/column in the AST, return `describe(expr._type)`.
- Could also show type modifiers (const, ref, temporary, implicit).

**Parameters:**
- `expression` (optional) — an expression to type-check
- `file` (optional) — source file
- `line` (optional) — line number
- `column` (optional) — column number

**Output:** Type description string, plus whether it's const/ref/temporary.

**Difficulty:** Medium. Expression mode is straightforward. File+location mode needs position-to-AST-node mapping.

---

## Phase 2: Refactoring (Medium Priority)

### rename_symbol

**What:** Rename a function, variable, struct, field, or enum value across one or more files.

**Why:** Safe mechanical renaming is tedious and error-prone by hand. The compiler knows all references.

**Implementation approach:**
- First, use `go_to_definition` to find the definition.
- Then, use `find_references` to find all usages.
- Apply text replacements at each location.
- Re-compile to verify the rename didn't break anything.
- Return the list of modified files and a diff.

**Important constraints:**
- Must handle qualified names: renaming struct `Foo` must also rename `Foo.field` access, `new Foo()`, type annotations `x : Foo`, etc.
- Method renames: `obj.method()` and `obj |> method()` and `obj->method()` all need updating.
- Must NOT rename unrelated symbols that happen to share the same name (different module, different scope).
- Should refuse to rename builtins (no source to modify).

**Parameters:**
- `file` (required) — file containing the symbol definition
- `symbol` (required) — current name
- `new_name` (required) — desired new name
- `scope` (optional) — `"file"` or `"project"`
- `dry_run` (optional, default true) — if true, return diff without modifying files

**Output:** List of changes `(file, line, old_text, new_text)` and compilation check result.

**Difficulty:** Hard. Depends on `go_to_definition` and `find_references`. Safe renaming across files is complex.

### extract_function

**What:** Extract a selected range of code into a new function, automatically determining parameters and return values.

**Why:** Common refactoring. The AI can identify the code to extract but needs compiler help to determine which variables must become parameters.

**Implementation approach:**
- Parse the selected code range to identify:
  - Variables read but not defined in the selection → become function parameters
  - Variables defined in the selection and used after it → become return values
  - Variables defined and only used within the selection → stay local
- Generate the function signature with proper types (from the compiled AST).
- Replace the selected code with a call to the new function.
- Handle `var` (mutable) parameters correctly — if the selection modifies a variable used later, it needs `var` parameter.

**Challenges:**
- daslang's move semantics: if the selection moves a value (`<-`), the extraction must preserve that.
- Block/lambda captures: if extracting code that's inside a block, the new function might need different parameter passing.
- Control flow: `return`, `break`, `continue` inside the selection complicate extraction.

**Parameters:**
- `file` (required) — source file
- `start_line` (required) — first line of selection
- `end_line` (required) — last line of selection
- `function_name` (required) — name for the new function
- `dry_run` (optional, default true)

**Output:** The new function definition, the modified call site, and a compilation check.

**Difficulty:** Very hard. Requires deep AST analysis of data flow. Could start with a simpler version that only handles straightforward cases (no control flow escapes, no moves).

### inline_function

**What:** Replace a function call with the function's body, substituting parameters.

**Why:** The inverse of extract — useful for removing unnecessary abstraction.

**Difficulty:** Hard. Needs parameter substitution, handling of return statements, variable name conflicts.

---

## Phase 3: Intelligence (Medium Priority)

### explain_error

**What:** Given a compiler error message, provide a detailed explanation with fix suggestions.

**Why:** daslang error messages can be cryptic, especially for type mismatches, move semantics, and macro expansion errors. The AI can provide better explanations but needs structured error info.

**Implementation approach:**
- Parse the error output from `compile_check` into structured fields: error code, file, line, message, context.
- Match against known error patterns and provide:
  - Plain-English explanation
  - Common causes
  - Suggested fixes with code snippets
- Could maintain a knowledge base of error patterns (similar to Rust's error index).

**Parameters:**
- `file` (required) — file with the error
- `error_text` (optional) — specific error message to explain (if not provided, compile and explain all errors)

**Output:** Structured explanation per error: what it means, why it happens, how to fix it.

**Difficulty:** Medium. Pattern matching on error messages is straightforward. Quality depends on the knowledge base.

### type_search

**What:** Search for functions by their type signature (like Haskell's Hoogle or Gleam's Gloogle).

**Why:** "I have a `string` and want an `int` — what function does that?" This is incredibly useful for discovering API.

**Implementation approach:**
- Index all functions across registered modules by parameter types and return type.
- Parse a query like `string -> int` or `(array<int>, int) -> bool`.
- Match functions where parameter/return types are compatible (exact match, then subtype match).
- Rank by relevance: exact matches first, then partial matches.

**Parameters:**
- `query` (required) — type signature query (e.g., `string -> int`, `float, float -> float`)
- `module` (optional) — limit search to a specific module

**Output:** List of matching functions with their full signatures and module names.

**Difficulty:** Medium-hard. Parsing type queries and matching against `TypeDecl` requires a type compatibility checker.

### try_fix

**What:** Given a file with compilation errors, attempt automatic fixes and return the corrected code.

**Why:** Many errors have mechanical fixes: missing `var`, wrong type cast, missing `require`, missing parentheses.

**Implementation approach:**
- Compile the file, collect errors.
- For each error, apply pattern-matched fixes:
  - "expecting T, got U" → insert cast or conversion
  - "undefined symbol X" → add `require` for the module containing X (using `find_symbol`)
  - "variable not found" → suggest `var` declaration
  - "can't copy" → suggest `:=` or `<-`
- Re-compile after each fix to verify it works.
- Iterate until no more auto-fixable errors remain.

**Parameters:**
- `file` (required) — file to fix
- `dry_run` (optional, default true)
- `max_iterations` (optional, default 5)

**Output:** Diff of changes made, remaining errors (if any), compilation result.

**Difficulty:** Hard. Each error pattern needs a specific fixer. Interactions between fixes can cause cascading issues.

---

## Phase 4: Project-Level (Lower Priority, Some Deferred)

### workspace_index

**What:** Build an in-memory index of all `.das` files in a project directory for fast cross-file operations.

**Why:** Many tools (find_references, rename_symbol) need to know about all files in a project. Compiling each file on every query is too slow.

**Implementation approach:**
- Scan a directory tree for `.das` files.
- Compile each file and cache: function/type/global definitions, require graph, symbol locations.
- Invalidate cache entries when files change (check mtime).
- Other tools query the index instead of compiling from scratch.

**Difficulty:** Hard. Cache invalidation is the main challenge (macro expansion means a change in one file can affect others).

### dependency_graph

**What:** Visualize the full dependency graph of a file or project.

**Why:** Understanding module dependencies helps with architecture decisions, finding circular deps, and planning refactors.

**Implementation approach:**
- Already have `list_requires` for single-file deps. Extend to:
  - Build a graph across all files in a project
  - Detect cycles
  - Output in DOT format or structured JSON
  - Show which deps are direct vs transitive

**Parameters:**
- `file` or `root` — starting point
- `format` — `"text"`, `"json"`, or `"dot"`
- `depth` (optional) — max depth to traverse

**Difficulty:** Medium. Building on `list_requires` infrastructure.

### package_search

**What:** Search a package registry for daslang packages by name, description, or functionality.

**Status:** **Deferred** — waiting for the daslang package manager to be implemented.

### scaffold

**What:** Generate boilerplate code for common patterns: new module, test file, C++ binding, etc.

**Why:** Ensures generated code follows project conventions (correct `options`, `require`s, annotations).

**Templates:**
- `module` — new daslib module with proper header
- `test` — test file with `[test]` functions and fixture setup
- `binding` — C++ module binding skeleton (`.das_module` + C++ stub)
- `tutorial` — tutorial .das file with comments

**Difficulty:** Easy-medium. Template-based generation. The AI already does this well, so the value-add is mainly in ensuring conventions.

---

## Implementation Priority

Recommended order based on value/effort ratio:

1. **go_to_definition** — highest value, medium effort, enables other tools
2. **type_of** — high value for AI-assisted coding, medium effort
3. **explain_error** — high value, relatively easy
4. **find_references** (file scope first) — high value, medium effort
5. **dependency_graph** — medium value, easy (extends `list_requires`)
6. **type_search** — high value for API discovery, medium-hard effort
7. **rename_symbol** — high value but depends on #1 and #4
8. **try_fix** — medium-high value, hard
9. **extract_function** — medium value, very hard
10. **workspace_index** — enabler for cross-file tools at scale
11. **scaffold** — low priority (AI already generates good code)
12. **package_search** — deferred until package manager exists

## Architecture Notes

### Position-to-AST Mapping

Several tools (go_to_definition, type_of, find_references) need to map a file position (line, column) to an AST node. This requires:

1. Compile the file to get the full AST.
2. Walk the AST with an `ExprVisitor` that checks each node's `LineInfo` against the target position.
3. Return the most specific (deepest) node containing that position.

This could be a shared utility in `common.das`:
```
def find_node_at(program, file, line, column) : ExpressionPtr?
```

### Cross-File Compilation

For project-level tools, we need to compile multiple files. Options:
- **Sequential:** compile each file independently. Simple but slow.
- **Cached:** maintain a `ModuleGroup` across compilations. Faster but complex lifetime management.
- **Indexed:** pre-scan files for `require` statements (text-level), build dependency order, compile in order. Best for large projects.

### Tool Composition

Many complex tools are compositions of simpler ones:
- `rename_symbol` = `go_to_definition` + `find_references` + text replacement + `compile_check`
- `try_fix` = `compile_check` + error pattern matching + text edit + `compile_check`
- `extract_function` = AST analysis + code generation + `compile_check`

Building the Phase 1 tools well creates a foundation for everything else.
