options gen2
options no_aot = true

require dastest/testing_boost public
require daslib/rtti
require daslib/ast
require daslib/fio
require strings

//! a variant with no name of its own is spelled ``variant`` in a
//! coverage error - never the alias of one of its alternatives'
//! types, which ``describe`` prints after the first ``aka``
[test]
def test_unaliased_variant_is_spelled_variant(t : T?) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            let fixture = path_join(get_das_root(), "tests/match/_fixtures/unaliased_variant.das")
            compile_file(fixture, access, unsafe(addr(mg)), cop) $(ok, _program, issues) {
                let text = string(issues)
                t |> success(!ok, "the non-exhaustive match does not compile")
                t |> success(find(text, "no arm matches `variant(s = _)`") >= 0,
                             "the missing case is spelled with `variant`:\n{text}")
                t |> success(find(text, "Name(") < 0,
                             "the missing case is not named after an alternative's type:\n{text}")
            }
        }
    }
}
