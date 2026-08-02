options gen2
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_text_source_model
require imgui/imgui_text_tree_sitter

//! Fold extraction from the tree-sitter structure walk — no UI, no live host.

let DAS_FIXTURE = "options gen2\n\ndef add(a, b : int) : int \{\n    let c = a + b\n    return c\n\}\n\nstruct Point \{\n    x : int\n    y : int\n\}\n\nenum Mode \{\n    one\n    two\n\}\n"

[test]
def test_das_fold_extraction(t : T?) {
    var document <- tree_sitter_source_document(DAS_FIXTURE, "das", 1)
    var kinds : table<string; int>
    for (fold in document.folds) {
        kinds[fold.kind]++
    }
    t |> equal(kinds?["function_declaration"] ?? 0, 1, "one function folds")
    t |> equal(kinds?["structure_declaration"] ?? 0, 1, "one struct folds")
    t |> equal(kinds?["enum_declaration"] ?? 0, 1, "one enum folds")
    var fn_index = -1
    for (index in range(length(document.folds))) {
        if (document.folds[index].kind == "function_declaration") {
            fn_index = index
        }
    }
    t |> success(fn_index >= 0, "function fold is present")
    if (fn_index >= 0) {
        let fold & = unsafe(document.folds[fn_index])
        t |> equal(fold.header_row, 2, "function header row is the def line")
        t |> equal(fold.end_row, 5, "function body ends at the closing brace row")
        let inside = text_source_fold_at(document, fold.start_byte + 4)
        t |> equal(inside, fn_index, "fold_at resolves a byte inside the function")
    }
    t |> equal(text_source_fold_at(document, 0), -1,
               "fold_at outside every region reports none")
    delete kinds
    text_source_document_dispose(document)
}

[test]
def test_das_folds_survive_bare_named_arguments(t : T?) {
    // the 0.6.4 bare named-call syntax was unparseable by the grammar and the
    // error region swallowed every fold after the first such call
    let source = ("options gen2\n\ndef caller() \{\n"
        + "    foo(1, primary = true, scope = 2)\n\}\n\n"
        + "def after() \{\n    let x = 1\n    let y = 2\n\}\n")
    var document <- tree_sitter_source_document(source, "das", 1)
    var functions = 0
    for (fold in document.folds) {
        if (fold.kind == "function_declaration") {
            functions++
        }
    }
    t |> equal(functions, 2, "the function AFTER a bare named call still folds")
    text_source_document_dispose(document)
}

[test]
def test_markdown_fold_extraction(t : T?) {
    let source = "# Title\n\nprose\n\n```\ncode line one\ncode line two\n```\n"
    var document <- markdown_source_document(source, 1)
    tree_sitter_collect_folds(document, "markdown", source)
    var fenced = 0
    for (fold in document.folds) {
        if (fold.kind == "fenced_code_block") {
            fenced++
        }
    }
    t |> equal(fenced, 1, "the fenced code block folds")
    t |> success(!empty(document.folds), "markdown source reports fold regions")
    text_source_document_dispose(document)
}
