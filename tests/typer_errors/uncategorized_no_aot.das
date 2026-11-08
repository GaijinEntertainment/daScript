options gen2
options no_aot

require daslib/ast
require strings
require dastest/testing_boost public
require _compile_issues

// Verifies issue #2571: type-mismatch messages print the alias short name and a
// single `where '<alias> = <def>'` clause instead of expanding the alias inline.




[test(description = "an argument type mismatch message prints the typedef short name in its expecting clause and adds a where clause for the alias")]
def test_alias_short_name_in_mismatch(t : T?) {
    t |> run("argument mismatch prints alias short name + where clause") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_alias_arg_mismatch.das")
        // expecting-clause uses the short alias name, not the expanded tuple
        t |> success(find(issues, "expecting 'array<RttiValue> const'") >= 0,
            "short alias name in expecting clause")
        // the alias is defined once in a trailing where clause
        t |> success(find(issues, "where 'RttiValue = tuple<argname:string;") >= 0,
            "where clause defines the alias")
        // the short name must not be doubled with a stray ` aka `
        t |> success(find(issues, "RttiValue aka RttiValue") < 0,
            "no doubled `aka` after the short name")
    }
}

// [arch] owns the SHAPE of a citation and nothing else: exactly one `at=` string, carrying a
// document path and an anchor, both non-empty, separated by '#'. Whether the citation resolves
// to a real section is LINT026's question (utils/lint/main.das), which is why a well-shaped
// citation to a document that is not there still compiles.




def expect_issue(t : T?; fixture, message : string) {
    let issues = compile_issues("{get_das_root()}/tests/typer_errors/{fixture}")
    t |> success(find(issues, message) >= 0,
        "expected '{message}' compiling {fixture}, got: {issues}")
}


[test(description = "an [arch] annotation with a missing, empty, anchorless, doc-less, misnamed, non-string or doubled argument is reported with its own message")]
def test_arch_citation_shape_errors(t : T?) {
    t |> run("[arch] with no citation") @(t : T?) {
        t |> expect_issue("_arch_no_argument.das",
            "expecting exactly one argument, at=\"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] citing the empty string") @(t : T?) {
        t |> expect_issue("_arch_empty_citation.das",
            "citation \"\" is not \"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] citing a document with no anchor") @(t : T?) {
        t |> expect_issue("_arch_no_hash.das",
            "citation \"ARCHITECTURE.md\" is not \"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] with a named argument that is not at") @(t : T?) {
        t |> expect_issue("_arch_wrong_name.das",
            "expecting exactly one argument, at=\"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] with a non-string argument") @(t : T?) {
        t |> expect_issue("_arch_wrong_type.das",
            "expecting exactly one argument, at=\"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] with a second argument") @(t : T?) {
        t |> expect_issue("_arch_two_args.das",
            "expecting exactly one argument, at=\"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] citing an anchor with no document half") @(t : T?) {
        t |> expect_issue("_arch_no_doc_half.das",
            "citation \"#x\" is not \"<doc>.md#<anchor>\"")
    }
    t |> run("[arch] citing a document with an empty anchor half") @(t : T?) {
        t |> expect_issue("_arch_no_anchor_half.das",
            "citation \"ARCHITECTURE.md#\" is not \"<doc>.md#<anchor>\"")
    }
}

// The default message a failing assert supplies for itself used to name the OTHER construct:
// a bare static_assert reported "concept failed" and a bare concept_assert reported "static assert
// failed", while both carried the right error code. Codes are pinned by `expect` elsewhere; only a
// compile-and-read-the-text check can pin the wording, so it lives here.




[test(description = "a failing bare static_assert reports static assert failed and a bare concept_assert reports concept assert failed")]
def test_bare_assert_names_its_own_construct(t : T?) {
    t |> run("a bare static_assert reports 'static assert failed'") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_bare_static_assert.das")
        t |> success(find(issues, "static assert failed") >= 0, "static_assert must name itself")
        t |> success(find(issues, "concept") < 0, "static_assert must not mention concepts at all")
    }
    t |> run("a bare concept_assert reports 'concept assert failed'") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_bare_concept_assert.das")
        t |> success(find(issues, "concept assert failed") >= 0, "concept_assert must name itself")
        t |> success(find(issues, "static assert failed") < 0, "concept_assert must not claim to be a static assert")
    }
}

// [inline] is fail-closed: a shape the inliner can't splice, an address-taken
// [inline] function, recursion through the inline graph, or a call position with
// no lowering must each fail compilation with a targeted message (ast_inline.cpp)





[test(description = "[inline] contract violations such as taking the address of an inline function, recursion, use in initializers and conflicting never_inline are each reported with their own message")]
def test_inline_contract_errors(t : T?) {
    t |> run("taking the address of an [inline] function") @(t : T?) {
        t |> expect_issue("_inline_addr.das",
            "can't take the address of [inline] function")
    }
    t |> run("recursion through the inline graph") @(t : T?) {
        t |> expect_issue("_inline_recursion.das",
            "[inline] function is recursive through the inline graph")
    }
    t |> run("[inline] call in a global initializer") @(t : T?) {
        t |> expect_issue("_inline_global_init.das",
            "in a global initializer")
    }
    t |> run("[inline] call in a field initializer") @(t : T?) {
        t |> expect_issue("_inline_field_init.das",
            "in a field initializer")
    }
    t |> run("[inline] on a non-ExprOp punctuation function") @(t : T?) {
        t |> expect_issue("_inline_operator.das",
            "[inline] does not support operator '[]'")
    }
    t |> run("recursion through an operator site") @(t : T?) {
        t |> expect_issue("_inline_operator_recursion.das",
            "[inline] function is recursive through the inline graph")
    }
    t |> run("statement-needing call in a ?? default") @(t : T?) {
        t |> expect_issue("_inline_coalesce.das",
            "inside ?? or a safe-navigation suffix")
    }
    t |> run("[inline] combined with [never_inline]") @(t : T?) {
        t |> expect_issue("_inline_never_inline_conflict.das",
            "[never_inline] conflicts with [inline]")
    }
    t |> run("[inline] combined with [never_fast_math], either order") @(t : T?) {
        t |> expect_issue("_inline_never_fast_math_conflict.das",
            "[never_fast_math] conflicts with [inline]")
        t |> expect_issue("_never_fast_math_inline_conflict.das",
            "[inline] conflicts with [never_inline] or [never_fast_math]")
    }
    t |> run("early return with a tail inside a finally-carrying block") @(t : T?) {
        t |> expect_issue("_inline_finally_hazard.das",
            "early returns conflict with a finally section")
    }
    t |> run("[inline] call in a mixed-type ternary arm") @(t : T?) {
        t |> expect_issue("_inline_mixed_ternary.das",
            "inside a mixed-type ternary")
    }
    t |> run("cross-module body with user-spelled _:: dispatch") @(t : T?) {
        t |> expect_issue("_inline_scope_dispatch.das",
            "body dispatches '_::help_scale' at the call site's module")
    }
    t |> run("[inline] call site inside a different module's with scope") @(t : T?) {
        t |> expect_issue("_inline_scope_in_with.das",
            "the call site resolves inside with (module _inline_scope_dispatch_helper)")
    }
    t |> run("writable by-ref argument that may alias caller storage") @(t : T?) {
        t |> expect_issue("_inline_alias_hazard.das",
            "may alias caller storage the callee writes")
    }
}

// operator declarations dispatch through fixed-operand nodes - shapes no call
// site can honor (like defaulted parameters) must fail compilation





[test(description = "an operator declared with a defaulted parameter is rejected with a message that names the operator")]
def test_operator_declaration_errors(t : T?) {
    t |> run("operator with a defaulted parameter") @(t : T?) {
        t |> expect_issue("_operator_default_arg.das",
            "operator '+' can't have default arguments")
    }
}

// a property whose setter is private to another module reads as a plain rvalue
// at the use site; the copy and clone errors name the setter they could not use
[test(description = "copying or cloning into a property whose setter is private reports the private setter as the reason")]
def test_private_property_setter_hint(t : T?) {
    t |> run("copy into a property with a private setter") @(t : T?) {
        t |> expect_issue("_private_setter_copy.das",
            "property setter _private_setter_helper::operator .`flag`clone(self: _private_setter_helper::Gate; value: bool const): void at")
        t |> expect_issue("_private_setter_copy.das", "is private")
    }
    t |> run("clone into a property with a private setter") @(t : T?) {
        t |> expect_issue("_private_setter_clone.das", "can only clone to a reference")
        t |> expect_issue("_private_setter_clone.das", "property setter _private_setter_helper::operator .`flag`clone")
    }
}

// the `?` and `[` operator families lower to plain ExprCall, where a defaulted
// trailing parameter pads like any call - the reject above must not catch them
struct Boxed {
    v : int
}

def operator ??(a : Boxed?; b : Boxed; scale : int = 2) => a != null ? *a : Boxed(v = b.v * scale)

def operator [] (b : Boxed; i : int; scale : int = 3) => b.v * i * scale

[test(description = "defaulted ?? and [] operator overloads compile and the defaults pad the call")]
def test_operator_defaults_that_pad(t : T?) {
    t |> run("defaulted `??` and `[]` overloads compile and pad") @(t : T?) {
        var p : Boxed?
        t |> equal((p ?? Boxed(v = 5)).v, 10)
        t |> equal(Boxed(v = 2)[3], 18)
    }
}

// A keyword in a name position reports the reserved word, not bison's
// bare token dump alone.




[test(description = "a keyword used as a parameter name is reported with the parser token dump and a note naming it as a reserved word")]
def test_reserved_word_as_parameter_name(t : T?) {
    t |> run("a keyword as a parameter name is named as the reserved word it is") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_reserved_word_name.das")
        t |> success(find(issues, "unexpected class") >= 0,
            "the parser's own token dump stays: {issues}")
        t |> success(find(issues, "'class' is a reserved word and cannot be used as a name") >= 0,
            "and the note names the word")
        let digits = compile_issues(
            "{get_das_root()}/tests/typer_errors/_reserved_word_name_digits.das")
        t |> success(find(digits, "'int2' is a reserved word and cannot be used as a name") >= 0,
            "a type keyword with digits in it is named too: {digits}")
    }
}

[test(description = "an identifier in the wrong place is not called a reserved word in the diagnostic")]
def test_identifier_out_of_place_draws_no_note(t : T?) {
    t |> run("an identifier out of place is not called a reserved word") @(t : T?) {
        let issues = compile_issues("{get_das_root()}/tests/typer_errors/_unexpected_name.das")
        t |> success(find(issues, "unexpected name") >= 0,
            "the parser's token dump names the identifier token: {issues}")
        t |> success(find(issues, "reserved word") < 0, "and no note follows it")
    }
}

// static_if must resolve identically in every compile profile. Under the lint/IDE
// profile (no_optimizations + no_infer_time_folding) a const global's init expression
// is never folded in place, so getConstExpr computes the value on the side
// (ast_infer_type_helper.cpp): composed initializers compile, genuinely non-constant
// conditions still refuse.


def compile_lint_profile(file_path : string) : tuple<ok : bool; issues : string> {
    var okc = false
    var issuesText = ""
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.export_all = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(file_path, access, unsafe(addr(mg)), cop) $(ok, _program, issues) {
                okc = ok
                issuesText := issues
            }
        }
    }
    return (ok = okc, issues = issuesText)
}

[test(description = "under the lint profile static_if resolves composed const globals, reports mutually recursive initializers as an init loop without crashing and still refuses a non-constant condition")]
def test_static_if_lint_profile(t : T?) {
    t |> run("composed const-global initializers resolve") @(t : T?) {
        let r = compile_lint_profile("{get_das_root()}/tests/typer_errors/_static_if_composed_init.das")
        t |> success(r.ok, "expected a clean compile, got: {r.issues}")
    }
    t |> run("mutually recursive initializers report the init loop, not a crash") @(t : T?) {
        let r = compile_lint_profile("{get_das_root()}/tests/typer_errors/_static_if_mutual_global.das")
        t |> success(!r.ok, "expected the compile to refuse")
        t |> success(find(r.issues, "initialization loop") >= 0, "wrong error: {r.issues}")
    }
    t |> run("a genuinely non-constant condition still refuses") @(t : T?) {
        let r = compile_lint_profile("{get_das_root()}/tests/typer_errors/_static_if_nonconst.das")
        t |> success(!r.ok, "expected the compile to refuse")
        t |> success(find(r.issues, "static_if must resolve to constant") >= 0, "wrong error: {r.issues}")
    }
}
