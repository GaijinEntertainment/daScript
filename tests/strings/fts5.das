options gen2

require dastest/testing_boost
require daslib/fts5_query

[test(description = "the FTS5 query compiler parses empty, single-term and prefix queries and case-folds term names")]
def test_compile_subset(t : T?) {
    t |> run("Empty query parses to root=-1") @(t : T?) {
        let q <- compile_text_match("")
        t |> equal(q.root, -1)
    }
    t |> run("Whitespace-only query parses to root=-1") @(t : T?) {
        let q <- compile_text_match("   \t\n  ")
        t |> equal(q.root, -1)
    }
    t |> run("Single term parses to a Term node") @(t : T?) {
        let q <- compile_text_match("hello")
        t |> equal(q.root >= 0, true)
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.Term, true)
        t |> equal(q.nodes[q.root].words[0], "hello")
        t |> equal(q.nodes[q.root].is_prefix, false)
    }
    t |> run("Prefix term parses with is_prefix") @(t : T?) {
        let q <- compile_text_match("hello*")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.Term, true)
        t |> equal(q.nodes[q.root].words[0], "hello")
        t |> equal(q.nodes[q.root].is_prefix, true)
    }
    t |> run("Term names are case-folded at parse time") @(t : T?) {
        let q <- compile_text_match("Hello")
        t |> equal(q.nodes[q.root].words[0], "hello")
    }
}


[test(description = "the compiler parses a quoted phrase and a phrase with a trailing star as a prefix phrase")]
def test_compile_phrase(t : T?) {
    t |> run("Quoted phrase parses to Phrase node") @(t : T?) {
        let q <- compile_text_match("\"quick brown fox\"")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.Phrase, true)
        t |> equal(length(q.nodes[q.root].words), 3)
        t |> equal(q.nodes[q.root].words[0], "quick")
        t |> equal(q.nodes[q.root].words[1], "brown")
        t |> equal(q.nodes[q.root].words[2], "fox")
        t |> equal(q.nodes[q.root].is_prefix, false)
    }
    t |> run("Phrase with trailing * is prefix phrase") @(t : T?) {
        let q <- compile_text_match("\"quick bro\"*")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.Phrase, true)
        t |> equal(q.nodes[q.root].is_prefix, true)
    }
}


[test(description = "the compiler parses implicit and explicit AND, OR, NOT and parentheses with AND binding tighter than OR")]
def test_compile_boolean(t : T?) {
    t |> run("Whitespace = implicit AND") @(t : T?) {
        let q <- compile_text_match("foo bar")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.AndOp, true)
    }
    t |> run("Explicit AND keyword") @(t : T?) {
        let q <- compile_text_match("foo AND bar")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.AndOp, true)
    }
    t |> run("OR keyword") @(t : T?) {
        let q <- compile_text_match("foo OR bar")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.OrOp, true)
    }
    t |> run("NOT keyword (binary)") @(t : T?) {
        let q <- compile_text_match("foo NOT bar")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.NotOp, true)
    }
    t |> run("Parentheses group sub-expressions") @(t : T?) {
        let q <- compile_text_match("(foo OR bar) AND baz")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.AndOp, true)
        let lkind = q.nodes[q.nodes[q.root].left].kind
        t |> equal(lkind == Fts5NodeKind.OrOp, true)
    }
    t |> run("Precedence: AND binds tighter than OR") @(t : T?) {
        // a OR b c  ->  a OR (b AND c)
        let q <- compile_text_match("a OR b c")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.OrOp, true)
        let rkind = q.nodes[q.nodes[q.root].right].kind
        t |> equal(rkind == Fts5NodeKind.AndOp, true)
    }
}


[test(description = "the compiler parses NEAR with the default and an explicit distance")]
def test_compile_near(t : T?) {
    t |> run("NEAR with default k") @(t : T?) {
        let q <- compile_text_match("NEAR(quick fox)")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.NearOp, true)
        t |> equal(q.nodes[q.root].near_k, 10)
        t |> equal(length(q.nodes[q.root].words), 2)
    }
    t |> run("NEAR with explicit k") @(t : T?) {
        let q <- compile_text_match("NEAR(quick fox, 3)")
        t |> equal(q.nodes[q.root].kind == Fts5NodeKind.NearOp, true)
        t |> equal(q.nodes[q.root].near_k, 3)
    }
}


[test(description = "try_compile reports an error for an unterminated phrase, an unmatched paren and malformed NEAR forms")]
def test_try_compile_errors(t : T?) {
    t |> run("Unterminated phrase reports error") @(t : T?) {
        let r <- try_compile_text_match("\"hello world")
        t |> equal(is_err(r), true)
    }
    t |> run("Unmatched '(' reports error") @(t : T?) {
        let r <- try_compile_text_match("(foo OR bar")
        t |> equal(is_err(r), true)
    }
    t |> run("NEAR without parens reports error") @(t : T?) {
        let r <- try_compile_text_match("NEAR foo bar")
        t |> equal(is_err(r), true)
    }
    t |> run("NEAR with single term reports error") @(t : T?) {
        let r <- try_compile_text_match("NEAR(solo)")
        t |> equal(is_err(r), true)
    }
    t |> run("NEAR with unterminated phrase reports error") @(t : T?) {
        let r <- try_compile_text_match("NEAR(\"foo bar)")
        t |> equal(is_err(r), true)
    }
    t |> run("NEAR with unterminated paren reports error") @(t : T?) {
        let r <- try_compile_text_match("NEAR(foo bar")
        t |> equal(is_err(r), true)
    }
}

[test(description = "evaluating an FTS5 query matches empty, single-term and prefix queries with ASCII case folding and punctuation as word breaks")]
def test_eval_subset(t : T?) {
    t |> run("Empty query matches everything") @(t : T?) {
        t |> equal(text_match("hello world", ""), true)
        t |> equal(text_match("", ""), true)
    }
    t |> run("Single term match") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "quick"), true)
        t |> equal(text_match("the quick brown fox", "slow"), false)
    }
    t |> run("Prefix term match") @(t : T?) {
        t |> equal(text_match("running jumps", "run*"), true)
        t |> equal(text_match("running jumps", "jump*"), true)
        t |> equal(text_match("running jumps", "walk*"), false)
        t |> equal(text_match("running jumps", "running*"), true)
    }
    t |> run("ASCII case fold (folded at parse and tokenize)") @(t : T?) {
        t |> equal(text_match("The Quick Brown Fox", "QUICK"), true)
        t |> equal(text_match("Running", "run*"), true)
    }
    t |> run("Punctuation tokenizes via each_word") @(t : T?) {
        t |> equal(text_match("the quick, brown fox.", "quick"), true)
        t |> equal(text_match("foo-bar baz", "foo"), true)
        t |> equal(text_match("foo-bar baz", "bar"), true)
    }
}


[test(description = "whitespace between terms means AND regardless of order, and empty terms are no-ops")]
def test_eval_implicit_and(t : T?) {
    t |> run("Whitespace = AND") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "quick fox"), true)
        t |> equal(text_match("the quick brown fox", "quick dog"), false)
        t |> equal(text_match("the quick brown fox", "quick"), true)
    }
    t |> run("Order does not matter for AND") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "fox quick"), true)
    }
    t |> run("Empty terms are no-ops") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "  quick   "), true)
    }
}


[test(description = "OR matches either side, NOT excludes the right side and parentheses override precedence")]
def test_eval_or_not(t : T?) {
    t |> run("OR matches if either side matches") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "quick OR slow"), true)
        t |> equal(text_match("the quick brown fox", "slow OR fast"), false)
    }
    t |> run("NOT excludes the right side") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "fox NOT lazy"), true)
        t |> equal(text_match("the lazy dog", "lazy NOT dog"), false)
    }
    t |> run("Parens override precedence") @(t : T?) {
        // (a OR b) AND c
        t |> equal(text_match("alpha gamma", "(alpha OR beta) gamma"), true)
        t |> equal(text_match("beta gamma", "(alpha OR beta) gamma"), true)
        t |> equal(text_match("delta gamma", "(alpha OR beta) gamma"), false)
    }
}


[test(description = "a phrase needs consecutive tokens, its trailing star matches a word prefix and a one-word phrase equals a term")]
def test_eval_phrase(t : T?) {
    t |> run("Phrase requires consecutive tokens") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "\"quick brown\""), true)
        t |> equal(text_match("the quick brown fox", "\"brown quick\""), false)
        t |> equal(text_match("the quick lazy brown fox", "\"quick brown\""), false)
    }
    t |> run("Phrase prefix matches last word as prefix") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "\"quick bro\"*"), true)
        t |> equal(text_match("the quick brown fox", "\"quick br\"*"), true)
        t |> equal(text_match("the quick brown fox", "\"quick xy\"*"), false)
    }
    t |> run("Single-word phrase = term") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "\"quick\""), true)
    }
}


[test(description = "NEAR matches terms within the default or explicit distance, including three terms")]
def test_eval_near(t : T?) {
    t |> run("NEAR(a b) with default k=10") @(t : T?) {
        // tokens within 10 positions
        t |> equal(text_match("alpha beta", "NEAR(alpha beta)"), true)
        t |> equal(text_match("alpha one two three beta", "NEAR(alpha beta)"), true)
    }
    t |> run("NEAR with explicit k limits proximity") @(t : T?) {
        t |> equal(text_match("alpha beta", "NEAR(alpha beta, 1)"), true)
        t |> equal(text_match("alpha one beta", "NEAR(alpha beta, 1)"), false)
        t |> equal(text_match("alpha one beta", "NEAR(alpha beta, 2)"), true)
    }
    t |> run("NEAR with three terms") @(t : T?) {
        t |> equal(text_match("alpha beta gamma", "NEAR(alpha beta gamma, 5)"), true)
        t |> equal(text_match("alpha gamma", "NEAR(alpha beta gamma, 5)"), false)
    }
}


[test(description = "evaluation folds Latin-1 case and treats Cyrillic and CJK characters as word characters")]
def test_eval_unicode(t : T?) {
    t |> run("Latin-1 case folding through whole pipeline") @(t : T?) {
        t |> equal(text_match("Élève NIÑO café", "élève"), true)
        t |> equal(text_match("Élève NIÑO café", "niño"), true)
        t |> equal(text_match("Élève NIÑO café", "café"), true)
    }
    t |> run("Cyrillic AND") @(t : T?) {
        t |> equal(text_match("Привет Мир", "привет мир"), true)
        t |> equal(text_match("Привет Мир", "привет тест"), false)
    }
    t |> run("CJK ideographs as word chars") @(t : T?) {
        t |> equal(text_match("中文 日本語 한국어", "日本語"), true)
        t |> equal(text_match("中文，日本語", "中文"), true)
    }
}


[test(description = "one pre-compiled query matches several texts")]
def test_compile_then_match(t : T?) {
    t |> run("Pre-compiled query matches multiple texts") @(t : T?) {
        let q <- compile_text_match("quick fox")
        t |> equal(text_match("the quick brown fox", q), true)
        t |> equal(text_match("a quick wild fox", q), true)
        t |> equal(text_match("a slow tortoise", q), false)
    }
}
