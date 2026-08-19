options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_source_model
require imgui/imgui_text_tree_sitter
require daslib/strings_boost

//! Grammar-drift canary: one tricky construct per section, each capped by a
//! generated marker function. A tree-sitter error region silently SWALLOWS
//! every construct after it (folds and outline just stop — the bare-named-args
//! gap hid exactly this way), so a missing marker fold names the section that
//! broke the grammar. ADDING SYNTAX TO ds2_parser.ypp? Add a section here.
//! Every snippet must stay real, compilable daslang — validate an edited
//! fixture by pasting the assembled source into a scratch file and running it.

struct private CanarySection {
    name : string
    snippet : string
}

let CANARY_SECTIONS : array<CanarySection> <- [
    CanarySection(name = "options + optional-require guard",
        snippet = "options gen2\noptions indenting = 4\n\nrequire ?zz_absent_guard daslib/algorithm\nrequire math\nrequire strings\nrequire daslib/stringify"),
    CanarySection(name = "enum with base type",
        snippet = "enum CanMode : uint8 \{\n    one\n    two\n\}"),
    CanarySection(name = "struct private + field default + @annotation",
        snippet = "struct private CanS \{\n    a : int = 7\n    @safe_when_uninitialized b : int\n\}"),
    CanarySection(name = "bitfield declaration",
        snippet = "bitfield CanFlags \{\n    alpha\n    beta\n    gamma\n\}"),
    CanarySection(name = "variant declaration",
        snippet = "variant CanV \{\n    i : int\n    f : float\n\}"),
    CanarySection(name = "typedef distinct (#3424)",
        snippet = "typedef distinct CanId = int"),
    CanarySection(name = "fixed-array typedef",
        snippet = "typedef CanRow = float[4]"),
    CanarySection(name = "defaulted parameters",
        snippet = "def named_stub(a : int; primary : bool = false; scope : int = 0) \{\n    pass\n\}"),
    CanarySection(name = "bare named call arguments (#3410)",
        snippet = "def can_bare_named() \{\n    named_stub(1, primary = true, scope = 2)\n    named_stub(3, primary = false)\n\}"),
    CanarySection(name = "bracketed named call arguments",
        snippet = "def can_bracket_named() \{\n    named_stub(1, [primary = true, scope = 2])\n    named_stub(4)\n\}"),
    CanarySection(name = "arrow-body function",
        snippet = "def can_arrow(a, b : int) : int => a + b"),
    CanarySection(name = "OR-type parameter",
        snippet = "def can_or_type(v : int | float) \{\n    print(\"x\\n\")\n    print(\"y\\n\")\n\}"),
    CanarySection(name = "generic auto(TT)",
        snippet = "def can_generic(x : auto(TT)) : auto \{\n    return x\n\}"),
    CanarySection(name = "postfix guards + multi-source for",
        snippet = "def can_flow() \{\n    var total = 0\n    for (index, twice in range(4), range(0, 8)) \{\n        continue if (index == 1)\n        total += twice\n        break if (total > 8)\n    \}\n    return if (total < 0)\n    print(\"\{total\}\\n\")\n\}"),
    CanarySection(name = "static_if",
        snippet = "def can_static_if() \{\n    static_if (true) \{\n        print(\"s\\n\")\n        print(\"t\\n\")\n    \}\n\}"),
    CanarySection(name = "lambdas + function pointers",
        snippet = "def can_lambdas() \{\n    let lam = @(x : int) => x + 1\n    let fp = @@(x : int) => x + 2\n    let a = invoke(lam, 1)\n    let b = invoke(fp, 2)\n    print(\"\{a + b\}\\n\")\n\}"),
    CanarySection(name = "with (module ...)",
        snippet = "def can_with_module() \{\n    with (module math) \{\n        let q = abs(-3)\n        print(\"\{q\}\\n\")\n    \}\n\}"),
    CanarySection(name = "classes + inheritance + override",
        snippet = "class CanBase \{\n    def def_can() : int \{\n        return 1\n    \}\n\}\n\nclass CanDerived : CanBase \{\n    def override def_can() : int \{\n        return 2\n    \}\n\}"),
    CanarySection(name = "casts: cast/upcast/reinterpret/addr<T?>",
        snippet = "def can_casts() \{\n    var iv = 5\n    let cv = int64(iv)\n    let ip = unsafe(addr<int?>(iv))\n    let rp = unsafe(reinterpret<uint8?>(ip))\n    var d = new CanDerived()\n    var b = cast<CanBase?>(d)\n    let back = unsafe(upcast<CanDerived?>(b))\n    print(\"\{cv\} \{ip != null\} \{rp != null\} \{b->def_can()\} \{back != null\}\\n\")\n    unsafe \{\n        delete d\n    \}\n\}"),
    CanarySection(name = "typeinfo/typedecl/default<>",
        snippet = "def can_typeinfo() \{\n    let cc = typeinfo can_copy(type<int>)\n    var dv = default<CanS>\n    let td : typedecl(dv.a) = 9\n    print(\"\{cc\} \{dv.a\} \{td\}\\n\")\n\}"),
    CanarySection(name = "container literals + comprehensions",
        snippet = "def can_literals() \{\n    var arr <- [1, 2, 3]\n    var tab <- \{ \"k\" => 1, \"j\" => 2 \}\n    var set_ : table<string> <- \{ \"a\", \"b\" \}\n    let fixed = fixed_array(1, 2, 3)\n    var comp <- [for (x in range(3)); x * x]\n    var tcomp <- \{ for (x in range(3)); x => x * x \}\n    print(\"\{length(arr)\} \{length(tab)\} \{length(set_)\} \{fixed[0]\} \{comp[2]\} \{length(tcomp)\}\\n\")\n    delete arr\n    delete tab\n    delete set_\n    delete comp\n    delete tcomp\n\}"),
    CanarySection(name = "make-struct + new with named fields",
        snippet = "def can_make_struct() \{\n    let s = CanS(a = 1)\n    var p = new CanS(a = 2)\n    print(\"\{s.a\} \{p.a\}\\n\")\n    unsafe \{\n        delete p\n    \}\n\}"),
    CanarySection(name = "tuple fields + variant is/as",
        snippet = "def can_tuple_variant() \{\n    let t = (1, 2.0f)\n    var v = CanV(i = 5)\n    if (v is i) \{\n        print(\"\{t._0\} \{v as i\}\\n\")\n    \}\n\}"),
    CanarySection(name = "string interpolation + escapes + multi-line",
        snippet = "def can_strings() \{\n    let x = 41\n    let s = \"value \\\{x\\\} is \{x\} padded \{x:d\}\"\n    let m = \"line one\nline two\"\n    print(\"\{s\} \{length(m)\}\\n\")\n\}"),
    CanarySection(name = "reader macro",
        snippet = "def can_reader_macro() \{\n    let blurb = %stringify~# Title\nbody line\n%%\n    print(\"\{length(blurb)\}\\n\")\n\}"),
    CanarySection(name = "finally + try/recover",
        snippet = "def can_finally_try() \{\n    var n = 0\n    \{\n        n = 1\n        n = 2\n    \} finally \{\n        n = 3\n    \}\n    try \{\n        panic(\"boom\")\n    \} recover \{\n        print(\"\{n\}\\n\")\n    \}\n\}"),
    CanarySection(name = "unsafe expression + block",
        snippet = "def can_unsafe_forms() \{\n    var q = 1\n    let pq = unsafe(addr(q))\n    unsafe \{\n        print(\"\{pq != null\}\\n\")\n        print(\"second\\n\")\n    \}\n\}"),
    CanarySection(name = "multi-dimensional fixed arrays",
        snippet = "def can_fixed_arrays() \{\n    var grid : int[3][4]\n    grid[0][1] = 5\n    var row : CanRow\n    row[2] = 1.5f\n    print(\"\{grid[0][1]\} \{row[2]\}\\n\")\n\}"),
    CanarySection(name = "numeric literal zoo",
        snippet = "def can_numbers() \{\n    let h = 0x3F\n    let u = 1ul\n    let d = 1.5lf\n    let hf = 1.5h\n    let ch = '('\n    print(\"\{h\} \{u\} \{d\} \{float(hf)\} \{ch\}\\n\")\n\}"),
    CanarySection(name = "local variable @annotation",
        snippet = "def can_local_annotation() \{\n    var @exact_size buf : array<int>\n    let @exact_size @other = 3 two = 2\n    buf |> reserve(two)\n    buf |> resize(two)\n    print(\"\{length(buf)\}\\n\")\n\}")
]

def private build_canary_source(var marker_rows : array<int>) : string {
    var parts : array<string>
    parts |> reserve(2 * length(CANARY_SECTIONS))
    var index = 0
    for (section in CANARY_SECTIONS) {
        parts |> push(section.snippet)
        parts |> push("def canary_marker_{index}() \{\n    print(\"a\\n\")\n    print(\"b\\n\")\n\}")
        index++
    }
    let source = join(parts, "\n\n")
    var inscope lines <- split(source, "\n")
    for (row in range(length(lines))) {
        if (lines[row] |> starts_with("def canary_marker_")) {
            marker_rows |> push(row)
        }
    }
    delete parts
    return source
}

[test]
def test_grammar_swallow_canary(t : T?) {
    var marker_rows : array<int>
    let source = build_canary_source(marker_rows)
    t |> equal(length(marker_rows), length(CANARY_SECTIONS),
        "one marker per section assembled")
    var document <- tree_sitter_source_document(source, "das", 1)
    var fold_rows : table<int>
    for (fold in document.folds) {
        if (fold.kind == "function_declaration") {
            fold_rows |> insert(fold.header_row)
        }
    }
    for (row, section in marker_rows, CANARY_SECTIONS) {
        t |> success(key_exists(fold_rows, row),
            "grammar still parses past section '{section.name}'")
    }
    delete fold_rows
    delete marker_rows
    text_source_document_dispose(document)
}
