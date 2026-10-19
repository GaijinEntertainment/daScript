options gen2
options indenting = 4

module lint_fixups shared private

//! Building blocks of the lint passes' ``--fix`` edits: the source
//! text of a node or a range, the checks a rewrite must pass, and
//! the ``LintFix`` constructors the rules share.

require daslib/ast_boost
require daslib/strings_boost
require daslib/cmp
require daslib/lint_config

//! A fix replacing the source range ``at`` with ``replacement``.
def public lint_fix(at : LineInfo; replacement : string) : LintFix =>
    LintFix(line = int(at.line), column = int(at.column), last_line = int(at.last_line),
            last_column = int(at.last_column), replacement = replacement)

//! The source range from the start of ``first`` to the end of
//! ``last``.
def public lint_range(first, last : LineInfo) : LineInfo {
    var at = first
    at.last_line = last.last_line
    at.last_column = last.last_column
    return at
}

//! The source text the range ``at`` covers, line breaks included.
def public lint_span_text(at : LineInfo) : string {
    return build_string() $(var w) {
        for (ln in range(int(at.line), int(at.last_line) + 1)) {
            let line = lint_source_line(at, ln)
            let from = ln == int(at.line) ? int(at.column) : 0
            let to = ln == int(at.last_line) ? int(at.last_column) : length(line)
            w |> write(slice(line, from, to))
            if (ln != int(at.last_line)) {
                w |> write("\n")
            }
        }
    }
}

//! The source text of ``e``; "" for a generated node.
def public lint_expr_text(e : Expression?) : string =>
    e == null || e.genFlags.generated ? "" : lint_span_text(e.at)

//! Whether ``text`` spells a comment opener; a literal spelling one
//! counts too, which only costs a fix.
def public lint_has_comment(text : string) : bool => find(text, "//") >= 0 || find(text, "/*") >= 0

//! A fix replacing the whole source of ``e`` with ``replacement``;
//! none for a generated node, an empty replacement, or a span holding
//! a comment.
def public lint_replace_fix(e : Expression?; replacement : string) : LintFix {
    let text = lint_expr_text(e)
    if (empty(text) || empty(replacement) || lint_has_comment(text)) return LintFix()
    return lint_fix(e.at, replacement)
}

//! A fix replacing the source range ``at``; none for an empty
//! replacement or a range holding a comment.
def public lint_span_fix(at : LineInfo; replacement : string) : LintFix {
    if (empty(replacement) || lint_has_comment(lint_span_text(at))) return LintFix()
    return lint_fix(at, replacement)
}

//! A fix replacing ``e`` with the call ``name(args)``; none when an
//! argument has no source.
def public lint_call_fix(e : Expression?; name : string; args : array<string>) : LintFix =>
    has_value(args, "") ? LintFix() : lint_replace_fix(e, "{name}({join(args, ", ")})")

//! Whether a fix may rewrite ``e`` in ``fn``: user source, and -
//! unless the rewrite holds for every instantiation - outside
//! generic instances.
def public lint_fix_site_ok(e : Expression?; fn : Function?; generic_safe : bool) : bool =>
    e != null && !e.genFlags.generated && (fn == null || generic_safe || fn.fromGeneric == null)

//! One fix spanning several edits to the file of ``at``, for a
//! rewrite touching separate places. None when ``edits`` is empty,
//! holds an edit without a span, or two edits overlap.
def public lint_merge_fixes(at : LineInfo; edits : array<LintFix>) : LintFix {
    if (empty(edits)) return LintFix()
    if (length(edits) == 1) return edits[0]
    var sorted := edits
    sort(sorted) <| cmp_by(_.line, _.column)
    for (k in range(length(sorted))) {
        return LintFix() if (sorted[k].line <= 0)
        if (k > 0) {
            let p = sorted[k - 1]
            let q = sorted[k]
            return LintFix() if (q.line < p.last_line
                                 || (q.line == p.last_line && q.column < p.last_column))
        }
    }
    let replacement = build_string() $(var w) {
        for (k in range(length(sorted))) {
            if (k > 0) {
                var gap = at
                gap.line = uint(sorted[k - 1].last_line)
                gap.column = uint(sorted[k - 1].last_column)
                gap.last_line = uint(sorted[k].line)
                gap.last_column = uint(sorted[k].column)
                w |> write(lint_span_text(gap))
            }
            w |> write(sorted[k].replacement)
        }
    }
    let last = sorted |> back()
    return LintFix(line = sorted[0].line, column = sorted[0].column, last_line = last.last_line,
                   last_column = last.last_column, replacement = replacement)
}

//! Whether ``e`` is one operand without parentheses - a name, a
//! constant, field, index, string builder, or ``name(args)`` call,
//! whose range is its enclosure, so nothing attaches outside it.
def public lint_is_operand(e : Expression?) : bool {
    if (e is ExprCall) {
        let enc = (e as ExprCall).atEnclosure
        return (enc.line == e.at.line && enc.column == e.at.column
                && enc.last_line == e.at.last_line && enc.last_column == e.at.last_column)
    }
    return (e is ExprVar || is_expr_const(e) || e is ExprField || e is ExprSafeField
        || e is ExprSwizzle || e is ExprAt || e is ExprSafeAt || e is ExprStringBuilder)
}

//! The source text of ``e`` as the operand of a tighter-binding
//! operator - parenthesized unless ``lint_is_operand``.
def public lint_operand_text(e : Expression?) : string {
    let text = lint_expr_text(e)
    return empty(text) || lint_is_operand(e) ? text : "({text})"
}

//! The width of the leading whitespace of ``line``.
def public lint_indent_of(line : string) : int => length(line) - length(strip_left(line))

//! Whether ``e`` alone fills its source lines - a single line under
//! ``one_line``.
def public lint_fills_lines(e : Expression?; one_line : bool) : bool {
    if (empty(lint_expr_text(e)) || (one_line && e.at.line != e.at.last_line)) return false
    let head = slice(lint_source_line(e.at, int(e.at.line)), 0, int(e.at.column))
    let tail = slice(lint_source_line(e.at, int(e.at.last_line)), int(e.at.last_column))
    return empty(strip(head)) && empty(strip(tail))
}

//! The edit replacing ``token`` at column ``col`` of line ``ln``;
//! none unless the source spells ``token`` there.
def public lint_token_fix(at : LineInfo; ln, col : int; token, replacement : string) : LintFix {
    if (col < 0
        || slice(lint_source_line(at, ln), col, col + length(token)) != token) return LintFix()
    return LintFix(line = ln, column = col, last_line = ln, last_column = col + length(token),
                   replacement = replacement)
}

//! Whether the module of ``fn`` sees module ``name`` - itself, a
//! require, or a public re-export of one.
def public lint_module_visible(fn : Function?; name : string) : bool {
    if (fn == null || fn._module == null) return false
    let m = find_module(compiling_program(), name)
    return m != null && is_visible_directly(fn._module, m)
}
