// Macro Tutorial 6: Structure Macros
//
// This tutorial demonstrates [structure_macro] — the structure-level
// equivalent of [function_macro].  Structure macros annotate structs
// and classes, and have three methods that fire at different stages
// of the compilation pipeline:
//
//   apply()  — during parsing, before inference.  Can add fields,
//              validate annotation arguments, and generate new
//              functions.  Cannot inspect inferred types.
//
//   patch()  — after inference.  Types are resolved, so type-aware
//              validation is possible.  Can set astChanged to restart
//              inference if modifications are needed.
//
//   finish() — after all inference and optimization.  Read-only:
//              useful for diagnostics and compile-time reporting.
//
// We build a [serializable] annotation that:
//   1. adds a _version field and generates a describe_X() function
//      for each annotated struct (apply)
//   2. rejects structs with unsupported field types like lambdas
//      and blocks (patch)
//   3. prints a compile-time summary of each struct (finish)
//
// Covers:
//   - [structure_macro(name=...)] to register a structure annotation
//   - AstStructureAnnotation with apply / patch / finish
//   - add_structure_field to add fields during apply
//   - Type validation after inference in patch
//   - qmacro_function + $b() + $v() + $f() for code generation
//   - Compile-time diagnostics in finish
//
// The macro is defined in structure_macro_mod.das.
//
// Run: daslang.exe tutorials/macros/06_structure_macro.das

options gen2

require structure_macro_mod

// ── A simple struct ──
//
// Default version (1).  The macro adds _version = 1 automatically.

[serializable]
struct Color {
    r : float
    g : float
    b : float
}

// ── A struct with explicit version ──
//
// version=2 is stored in the annotation argument and used to
// initialize the _version field.

[serializable(version=2)]
struct Player {
    name : string
    health : int
    score : float
}

// ── Using the generated describe functions ──

[export]
def main() {
    var c = Color(r = 0.2, g = 0.7, b = 1.0)
    print("--- describe_Color ---\n")
    describe_Color(c)

    print("\n--- describe_Player ---\n")
    var p = Player(name = "Alice", health = 100, score = 42.5)
    describe_Player(p)

    // The _version field is a regular field — accessible at runtime
    print("\n--- version info ---\n")
    print("Color version: {c._version}\n")
    print("Player version: {p._version}\n")
}

// compile-time output (from finish):
//   [serializable] Color: 3 field(s), version 1
//   [serializable] Player: 3 field(s), version 2
//
// runtime output:
// --- describe_Color ---
// Color (version 1):
//   r = 0.2
//   g = 0.7
//   b = 1
//
// --- describe_Player ---
// Player (version 2):
//   name = Alice
//   health = 100
//   score = 42.5
//
// --- version info ---
// Color version: 1
// Player version: 2
