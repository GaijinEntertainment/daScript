// Macro Tutorial 6: Structure Macros — Module
//
// This module defines the [serializable] structure macro, which
// demonstrates all three AstStructureAnnotation methods:
//
//   apply()  — runs before inference.  Validates annotation arguments,
//              adds a _version field, and generates a describe_X()
//              function for the struct.
//
//   patch()  — runs after inference.  Validates field types (rejects
//              unsupported types like lambdas and blocks).  Can also
//              set astChanged to restart inference when needed.
//
//   finish() — runs after all inference is complete.  Prints a
//              compile-time diagnostic summary of the struct.
//
// Structure macros cannot be used in the module that defines them —
// see 06_structure_macro.das for usage.

options gen2

module structure_macro_mod public

require ast
require rtti
require daslib/ast_boost
require daslib/templates_boost

// ============================================================================
// The [serializable] structure macro
//
// Given:
//
//   [serializable(version=2)]
//   struct Player {
//       name : string
//       health : int
//   }
//
// The macro:
//   1. (apply)  adds a `_version : int = 2` field and generates
//               `def describe_Player(obj)` that prints all fields
//   2. (patch)  validates field types (rejects blocks/lambdas/functions)
//   3. (finish) prints "[serializable] Player: 3 fields" at compile time
//
// ============================================================================

[structure_macro(name="serializable")]
class SerializableMacro : AstStructureAnnotation {

    // ------------------------------------------------------------------
    // apply() — before type inference
    //
    // Runs during parsing, before types are resolved.  We:
    //   - validate annotation arguments (only `version` is allowed)
    //   - add a `_version : int` field to the struct
    //   - generate a describe_StructName() function
    //
    // Generated code must exist before inference so that callers can
    // reference it.  Type validation is deferred to patch().
    // ------------------------------------------------------------------
    def override apply(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string) : bool {
        // Validate arguments: only `version` (int) is allowed
        var version = 1
        for (arg in args) {
            if (arg.name == "version") {
                let val = get_annotation_argument_value(arg)
                if (val is tInt) {
                    version = val as tInt
                } else {
                    errors := "[serializable] 'version' argument must be an integer"
                    return false
                }
            } else {
                errors := "[serializable] unknown argument '{arg.name}' — only 'version' is supported"
                return false
            }
        }
        // Add a _version field with the specified version.
        // Pass temporaries — add_structure_field moves its arguments.
        st |> add_structure_field("_version", clone_type(qmacro_type(type<int>)), qmacro($v(version)))

        // Generate: def describe_StructName(obj : StructName)
        //
        // IMPORTANT: inside qmacro(), string interpolation like "{x}"
        // creates AST nodes that reference `x` at RUNTIME.  Macro-time
        // variables (st, fld) don't exist at runtime.  Use $v() to
        // splice compile-time strings, and only `obj` (the generated
        // function's parameter) for runtime references.
        let funcName = "describe_{st.name}"
        var inscope bodyExprs : array<ExpressionPtr>

        // Header: "StructName (version " + runtime _version + "):\n"
        bodyExprs |> emplace_new <| qmacro(print($v("{st.name} (version ")))
        bodyExprs |> emplace_new <| qmacro(print("{obj._version}"))
        bodyExprs |> emplace_new <| qmacro(print($v("):\n")))

        // One line per user field: "  fieldName = <value>\n"
        for (fld in st.fields) {
            if (fld.name == "_version") {
                continue  // skip the internal version field
            }
            bodyExprs |> emplace_new <| qmacro(print($v("  {fld.name} = ")))
            bodyExprs |> emplace_new <| qmacro(print("{obj.$f(fld.name)}"))
            bodyExprs |> emplace_new <| qmacro(print($v("\n")))
        }

        // Build and register the function.
        // $t(st) splices the struct type, $b() injects the statements.
        var inscope fn <- qmacro_function(funcName) $(obj : $t(st)) {
            $b(bodyExprs)
        }
        fn.flags |= FunctionFlags.generated
        fn.body |> force_at(st.at)
        add_function(st._module, fn)

        return true
    }

    // ------------------------------------------------------------------
    // patch() — after type inference
    //
    // Types are now resolved.  We validate that no field has an
    // unsupported type (blocks, lambdas, functions cannot be
    // serialized).  This check is impossible in apply() because
    // types aren't inferred yet.
    //
    // patch() can also set astChanged to restart inference — useful
    // when generating new code that depends on inferred types.  Here
    // we only validate, so astChanged is not needed.
    // ------------------------------------------------------------------
    def override patch(var st : StructurePtr; var group : ModuleGroup;
                       args : AnnotationArgumentList; var errors : das_string;
                       var astChanged : bool&) : bool {
        // Validate: reject unsupported field types
        for (fld in st.fields) {
            if (fld._type.baseType == Type.tBlock) {
                errors := "[serializable] struct '{st.name}' has block field '{fld.name}' — blocks cannot be serialized"
                return false
            }
            if (fld._type.baseType == Type.tLambda) {
                errors := "[serializable] struct '{st.name}' has lambda field '{fld.name}' — lambdas cannot be serialized"
                return false
            }
            if (fld._type.baseType == Type.tFunction) {
                errors := "[serializable] struct '{st.name}' has function field '{fld.name}' — functions cannot be serialized"
                return false
            }
        }
        return true
    }

    // ------------------------------------------------------------------
    // finish() — after inference is complete
    //
    // Read-only inspection phase.  All types are final, code generation
    // is done. We print a compile-time diagnostic so the developer sees
    // a summary during compilation.
    //
    // Unlike apply() and patch(), finish() cannot modify the struct
    // or generate new code.  It is useful for validation, diagnostics,
    // and AOT-related output.
    // ------------------------------------------------------------------
    def override finish(var st : StructurePtr; var group : ModuleGroup;
                        args : AnnotationArgumentList; var errors : das_string) : bool {
        // Count user-visible fields (skip _version)
        var userFields = 0
        for (fld in st.fields) {
            if (fld.name != "_version") {
                userFields++
            }
        }
        print("[serializable] {st.name}: {userFields} field(s), version ")
        // Read the version from the annotation argument
        let ver = find_arg(args, "version")
        if (ver is tInt) {
            print("{ver as tInt}")
        } else {
            print("1")
        }
        print("\n")
        return true
    }
}
