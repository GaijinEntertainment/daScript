options gen2
options no_aot

require dastest/testing_boost public
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost

def fixture_path() : string => path_join(get_das_root(), "tests/language/_virtual_dead_arm.das")

def fixture_source() : string {
    return [
        "options gen2",
        "",
        "let NC = 4",
        "let RSI = 1",
        "",
        "def body : float \{",
        "    var s0, s1 : float[NC]",
        "    s0[0] = 1.0",
        "    static_if (RSI == 2) \{",
        "        s1[0] = 2.0",
        "    \}",
        "    return s0[0]",
        "\}",
        ""
    ] |> join("\n")
}

class LetMarks : AstVisitor {
    marked : table<string; bool>
    def override preVisitExprLetVariable(_expr : ExprLet?; arg : VariablePtr; _last : bool) : void {
        marked[string(arg.name)] = arg.flags.marked_used
    }
}

[test]
def test_dead_arm_marks_let_typed_later(t : T?) {
    var inscope access <- make_file_access("")
    set_file_source(access, fixture_path(), fixture_source())
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(fixture_path(), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                t |> success(ok, "the fixture compiles: {issues}")
                return if (!ok)
                var marks = new LetMarks()
                for_each_function(get_this_module(program), "body") $(fn) {
                    make_visitor(*marks) $(adapter) {
                        visit(fn, adapter)
                    }
                }
                t |> equal(marks.marked?["s1"] ?? false, true)
                unsafe {
                    delete marks
                }
            }
        }
    }
}
