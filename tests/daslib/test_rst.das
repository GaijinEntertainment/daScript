options gen2
options rtti
options no_aot
options no_coverage

require dastest/testing_boost public
require daslib/rtti
require daslib/ast
require daslib/rst
require daslib/regex
require daslib/regex_boost
require daslib/fio
require strings
require daslib/strings_boost
require _rst_probe
require _rst_digest_mod
require _rst_distinct_mod
require _rst_pageless_mod
require daslib/enum_trait
require _rst_stub_mod_a
require _rst_stub_mod_b

def private write_twin(root, key, text : string) {
    let path = "{root}/handmade/handmade/{key}.rst"
    mkdir_rec(dir_name(path))
    fwrite(path, text)
}

def private bom() : string {
    let bytes <- [uint8(0xEF), uint8(0xBB), uint8(0xBF)]
    return string(bytes)
}

def private has_line(text, line : string) : bool => find(text, "\n{line}\n") >= 0

[test(description = "the rst digest builds module sections, groups, collapsed overloads, stub rows and tabular topics as listed")]
def test_rst_digest_rows(t : T?) {
    var mod = find_module("_rst_digest_mod")
    let mods : array<Module?> = [mod]
    var two_arg_key = ""
    var lonely_key = ""
    for_each_function(mod, "") $(fn) {
        if (fn.name == "digest_pair" && length(fn.arguments) == 2) {
            two_arg_key = "function-_rst_digest_mod-{function_label_file(fn)}"
        } elif (fn.name == "digest_lonely") {
            lonely_key = "function-_rst_digest_mod-{function_label_file(fn)}"
        }
    }
    t |> success(!empty(two_arg_key) && !empty(lonely_key), "the fixture functions' topic keys")
    var err : string
    let root = create_temp_directory("das_rst_digest", err)
    t |> success(empty(err), "temp directory: {err}")
    write_twin(root, "module-_rst_digest_mod", "Digest fixture module. It has a second sentence.\n\nA second paragraph stays out.\n")
    write_twin(root, two_arg_key, "Adds ``two`` numbers, see :ref:`digest_lonely <lbl-lonely>`. A second sentence.\n")
    write_twin(root, lonely_key, "// stub\ndef digest_lonely (a: int) : int\n")
    write_twin(root, "structure-_rst_digest_mod-DigestPoint", "A point in two axes\nx coordinate\ny coordinate\n")
    write_twin(root, "enumeration-_rst_digest_mod-DigestKind", "Kinds of digest\nthe first kind\nthe second kind\n")
    write_twin(root, "typedef-_rst_digest_mod-DigestFlags", "Two flags\nthe one bit\nthe two bit\n")
    write_twin(root, "Variable-_rst_digest_mod-DIGEST_LIMIT", "{bom()}The digest limit.\n")
    let saved_topic = topic_root
    let saved_handmade = handmade_root
    topic_root = "{root}/generated"
    handmade_root = "{root}/handmade"
    digest_path = "{root}/digest.md"
    ast_gc_guard() {
        var groups <- array<DocGroup>(
            group_by_regex("Pairs", mod, %regex~digest_pair$%%)
        )
        documents("Probe", mods, "probe.rst", groups, DocsHook())
        write_digest()
    }
    digest_path = ""
    topic_root = saved_topic
    handmade_root = saved_handmade
    let digest = fread("{root}/digest.md")
    t |> run("the module has a section with its intro paragraph and an index line with its first sentence") @(t : T?) {
        t |> success(has_line(digest, "## _rst_digest_mod"), "section heading: {digest}")
        t |> success(has_line(digest, "Digest fixture module. It has a second sentence."), "intro paragraph: {digest}")
        t |> success(find(digest, "second paragraph") < 0, "the paragraph ends at the blank line: {digest}")
        t |> success(has_line(digest, "- [_rst_digest_mod](#_rst_digest_mod) - Digest fixture module."), "index line carries the first sentence only: {digest}")
    }
    t |> run("groups become headings, an ungrouped function lands under Uncategorized") @(t : T?) {
        let pairs = find(digest, "\n### Pairs\n")
        let uncategorized = find(digest, "\n### Uncategorized\n")
        let pair_row = find(digest, "- `digest_pair`")
        let lonely_row = find(digest, "- `digest_lonely`")
        t |> success(pairs >= 0 && uncategorized >= 0, "both headings present: {digest}")
        t |> success(pairs < pair_row && pair_row < uncategorized && uncategorized < lonely_row, "each row sits under its heading: {digest}")
    }
    t |> run("overloads collapse to one row and a documented later overload fills it") @(t : T?) {
        t |> success(count(digest, "- `digest_pair`") == 1, "one row per name: {digest}")
        t |> success(has_line(digest, "- `digest_pair` - Adds `two` numbers, see `digest_lonely`."), "first sentence with RST markup stripped: {digest}")
    }
    t |> run("a symbol whose only topic is a stub keeps a bare row") @(t : T?) {
        t |> success(has_line(digest, "- `digest_lonely`"), "the stub's signature line is not a description: {digest}")
    }
    t |> run("tabular topics drop their per-entry tail and keep the description untruncated") @(t : T?) {
        t |> success(has_line(digest, "- `DigestPoint` - A point in two axes"), "struct row: {digest}")
        t |> success(has_line(digest, "- `DigestKind` - Kinds of digest"), "enum row: {digest}")
        t |> success(has_line(digest, "- `DigestFlags` - Two flags"), "bitfield row: {digest}")
        t |> success(find(digest, "coordinate") < 0 && find(digest, "kind\n") < 0 && find(digest, " bit\n") < 0, "entry lines stay out: {digest}")
    }
    t |> run("constants, classes and their methods have rows") @(t : T?) {
        t |> success(has_line(digest, "- `DIGEST_LIMIT` - The digest limit."), "constant row, the source's byte-order mark dropped: {digest}")
        t |> success(has_line(digest, "- `DigestBox`"), "class row: {digest}")
        t |> success(has_line(digest, "- `DigestBox.show`"), "method row: {digest}")
    }
    rmdir_rec(root, err)
}

[test(description = "distinct types get entries and links in signatures, and read their description from the typedef topic")]
def test_rst_distinct_types(t : T?) {
    let mods : array<Module?> = [find_module("_rst_distinct_mod")]
    let docs <- document_into_temp(t, mods, DocsHook())
    t |> run("a public distinct type is declared with its underlying type") @(t : T?) {
        t |> success(find(docs.page, ".. _handle-_rst_distinct_mod-RstMeters:") >= 0, "no label: {docs.page}")
        t |> success(find(docs.page, ".. das:attribute:: RstMeters = distinct float") >= 0, "no declaration: {docs.page}")
    }
    t |> run("a distinct type is not a structure macro") @(t : T?) {
        t |> success(find(docs.page, "Structure macros") < 0, "listed as a structure macro: {docs.page}")
    }
    t |> run("a private distinct type gets no entry") @(t : T?) {
        t |> success(find(docs.page, "_handle-_rst_distinct_mod-RstHidden") < 0, "private distinct type labeled: {docs.page}")
        t |> success(find(docs.page, "RstHidden = distinct") < 0, "private distinct type declared: {docs.page}")
    }
    t |> run("a signature naming a private distinct type prints it without a link") @(t : T?) {
        t |> success(find(docs.page, "**h** : RstHidden") >= 0, "no plain argument type: {docs.page}")
        t |> success(find(docs.page, "RstHidden <handle-") < 0, "links a label no page writes: {docs.page}")
    }
    t |> run("a signature names the distinct type and links it") @(t : T?) {
        t |> success(find(docs.page, "RstMeters <handle-_rst_distinct_mod-RstMeters>") >= 0, "no link: {docs.page}")
        t |> success(find(docs.page, "**m** : distinct") < 0, "argument printed as its base type: {docs.page}")
    }
    t |> run("the distinct type reads its description from the typedef topic") @(t : T?) {
        t |> success(find(docs.stubs, "|detail/typedef-_rst_distinct_mod-RstMeters|") >= 0, "no typedef stub: {docs.stubs}")
    }
    drop_run(docs)
}

[test(description = "a distinct type removed by the annotation hook gets no entry and no link")]
def test_rst_distinct_type_filtered_by_hook(t : T?) {
    let mods : array<Module?> = [find_module("_rst_distinct_mod")]
    var hook = DocsHook()
    hook.annotationFilter <- @(ann : rtti::Annotation const) : bool {
        return ann.name != "RstMeters"
    }
    let docs <- document_into_temp(t, mods, hook)
    t |> run("a filtered distinct type gets no entry and no group") @(t : T?) {
        t |> success(find(docs.page, "Distinct types") < 0, "empty group written: {docs.page}")
        t |> success(find(docs.page, "_handle-_rst_distinct_mod-RstMeters:") < 0, "filtered type labeled: {docs.page}")
    }
    t |> run("a signature naming a filtered type prints it without a link") @(t : T?) {
        t |> success(find(docs.page, "**m** : RstMeters") >= 0, "no plain argument type: {docs.page}")
        t |> success(find(docs.page, "RstMeters <handle-") < 0, "links a label no page writes: {docs.page}")
    }
    drop_run(docs)
}

[test(description = "a signature naming a pageless module's distinct type prints it without a link")]
def test_rst_distinct_type_in_a_pageless_module(t : T?) {
    mark_module_pageless("_rst_pageless_mod")
    let mods : array<Module?> = [find_module("_rst_pageless_mod")]
    let docs <- document_into_temp(t, mods, DocsHook())
    t |> run("a signature naming a pageless module's distinct type prints it without a link") @(t : T?) {
        t |> success(find(docs.page, "**i** : RstPagelessId") >= 0, "no plain argument type: {docs.page}")
        t |> success(find(docs.page, "RstPagelessId <handle-") < 0, "links into a module with no page: {docs.page}")
    }
    drop_run(docs)
}

[test(description = "each written constructor of a class or struct is documented and a default-only class documents none")]
def test_rst_written_constructors(t : T?) {
    let mods : array<Module?> = [find_module("_rst_distinct_mod")]
    let docs <- document_into_temp(t, mods, DocsHook())
    t |> run("each constructor the class declares is documented") @(t : T?) {
        t |> equal(count(docs.page, ".. das:function:: RstCamera("), 2, docs.page)
    }
    t |> run("a struct's written constructor is documented") @(t : T?) {
        t |> equal(count(docs.page, ".. das:function:: RstPoint("), 1, docs.page)
    }
    t |> run("a class with only the default constructor documents none") @(t : T?) {
        t |> success(find(docs.page, ".. das:function:: RstPlain(") < 0, "default constructor documented: {docs.page}")
    }
    drop_run(docs)
}

[test(description = "get_distinct_underlying_type throws for an annotation that is not a distinct type")]
def test_distinct_accessors_reject_other_annotations(t : T?) {
    var checked = false
    module_for_each_annotation(get_module("rtti_core")) $(value) {
        return if (checked || value.isDistinctTypeAnnotation)
        checked = true
        var threw = false
        try {
            feint("{get_distinct_underlying_type(value) != null}")
        } recover {
            threw = true
        }
        t |> success(threw, "get_distinct_underlying_type accepted {value.name}, which is not a distinct type")
    }
    t |> success(checked, "rtti_core carries no annotation to try")
}

[test(description = "the trailing doc comment of a distinct typedef reaches its typedef topic, and a daslib module required beside the reader gets topics for its body doc comments", timeout = 1200)]
def test_rst_comment_reads_distinct_typedef(t : T?) {
    var err : string
    let root = create_temp_directory("das_rst_comment", err)
    t |> success(empty(err), "temp directory: {err}")
    let exe = get_command_line_arguments()[0]
    let driver = "{get_das_root()}/tests/daslib/_rst_comment_driver.das"
    var output : string
    let exit_code = run_and_capture([exe, "-no-module-cache", "-dasroot", get_das_root(),
            driver, "--", "--detail_output", root], output)
    t |> run("the trailing //! of a distinct typedef reaches its typedef topic") @(t : T?) {
        t |> equal(exit_code, 0, output)
        let detail = fread("{root}/detail/typedef-_rst_distinct_mod-RstMeters.rst")
        t |> success(find(detail, "Distance along a path") >= 0, "detail: '{detail}' output: {output}")
    }
    t |> run("a daslib module required beside the reader gets its body //! comments") @(t : T?) {
        var lower_bound_topics = 0
        dir("{root}/detail") $(name) {
            if (name |> starts_with("function-algorithm-lower_bound-")) {
                lower_bound_topics++
            }
        }
        t |> success(lower_bound_topics > 0,
            "no function-algorithm-lower_bound detail topic; output: {output}")
    }
    rmdir_rec(root, err)
}

[test(description = "the label helpers escape every special character of a function name and spell an option type as its alternatives joined by a bar")]
def test_rst_label_helpers(t : T?) {
    t |> run("safe_function_name escapes") @(t : T?) {
        let escaped = ("a_b_builtin_c_hh_d_c_e_at_f_ls_g_gr_h_rq_i_ex_j_eq_k"
            + "_st_l_dot_m_q_n_ref_opq_lb_r_rb_st")
        t |> equal(safe_function_name("a b$c#d:e@f<g>h`i!j=k*l.m?n&o(p)q[r]s|t"), escaped)
    }
    t |> run("describe_type_short joins an option type") @(t : T?) {
        ast_gc_guard() {
            var td <- new TypeDecl(baseType = Type.option)
            td.argTypes |> push(new TypeDecl(baseType = Type.tInt))
            td.argTypes |> push(new TypeDecl(baseType = Type.tFloat))
            t |> equal(describe_type_short(td), "int|float")
        }
    }
}

[test(description = "annotationFilter reaches enumeration annotations too")]
def test_rst_enumeration_annotation_hook(t : T?) {
    let mods : array<Module?> = [find_module("enum_trait")]
    let unfiltered <- document_into_temp(t, mods, DocsHook())
    t |> success(find(unfiltered.page, "Handled enumerations") >= 0, "fixture module has no enumeration annotation")
    drop_run(unfiltered)
    var hook = DocsHook()
    hook.annotationFilter <- @(ann : rtti::Annotation const) : bool {
        return false
    }
    let filtered <- document_into_temp(t, mods, hook)
    t |> run("annotationFilter reaches the enumeration annotations too") @(t : T?) {
        t |> success(find(filtered.page, "Handled enumerations") < 0, "filtered-out annotations still documented")
    }
    drop_run(filtered)
}

[test(description = "the stub file keeps every module's symbols and the topic names the page reads back")]
def test_rst_substitution_stubs_span_every_module(t : T?) {
    let mods : array<Module?> = [find_module("_rst_stub_mod_a"), find_module("_rst_stub_mod_b")]
    let docs <- document_into_temp(t, mods, DocsHook())
    t |> run("the stub file keeps the first module's symbols too") @(t : T?) {
        t |> success(find(docs.stubs, "rst_stub_beta") >= 0, "last module missing: {docs.stubs}")
        t |> success(find(docs.stubs, "rst_stub_alpha") >= 0, "first module missing: {docs.stubs}")
    }
    t |> run("stub topic names are the ones the page reads back") @(t : T?) {
        for (name in fixed_array<string>("rst_stub_alpha", "rst_stub_alpha_two", "rst_stub_beta", "rst_stub_beta_two")) {
            let needle = "{name}-0x"
            t |> success(find(docs.stubs, needle) >= 0, "{name} stub carries no argument hash: {docs.stubs}")
        }
    }
    drop_run(docs)
}
