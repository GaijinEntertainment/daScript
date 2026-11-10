options gen2
require dastest/testing_boost public
require daslib/ast_boost
require daslib/fio

def compile_it(name : string) : bool {
    var res = false
    let path = path_join(get_das_root(), "tests/language/{name}")
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; _program; _issues) {
                res = ok
            }
        }
    }
    return res
}

def compile_given_source(source : string) : bool {
    var res = false
    let path = path_join(get_das_root(), "tests/language/_virtual.das")
    var inscope access <- make_file_access("")
    set_file_source(access, path, source)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            compile_file(path, access, unsafe(addr(mg)), cop) $(ok; _program; _issues) {
                res = ok
            }
        }
    }
    return res
}

[test(description = "the same read outside the folder the web lane runs")]
def test_reads_the_tree(t : T?) {
    t |> success(compile_it("_fixture.das"))
    t |> success(compile_given_source("options gen2"))
}
