options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require lr1/lr1
require strings

// The LALR(1) engine on grammars assembled by hand: acceptance, the viable-prefix refusal, the next-byte
// set, probing whole strings, epsilon and left-recursive rules, and the verdicts on a conflicting and an
// ill-formed grammar.

def private digits() : ByteSet {
    var bs : ByteSet
    for (c in range('0', '9' + 1)) {
        bs[c] = true
    }
    return bs
}

def private seq(var out : array<GrammarSym>; items : array<GrammarSym>) {
    out |> push_from(items)
}

// expr ::= expr "+" term | term ; term ::= digit+ (left recursive, with an epsilon-free plus)
def private arith() : Grammar {
    var gb : GrammarBuilder
    let expr = rule_id(gb, "expr")
    let term = rule_id(gb, "term")
    var p1 : array<GrammarSym>
    seq(p1, [sym_rule(expr), sym_byte('+'), sym_rule(term)])
    add_production(gb, expr, p1)
    var p2 : array<GrammarSym>
    seq(p2, [sym_rule(term)])
    add_production(gb, expr, p2)
    var p3 : array<GrammarSym>
    seq(p3, [sym_bytes(digits())])
    add_production(gb, term, p3)
    var p4 : array<GrammarSym>
    seq(p4, [sym_rule(term), sym_bytes(digits())])
    add_production(gb, term, p4)
    return <- build_grammar(gb, expr)
}

[test]
def test_lr1_arith(t : T?) {
    t |> run("a left-recursive sum of numbers: sentences, refusals, the viable prefix") @(t : T?) {
        var g <- arith()
        t |> equal(g.err, "", "the grammar builds")
        t |> success(lr_matches(g, "42"), "one number")
        t |> success(lr_matches(g, "1+2+3+4+5"), "a sum")
        t |> success(lr_matches(g, "123+456"), "wider numbers")
        t |> success(!lr_matches(g, ""), "nothing is not a sentence")
        t |> success(!lr_matches(g, "+"), "a bare operator")
        t |> success(!lr_matches(g, "1+"), "a trailing operator is not complete")
        t |> success(!lr_matches(g, "12a45"), "a letter inside")
        var st <- lr_start(g)
        t |> success(lr_step_bytes(g, st, "12+"), "a valid prefix advances")
        t |> success(!lr_at_end(g, st), "... and is not complete after the operator")
        t |> success(!lr_step(g, st, uint8('+')), "the second operator is refused at once (viable prefix)")
        t |> success(lr_step(g, st, uint8('7')), "a digit continues")
        t |> success(lr_at_end(g, st), "... and completes the sentence")
        let nb = lr_next_bytes(g, st)
        t |> success(nb['+'] && nb['0'] && nb['9'] && !nb['a'] && !nb[' '], "after a number: an operator or another digit")
        t |> success(!lr_step_bytes(g, st, "+x"), "a refused probe leaves the stack where it was")
        t |> success(lr_at_end(g, st), "... still complete")
    }
}

[test]
def test_lr1_epsilon_and_nesting(t : T?) {
    t |> run("optional pieces and nesting: list ::= '[' items? ']' ; items ::= item | items ',' item ; item ::= 'a' | list") @(t : T?) {
        var gb : GrammarBuilder
        let list = rule_id(gb, "list")
        let opt = rule_id(gb, "opt")
        let items = rule_id(gb, "items")
        let item = rule_id(gb, "item")
        var p : array<GrammarSym>
        seq(p, [sym_byte('['), sym_rule(opt), sym_byte(']')])
        add_production(gb, list, p)
        var e : array<GrammarSym>
        add_production(gb, opt, e)
        var o : array<GrammarSym>
        seq(o, [sym_rule(items)])
        add_production(gb, opt, o)
        var i1 : array<GrammarSym>
        seq(i1, [sym_rule(item)])
        add_production(gb, items, i1)
        var i2 : array<GrammarSym>
        seq(i2, [sym_rule(items), sym_byte(','), sym_rule(item)])
        add_production(gb, items, i2)
        var a : array<GrammarSym>
        seq(a, [sym_byte('a')])
        add_production(gb, item, a)
        var nested : array<GrammarSym>
        seq(nested, [sym_rule(list)])
        add_production(gb, item, nested)
        var g <- build_grammar(gb, list)
        t |> equal(g.err, "", "the grammar builds")
        t |> success(lr_matches(g, "[]"), "the empty list (an epsilon production)")
        t |> success(lr_matches(g, "[a]"), "one item")
        t |> success(lr_matches(g, "[a,a,[a,[]],a]"), "nesting")
        t |> success(!lr_matches(g, "[a,]"), "a trailing comma")
        t |> success(!lr_matches(g, "[[a]"), "an unclosed list")
        var st <- lr_start(g)
        t |> success(lr_step_bytes(g, st, "[a,[a"), "deep inside")
        let nb = lr_next_bytes(g, st)
        t |> success(nb[','] && nb[']'] && !nb['['] && !nb['a'], "after an item: a comma or the close")
    }
}

[test]
def test_lr1_verdicts(t : T?) {
    t |> run("an ambiguous grammar reports its conflict and still accepts its language; an ill-formed one accepts nothing") @(t : T?) {
        // s ::= a | b ; a ::= 'x' ; b ::= 'x'  - a reduce/reduce conflict
        var gb : GrammarBuilder
        let s = rule_id(gb, "s")
        let a = rule_id(gb, "a")
        let b = rule_id(gb, "b")
        var p1 : array<GrammarSym>
        seq(p1, [sym_rule(a)])
        add_production(gb, s, p1)
        var p2 : array<GrammarSym>
        seq(p2, [sym_rule(b)])
        add_production(gb, s, p2)
        var x1 : array<GrammarSym>
        seq(x1, [sym_byte('x')])
        add_production(gb, a, x1)
        var x2 : array<GrammarSym>
        seq(x2, [sym_byte('x')])
        add_production(gb, b, x2)
        var g <- build_grammar(gb, s)
        t |> equal(g.err, "", "an ambiguous grammar still builds")
        t |> success(g.conflicts > 0 && find(g.first_conflict, "reduce/reduce") >= 0, "the conflict is counted and named: {g.first_conflict}")
        t |> success(lr_matches(g, "x") && !lr_matches(g, "xx"), "... and the acceptor forks through it: exactly the language")

        // loop ::= loop 'x' - derives no terminal string
        var gb2 : GrammarBuilder
        let loop = rule_id(gb2, "loop")
        var l : array<GrammarSym>
        seq(l, [sym_rule(loop), sym_byte('x')])
        add_production(gb2, loop, l)
        let g2 <- build_grammar(gb2, loop)
        t |> success(find(g2.err, "derives no terminal string") >= 0, "the unproductive rule is named: {g2.err}")

        // root ::= 'x' ; orphan ::= orphan 'y' - an unproductive rule the root never reaches is dead text, no defect
        var gb3 : GrammarBuilder
        let root = rule_id(gb3, "root")
        let orphan = rule_id(gb3, "orphan")
        var rx : array<GrammarSym>
        seq(rx, [sym_byte('x')])
        add_production(gb3, root, rx)
        var oy : array<GrammarSym>
        seq(oy, [sym_rule(orphan), sym_byte('y')])
        add_production(gb3, orphan, oy)
        let g3 <- build_grammar(gb3, root)
        t |> equal(g3.err, "", "an unreachable rule, productive or not, is ignored")
        t |> success(lr_matches(g3, "x") && !lr_matches(g3, "y"), "... and the language is the root's")
    }
}

[test]
def test_lr1_byte_classes(t : T?) {
    t |> run("overlapping byte sets partition into classes; a set spanning classes becomes alternatives") @(t : T?) {
        // root ::= hexdigit digit  - [0-9a-f] then [0-9]: the digits are one class, a-f another
        var gb : GrammarBuilder
        let root = rule_id(gb, "root")
        var hex : ByteSet = digits()
        for (c in range('a', 'f' + 1)) {
            hex[c] = true
        }
        var p : array<GrammarSym>
        seq(p, [sym_bytes(hex), sym_bytes(digits())])
        add_production(gb, root, p)
        var g <- build_grammar(gb, root)
        t |> equal(g.err, "", "the grammar builds")
        t |> equal(g.nclasses, 3, "digits, a-f, everything else")
        t |> success(lr_matches(g, "a7") && lr_matches(g, "37"), "both halves of the hex set lead to a digit")
        t |> success(!lr_matches(g, "7a") && !lr_matches(g, "g7"), "a letter second, or outside the set, is refused")
    }
}
