options gen2
options rtti = true
options no_aot
options strict_smart_pointers = false

require dastest/testing_boost public
require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require strings

//! Regression for three LineInfo range bugs (see utils/internal/lineinfo-audit):
//! 1. ExprLet.atInit used to include the lexer-synthesized statement-terminating
//!    semicolon, overshooting one phantom column past the end of line.
//! 2. Closure/block-arg bodies started one column early — the hidden mid-rule
//!    action before '{' in expression_block leaked its zero-width between-token
//!    location into the block span via ast_makeBlock.
//! 3. Class-method bodies are wrapped in a synthesized ExprBlock/ExprWith that
//!    was stamped with the method-NAME token range instead of the body's span.

let LINEINFO_FIXTURE = "{get_das_root()}/tests/ast/_lineinfo_ranges_fixture.das"


class RangeChecker : AstVisitor {
    src_lines : array<string>
    problems : array<string>

    def line_of(l : int) : string => (l >= 1 && l <= length(src_lines)) ? src_lines[l - 1] : ""

    def char_at(l, c : int) : string {
        let ln = line_of(l)
        return (c >= 0 && c < length(ln)) ? slice(ln, c, c + 1) : ""
    }

    def override preVisitExprLet(expr : ExprLet?) {
        if (expr.genFlags.generated) return
        let il = int(expr.atInit.last_line)
        let ilc = int(expr.atInit.last_column)
        if (il < 1) return
        if (ilc > length(line_of(il))) {
            problems |> push("atInit overshoots line end at {int(expr.at.line)}:{int(expr.at.column)} (..{il}:{ilc}, line is {length(line_of(il))})")
            return
        }
        // end-exclusive: last char of the span must be a real token char
        let last_ch = char_at(il, ilc - 1)
        if (empty(last_ch) || last_ch == " " || last_ch == ";") {
            problems |> push("atInit ends on '{last_ch}' at {il}:{ilc}")
        }
    }

    def override preVisitExprBlock(blk : ExprBlock?) {
        if (blk.genFlags.generated) return
        let l = int(blk.at.line)
        let ll = int(blk.at.last_line)
        let lc = int(blk.at.last_column)
        if (l < 1) return
        let opener = char_at(l, int(blk.at.column))
        let closer = char_at(ll, lc - 1)
        if (opener != "\{" || closer != "\}") {
            problems |> push("block at {l}:{int(blk.at.column)}..{ll}:{lc} spans '{opener}'..'{closer}', expected braces")
        }
    }
}


def check_fixture(t : T?; var checker : RangeChecker?) : bool {
    let text = fread(LINEINFO_FIXTURE)
    if (empty(text)) {
        t |> success(false, "could not read {LINEINFO_FIXTURE}")
        return false
    }
    var src_lines <- text |> split("\n")
    for (ln in src_lines) {
        if (ln |> ends_with("\r")) {
            ln = slice(ln, 0, length(ln) - 1)
        }
    }
    checker.src_lines <- src_lines
    var compiled = false
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.version_2_syntax = true
            cop.no_optimizations = true
            compile_file(LINEINFO_FIXTURE, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    t |> success(false, "fixture failed to compile: {issues}")
                    return
                }
                compiled = true
                let thisMod = get_this_module(program)
                for_each_function(thisMod, "") $(func) {
                    // skip reified generics from required modules — only walk
                    // functions written in the fixture
                    if (func.flags.builtIn || func.flags.generated || func.at.fileInfo == null
                            || !(func.at.fileInfo.name |> ends_with("_lineinfo_ranges_fixture.das"))) return
                    make_visitor(*checker) $(adapter) {
                        visit(func, adapter)
                    }
                }
            }
        }
    }
    return compiled
}


[test(description = "let atInit and block spans match the source text")]
def test_lineinfo_ranges(t : T?) {
    t |> run("let atInit and block spans match source") @(t : T?) {
        var checker = new RangeChecker()
        if (check_fixture(t, checker)) {
            for (p in checker.problems) {
                to_log(LOG_ERROR, "lineinfo: {p}\n")
            }
            t |> success(empty(checker.problems), "no lineinfo range violations (found {length(checker.problems)})")
        }
        unsafe {
            delete checker
        }
    }
}


//! The last line of a file may carry no newline. The lexer records where a line of code ends when
//! it consumes that newline, so a block closed by end of file used to end at the line before it.


class private SpanCollector : AstVisitor {
    spans : array<string>

    def override preVisitExprBlock(blk : ExprBlock?) {
        if (blk.genFlags.generated) return
        spans |> push("{int(blk.at.line)}:{int(blk.at.column)}..{int(blk.at.last_line)}:{int(blk.at.last_column)}")
    }
}


def private gen1_block_spans(path : string; var spans : array<string>) : bool {
    var parsed = false
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.version_2_syntax = false
            cop.no_optimizations = true
            parse_file(path, access, unsafe(addr(mg)), cop) $(ok; program; _issues) {
                if (!ok) return
                parsed = true
                var collector = new SpanCollector()
                for_each_function(get_this_module(program), "") $(var fn) {
                    if (fn.at.fileInfo == null || fn.flags.generated) return
                    make_visitor(*collector) $(adapter) {
                        visit(fn, adapter)
                    }
                }
                spans |> push_from(collector.spans)
                unsafe {
                    delete collector
                }
            }
        }
    }
    return parsed
}


[test(description = "a v1 block closed by the end of file ends at its last statement")]
def test_lineinfo_ranges_gen1_eof(t : T?) {
    t |> run("a v1 block closed by end of file ends at its last statement") @(t : T?) {
        let body = "def eof_block(n : int)\n    var acc = 0\n    for i in range(n)\n        acc += i\n    return acc"
        let nlFile = create_temp_file_result("lineinfo_gen1_nl", ".das")
        let eofFile = create_temp_file_result("lineinfo_gen1_eof", ".das")
        t |> success(nlFile is value && eofFile is value, "temporary fixtures created")
        if (!(nlFile is value && eofFile is value)) return
        let nlPath = unsafe(nlFile.value)
        let eofPath = unsafe(eofFile.value)
        t |> success(fwrite(nlPath, "{body}\n") && fwrite(eofPath, body), "temporary fixtures written")
        var nlSpans, eofSpans : array<string>
        let nlOk = gen1_block_spans(nlPath, nlSpans)
        let eofOk = gen1_block_spans(eofPath, eofSpans)
        remove(nlPath)
        remove(eofPath)
        t |> success(nlOk && eofOk, "both fixtures parsed")
        if (!(nlOk && eofOk)) return
        t |> equal(join(eofSpans, ", "), join(nlSpans, ", "))
        t |> success(!empty(eofSpans) && eofSpans[0] |> ends_with("..5:14"),
            "outer block ends at the last statement ({empty(eofSpans) ? "none" : eofSpans[0]})")
    }
}

//! `parse_file` hands over the AST as parsed, before infer. Source-rewriting tools need that
//! tree: infer lowers make-syntax, folds constants and inserts generated nodes, none of which
//! map back to a source span. The fixture is v1 syntax, so this also pins that a gen1 parse is
//! reachable from das (version_2_syntax = false).
//!
//! It further pins WHERE the spans are, because a rewriter's anchors depend on it: a make-node's
//! own `at` covers only its 2-char opener (`[[` / `\{\{`), while the enclosing `ExprLet.atInit`
//! reaches past the closer. A change in either is a deliberate decision, not a detail — this
//! test is the tripwire.
//!
//! The AST dies with the program, so every assertion runs INSIDE the parse_file block: an
//! ExpressionPtr kept past it is a dangling gc_node pointer.

let PARSE_FIXTURE = "{get_das_root()}/tests/ast/_fixture_parse_file_gen1.das"


def private read_lines(file : string) : array<string> {
    var lines <- fread(file) |> split("\n")
    for (ln in lines) {
        if (ln |> ends_with("\r")) {
            ln = slice(ln, 0, length(ln) - 1)
        }
    }
    return <- lines
}

def private text_at(lines : array<string>; line, column, count : int) : string {
    if (line < 1 || line > length(lines)) return ""
    let ln = lines[line - 1]
    if (column < 0 || column + count > length(ln)) return ""
    return slice(ln, column, column + count)
}

def private span_head(lines : array<string>; li : LineInfo; count : int) : string =>
    text_at(lines, int(li.line), int(li.column), count)

def private span_tail(lines : array<string>; li : LineInfo; count : int) : string =>
    text_at(lines, int(li.last_line), int(li.last_column) - count, count)


def private collect_inits(var blk : ExprBlock?; var inits : array<ExpressionPtr>;
                          var initEndLine, initEndColumn : array<int>) {
    for (stmt in blk.list) {
        var letExpr = stmt ?as ExprLet
        if (letExpr == null) continue
        for (v in letExpr.variables) {
            if (v.init == null) continue
            inits |> push(v.init)
            initEndLine |> push(int(letExpr.atInit.last_line))
            initEndColumn |> push(int(letExpr.atInit.last_column))
        }
    }
}


def private check_make_nodes(t : T?; var fn : FunctionPtr; lines : array<string>) {
    var blk = fn.body ?as ExprBlock
    if (blk == null) {
        t |> failure("make_them has no block body")
        return
    }
    var inits : array<ExpressionPtr>
    var initEndLine, initEndColumn : array<int>
    collect_inits(blk, inits, initEndLine, initEndColumn)
    t |> equal(length(inits), 4, "four initializers in make_them")
    if (length(inits) != 4) return

    // [[Foo a=1, b=2.0]] stays a make-struct; `useInitializer` records that no () was written
    let msd = inits[0] ?as ExprMakeStruct
    t |> success(msd != null, "first init is ExprMakeStruct")
    if (msd != null) {
        t |> success(!msd.makeStructFlags.useInitializer, "no () in [[Foo ...]] means useInitializer == false")
        t |> equal(length(msd.structs), 1, "one instance")
    }

    // [[auto 1; 2; 3]] — the v1 spelling is recorded in the tree, so a rewriter does not have
    // to consult the source to know which syntax was written
    let mka = inits[1] ?as ExprMakeArray
    t |> success(mka != null, "second init is ExprMakeArray")
    if (mka != null) {
        t |> success(!mka.gen2, "v1 make-array carries gen2 == false")
        t |> equal(length(mka.values), 3, "three values")
    }

    // [[auto 4, 5]] is a one-element make-array holding a make-tuple; infer is what collapses
    // that into a bare tuple, so a pre-infer walk has to expect the wrapper
    let mkt = inits[2] ?as ExprMakeArray
    t |> success(mkt != null, "[[auto 4, 5]] is ExprMakeArray")
    if (mkt != null) {
        t |> equal(length(mkt.values), 1, "one value")
        if (length(mkt.values) == 1) {
            t |> success(mkt.values[0] ?as ExprMakeTuple != null, "the value is ExprMakeTuple")
        }
    }

    // {{ k => v }} is lowered to a to_table_move call over a make-array AT PARSE TIME, so
    // make-table detection is call-shaped, not node-shaped
    let tabCall = inits[3] ?as ExprCall
    t |> success(tabCall != null, "fourth init is ExprCall")
    if (tabCall != null) {
        t |> success(find(string(tabCall.name), "to_table_move") >= 0, "call is to_table_move, got {tabCall.name}")
        t |> equal(length(tabCall.arguments), 1, "one argument")
        if (length(tabCall.arguments) == 1) {
            t |> success(tabCall.arguments[0] ?as ExprMakeArray != null, "argument is ExprMakeArray")
        }
    }

    check_make_spans(t, inits, initEndLine, initEndColumn, lines)
}


def private check_make_spans(t : T?; inits : array<ExpressionPtr>;
                             initEndLine, initEndColumn : array<int>; lines : array<string>) {
    // a make-node's own span is JUST the opener — the closer is nowhere in the tree, which is
    // why a rewriter has to scan forward for `]]` / `\}\}`
    t |> equal(span_head(lines, inits[0].at, 2), "[[", "make-struct span starts at [[")
    t |> equal(span_tail(lines, inits[0].at, 2), "[[", "make-struct span ENDS at [[ too")
    t |> equal(span_head(lines, inits[1].at, 2), "[[", "make-array span starts at [[")
    t |> equal(span_tail(lines, inits[1].at, 2), "[[", "make-array span ENDS at [[ too")
    t |> equal(span_head(lines, inits[3].at, 2), "\{\{", "make-table span starts at \{\{")

    // the enclosing let's atInit is the anchor that does include the closer
    t |> equal(text_at(lines, initEndLine[0], initEndColumn[0] - 2, 2), "]]", "atInit ends at ]]")
    t |> equal(text_at(lines, initEndLine[1], initEndColumn[1] - 2, 2), "]]", "atInit ends at ]]")
    t |> equal(text_at(lines, initEndLine[3], initEndColumn[3] - 2, 2), "\}\}", "atInit ends at \}\}")
}


[test(description = "v1 make-syntax and its spans survive parsing before inference")]
def test_parse_file_gen1_pre_infer(t : T?) {
    t |> run("v1 make-syntax and its spans survive the parse") @(t : T?) {
        let lines <- read_lines(PARSE_FIXTURE)
        if (empty(lines)) {
            t |> success(false, "could not read {PARSE_FIXTURE}")
            return
        }
        var seen = false
        var inscope access <- make_file_access("")
        using() $(var mg : ModuleGroup) {
            using() $(var cop : CodeOfPolicies) {
                cop.ignore_shared_modules = true
                cop.export_all = true
                cop.version_2_syntax = false
                cop.no_optimizations = true
                parse_file(PARSE_FIXTURE, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                    if (!ok) {
                        t |> success(false, "fixture failed to parse: {issues}")
                        return
                    }
                    for_each_function(get_this_module(program), "") $(var fn) {
                        // required modules contribute generic instances (builtin's
                        // to_table_move, which the \{\{...\}\} lowering emits) — only walk
                        // the fixture's own code
                        if (fn.at.fileInfo == null || fn.name != "make_them"
                                || !(fn.at.fileInfo.name |> ends_with("_fixture_parse_file_gen1.das"))) return
                        seen = true
                        check_make_nodes(t, fn, lines)
                    }
                }
            }
        }
        t |> success(seen, "make_them was found in the parsed module")
    }
}
