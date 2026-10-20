options gen2
options no_aot
options rtti

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/ast_print
require daslib/option
require strings

//! ast_print spells a make-variant of a case with no value as the
//! bare case name, and a valued case as `name = value`, so a printed
//! tree reads back as the source it came from. A value `$v` bakes
//! (convert_to_expression) takes the same shape.

typedef ValueTags = variant<amount : int; nada>

def private fixture_path() : string => "{get_das_root()}/tests/ast/_variant_bare_case_fixture.das"

def private returned_expression(var fn : FunctionPtr) : ExpressionPtr {
    var blk = fn.body ?as ExprBlock
    return null if (blk == null || empty(blk.list))
    var ret = blk.list[0] ?as ExprReturn
    return ret != null ? ret.subexpr : null
}

def private printed(var expr : ExpressionPtr) : string {
    return build_string() $(var writer) {
        printExpr(expr, unsafe(addr(writer)))
    }
}

def private missing_arm_fixture_path() : string =>
    "{get_das_root()}/tests/ast/_variant_missing_arm_fixture.das"

[test]
def test_print_variant_bare_case(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the fixture did not compile: {issues}")
                return if (!ok || program == null)
                let mod = get_this_module(program)
                var bare = returned_expression(find_unique_function(mod, "bare"))
                t |> success(bare != null && bare is ExprMakeVariant,
                    "bare() does not return a make-variant")
                return if (bare == null)
                t |> equal(printed(bare), "variant<nada;anything:int> aka Tags(nada)")
                var valued = returned_expression(find_unique_function(mod, "valued"))
                t |> success(valued != null && valued is ExprMakeVariant,
                    "valued() does not return a make-variant")
                return if (valued == null)
                t |> equal(printed(valued), "variant<nada;anything:int> aka Tags(anything = 7)")
            }
        }
    }
}

[test]
def test_missing_arm_report_spells_bare_case(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            let fixture = missing_arm_fixture_path()
            compile_file(fixture, access, unsafe(addr(mg)), cop) $(ok; _program; issues) {
                t |> success(!ok, "the fixture compiled although an arm is missing")
                let reported = string(issues) |> find("no arm matches `Tags(nothing)`") >= 0
                t |> success(reported, "unexpected report: {issues}")
            }
        }
    }
}

[test]
def test_baked_variant_bare_case(t : T?) {
    ast_gc_guard() {
        let bare = ValueTags(nada)
        t |> equal(printed(convert_to_expression(bare, LineInfo())), "(nada)")
        let valued = ValueTags(amount = 7)
        t |> equal(printed(convert_to_expression(valued, LineInfo())), "(amount = 7)")
        let nothing = none(type<string>)
        t |> equal(printed(convert_to_expression(nothing, LineInfo())), "(none)")
    }
}
